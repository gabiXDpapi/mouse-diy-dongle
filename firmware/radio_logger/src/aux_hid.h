/* Auxiliary HID interface: Consumer (ID 1), System Control (ID 2),
 * Keyboard (ID 3 + LED Output), and Vendor Notifications FFA0/FFFF (IDs 4/5).
 */
#ifdef RF_CONFIG_HID_DIAGNOSTICS
#define AUX_HID_DEVICE_NAME "HID_2"
#else
#define AUX_HID_DEVICE_NAME "HID_1"
#endif
#define AUX_HID_MAX_REPORT_SIZE 9u

static const uint8_t aux_hid_descriptor[] = {
    /* Report ID 1: Consumer Control (16-bit usage array, 0x0000..0x023C) */
    0x05, 0x0c, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x01,
    0x19, 0x00, 0x2a, 0x3c, 0x02, 0x15, 0x00, 0x26, 0x3c, 0x02,
    0x95, 0x01, 0x75, 0x10, 0x81, 0x00, 0xc0,
    /* Report ID 2: System Control (Power Down, Sleep, Wake Up) */
    0x05, 0x01, 0x09, 0x80, 0xa1, 0x01, 0x85, 0x02,
    0x19, 0x81, 0x29, 0x83, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x03, 0x81, 0x02, 0x95, 0x05, 0x81, 0x01, 0xc0,
    /* Report ID 3: Standard 6KRO Keyboard + 5-bit LED Output */
    0x05, 0x01, 0x09, 0x06, 0xa1, 0x01, 0x85, 0x03,
    0x05, 0x07, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
    0x95, 0x01, 0x75, 0x08, 0x81, 0x03,
    0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02,
    0x95, 0x01, 0x75, 0x03, 0x91, 0x03,
    0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x25, 0x65,
    0x05, 0x07, 0x19, 0x00, 0x29, 0x65, 0x81, 0x00, 0xc0,
    /* Report ID 4: Vendor Notification FFA0:0001 (8 bytes) */
    0x06, 0xa0, 0xff, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x04,
    0x09, 0x01, 0x15, 0x00, 0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x08, 0x81, 0x02, 0xc0,
    /* Report ID 5: Vendor Notification FFFF:0001 (8 bytes) */
    0x06, 0xff, 0xff, 0x09, 0x01, 0xa1, 0x01, 0x85, 0x05,
    0x09, 0x01, 0x15, 0x00, 0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x08, 0x81, 0x02, 0xc0,
};

static const struct device *aux_hid_device;
static K_SEM_DEFINE(aux_hid_slot, 1, 1);
static bool aux_hid_kbd_active, aux_hid_consumer_active, aux_hid_system_active;
static unsigned int aux_hid_reports, aux_hid_busy;
UDC_STATIC_BUF_DEFINE(aux_hid_report, AUX_HID_MAX_REPORT_SIZE);
BUILD_ASSERT(sizeof(aux_hid_report) >= AUX_HID_MAX_REPORT_SIZE);

static void aux_hid_ready(const struct device *dev)
{
    ARG_UNUSED(dev);
    k_sem_give(&aux_hid_slot);
}

static int aux_hid_set_report(const struct device *dev, struct usb_setup_packet *setup,
                              int32_t *len, uint8_t **data)
{
    ARG_UNUSED(dev);
    if (!setup || !len || !data || !*data) {
        return -EINVAL;
    }
    /* Accept host Keyboard LED Output reports (Output type=2, Report ID=3). */
    if (setup->wValue == 0x0203 && (setup->wLength == 1 || setup->wLength == 2) &&
        *len == (int32_t)setup->wLength) {
        return 0;
    }
    return -ENOTSUP;
}

static int init_aux_hid(void)
{
    static const struct hid_ops ops = {
        .int_in_ready = aux_hid_ready,
        .set_report = aux_hid_set_report,
    };
    aux_hid_device = device_get_binding(AUX_HID_DEVICE_NAME);
    if (!aux_hid_device || !device_is_ready(aux_hid_device)) {
        return -ENODEV;
    }
    aux_hid_kbd_active = aux_hid_consumer_active = aux_hid_system_active = false;
    aux_hid_reports = aux_hid_busy = 0;
    usb_hid_register_device(aux_hid_device, aux_hid_descriptor,
                            sizeof(aux_hid_descriptor), &ops);
    return usb_hid_init(aux_hid_device);
}

