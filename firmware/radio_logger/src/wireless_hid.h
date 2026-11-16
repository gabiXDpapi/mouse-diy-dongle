/* Independent six-byte mouse report: buttons, signed16 X/Y, signed8 wheel. */
#define WIRELESS_HID_REPORT_SIZE 6u
static const uint8_t wireless_hid_descriptor[] = {
    0x05, 0x01, 0x09, 0x02, 0xa1, 0x01, 0x09, 0x01, 0xa1, 0x00,
    0x05, 0x09, 0x19, 0x01, 0x29, 0x05, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x05, 0x81, 0x02,
    0x75, 0x03, 0x95, 0x01, 0x81, 0x03,
    0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x16, 0x00, 0x80,
    0x26, 0xff, 0x7f, 0x75, 0x10, 0x95, 0x02, 0x81, 0x06,
    0x09, 0x38, 0x15, 0x81, 0x25, 0x7f, 0x75, 0x08, 0x95, 0x01,
    0x81, 0x06, 0xc0, 0xc0,
};
static const struct device *wireless_hid_device;
static K_SEM_DEFINE(wireless_hid_slot, 1, 1);
static atomic_t wireless_hid_online, wireless_hid_failed, wireless_hid_restart;
static bool wireless_hid_has_sequence;
static uint8_t wireless_hid_last_sequence, wireless_hid_last_buttons;
static unsigned int wireless_hid_reports, wireless_hid_retries, wireless_hid_busy;
UDC_STATIC_BUF_DEFINE(wireless_hid_report, WIRELESS_HID_REPORT_SIZE);
BUILD_ASSERT(sizeof(wireless_hid_report) >= WIRELESS_HID_REPORT_SIZE);

static void wireless_hid_ready(const struct device *dev)
{
    ARG_UNUSED(dev);
#ifdef LATENCY_DIAGNOSTICS
    latency_usb_complete();
#endif
    k_sem_give(&wireless_hid_slot);
}

static void wireless_hid_status(enum usb_dc_status_code status, const uint8_t *param)
{
    ARG_UNUSED(param);
    if (status == USB_DC_CONFIGURED) {
        if (!atomic_get(&wireless_hid_failed)) {
            atomic_set(&wireless_hid_online, 1);
        }
    } else if (status == USB_DC_RESET || status == USB_DC_DISCONNECTED || status == USB_DC_ERROR) {
        if (atomic_get(&wireless_hid_online)) {
            atomic_set(&wireless_hid_failed, 1);
            if (status != USB_DC_ERROR) {
                atomic_set(&wireless_hid_restart, 1);
            }
        }
        atomic_clear(&wireless_hid_online);
    }
}

static void wireless_hid_restart_if_needed(void)
{
    /* Restart outside the USB callback; main applies the selected startup policy. */
    if (atomic_get(&wireless_hid_restart)) {
        sys_reboot(SYS_REBOOT_COLD);
    }
}

static int init_wireless_hid(void)
{
    static const struct hid_ops ops = { .int_in_ready = wireless_hid_ready };
    wireless_hid_device = device_get_binding("HID_0");
    if (!wireless_hid_device || !device_is_ready(wireless_hid_device)) {
        return -ENODEV;
    }
    usb_hid_register_device(wireless_hid_device, wireless_hid_descriptor,
                            sizeof(wireless_hid_descriptor), &ops);
    return usb_hid_init(wireless_hid_device);
}

static int wireless_hid_write(const uint8_t report[WIRELESS_HID_REPORT_SIZE], int wait_ms)
{
    if (!atomic_get(&wireless_hid_online)) {
        return -ENODEV;
    }
    if (k_sem_take(&wireless_hid_slot, K_MSEC(wait_ms)) != 0) {
        return wait_ms ? -ETIMEDOUT : -EAGAIN;
    }
    /* The SDK pads the DMA allocation; only six report bytes belong on the wire. */
    memcpy(wireless_hid_report, report, WIRELESS_HID_REPORT_SIZE);
    uint32_t written = 0;
#ifdef LATENCY_DIAGNOSTICS
    bool measured = !wait_ms && latency_submit_begin();
#endif
    int err = hid_int_ep_write(wireless_hid_device, wireless_hid_report,
                               WIRELESS_HID_REPORT_SIZE, &written);
#ifdef LATENCY_DIAGNOSTICS
    latency_submit_result(measured, !err && written == WIRELESS_HID_REPORT_SIZE);
#endif
    if (err) {
        k_sem_give(&wireless_hid_slot);
        return err;
    }
    wireless_hid_last_buttons = report[0];
    return written == WIRELESS_HID_REPORT_SIZE ? 0 : -EIO;
}

static void wireless_hid_new_link(void)
{
    wireless_hid_has_sequence = false;
}

static bool wireless_hid_new_session(void)
{
    if (!atomic_get(&wireless_hid_online) || atomic_get(&wireless_hid_failed) ||
        atomic_get(&wireless_hid_restart)) {
        return false;
    }
#ifdef LATENCY_DIAGNOSTICS
    if (!latency_reset()) return false;
#endif
    wireless_hid_new_link();
    wireless_hid_reports = wireless_hid_retries = wireless_hid_busy = 0;
    return true;
}

static int wireless_hid_forward(const uint8_t *p)
{
    if (!atomic_get(&wireless_hid_online)) {
        return -ENODEV;
    }
    if (p[0] == 2) {
        return 0; /* Idle does not advance the transport sequence. */
    }
    if (p[0] == 11 && p[3] == 0x45) {
        /* Status advancing transport prevents a later wrapped report looking like a retry. */
        if (wireless_hid_has_sequence && wireless_hid_last_sequence != p[1]) {
            wireless_hid_has_sequence = false;
        }
        return 0;
    }
    if ((p[0] != 11 && p[0] != 20) || p[3] != 0x41 || (p[0] == 20 && p[12] != 0x45)) {
        return -EINVAL;
    }
    if (wireless_hid_has_sequence && wireless_hid_last_sequence == p[1]) {
        wireless_hid_retries++;
        return 0;
    }
    uint8_t report[6] = {p[4] & 0x1f, p[6], p[7], p[8], p[9], p[5]};
    int err = wireless_hid_write(report, 0);
    if (err == -EAGAIN) {
        wireless_hid_busy++;
    }
    if (!err) {
        wireless_hid_reports++;
        wireless_hid_last_sequence = p[1];
        wireless_hid_has_sequence = true;
    }
    return err;
}

static int wireless_hid_finish(void)
{
    if (!wireless_hid_last_buttons) {
        return 0;
    }
    const uint8_t released[6] = {0};
    int err = wireless_hid_write(released, 1000);
    if (err) {
        return err;
    }
    if (k_sem_take(&wireless_hid_slot, K_MSEC(1000)) != 0) {
        return -ETIMEDOUT;
    }
    k_sem_give(&wireless_hid_slot);
    return 0;
}