static bool aux_hid_frame_valid(const uint8_t *p)
{
    if (p[0] < 2 || p[0] > 64) {
        return false;
    }
    if (p[0] == 2) {
        return true;
    }
    unsigned int at = 3, count = 0;
    while (at <= p[0]) {
        if (count >= 8u) {
            return false;
        }
        uint8_t header = p[at++];
        unsigned int len = (header >> 3) & 15u;
        unsigned int type = header & 7u;
        bool cont = (header & 0x80u) != 0;
        if (at + len > (unsigned int)p[0] + 1) {
            return false;
        }
        bool ok = (!cont && ((type == 1 && len == 8) || (type == 2 && len == 8) ||
                             (type == 3 && len == 2) || (type == 4 && len == 1) ||
                             (type == 5 && len == 8) || (type == 6 && len == 8)))
#ifdef CORE_PROTO_FULL
                  || (type == 7 && len == 8)
#endif
                  ;
        if (!ok) {
            return false;
        }
        at += len;
        count++;
    }
    return count > 0 && at == (unsigned int)p[0] + 1;
}

static bool aux_hid_nonzero(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (data[i] != 0) {
            return true;
        }
    }
    return false;
}

static int aux_hid_write(const uint8_t *report, uint32_t size, int wait_ms)
{
    if (!atomic_get(&wireless_hid_online)) {
        return -ENODEV;
    }
    if (!report || size == 0 || size > AUX_HID_MAX_REPORT_SIZE) {
        return -EINVAL;
    }
    if (k_sem_take(&aux_hid_slot, K_MSEC(wait_ms)) != 0) {
        return wait_ms ? -ETIMEDOUT : -EAGAIN;
    }
    memcpy(aux_hid_report, report, size);
    uint32_t written = 0;
    int err = hid_int_ep_write(aux_hid_device, aux_hid_report, size, &written);
    if (err) {
        k_sem_give(&aux_hid_slot);
        return err;
    }
    return written == size ? 0 : -EIO;
}

static int aux_hid_forward_record(uint8_t type, const uint8_t *payload, uint8_t len)
{
    uint8_t buf[AUX_HID_MAX_REPORT_SIZE] = {0};
    uint32_t size = 0;
    if (type == 2 && len == 8) {
        buf[0] = 0x03;
        memcpy(buf + 1, payload, 8);
        size = 9;
    } else if (type == 3 && len == 2) {
        buf[0] = 0x01;
        buf[1] = payload[0];
        buf[2] = payload[1];
        size = 3;
    } else if (type == 4 && len == 1) {
        buf[0] = 0x02;
        buf[1] = payload[0] & 0x07u;
        size = 2;
    } else if (type == 5 && len == 8) {
        buf[0] = 0x04;
        memcpy(buf + 1, payload, 8);
        size = 9;
    } else if (type == 6 && len == 8) {
        buf[0] = 0x05;
        memcpy(buf + 1, payload, 8);
        size = 9;
    } else {
        return -EINVAL;
    }
    int err = aux_hid_write(buf, size, 0);
    if (err == -EAGAIN) {
        aux_hid_busy++;
    }
    if (!err) {
        aux_hid_reports++;
        if (type == 2) {
            aux_hid_kbd_active = aux_hid_nonzero(payload, 8);
        } else if (type == 3) {
            aux_hid_consumer_active = aux_hid_nonzero(payload, 2);
        } else if (type == 4) {
            aux_hid_system_active = (buf[1] != 0);
        }
    }
    return err;
}

static int aux_hid_release_one(uint8_t report_id, uint32_t size, bool *active)
{
    if (!*active) {
        return 0;
    }
    uint8_t released[AUX_HID_MAX_REPORT_SIZE] = {0};
    released[0] = report_id;
    int err = aux_hid_write(released, size, 1000);
    if (err) {
        return err;
    }
    if (k_sem_take(&aux_hid_slot, K_MSEC(1000)) != 0) {
        return -ETIMEDOUT;
    }
    k_sem_give(&aux_hid_slot);
    *active = false;
    return 0;
}

static int aux_hid_finish(void)
{
    int err = aux_hid_release_one(0x03, 9, &aux_hid_kbd_active);
    if (err) return err;
    err = aux_hid_release_one(0x01, 3, &aux_hid_consumer_active);
    if (err) return err;
    return aux_hid_release_one(0x02, 2, &aux_hid_system_active);
}
