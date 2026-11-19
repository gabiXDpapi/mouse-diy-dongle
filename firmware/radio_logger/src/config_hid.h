/* Prototype identity only: validate USB->RF reads before CORE may write profiles. */
static const uint8_t config_hid_descriptor[] = {
    0x06, 0xff, 0xff, 0x09, 0x00, 0xa1, 0x01, 0x15, 0x00,
    0x26, 0xff, 0x00, 0x75, 0x08, 0x95, 0x40, 0xb1, 0x02, 0xc0,
};
static uint8_t config_hid_response[64];
UDC_STATIC_BUF_DEFINE(config_hid_snapshot, 64);
static bool config_hid_pending, config_hid_claimed, config_hid_published;
static bool config_hid_query_active;

static void config_hid_reset(void)
{
    config_hid_pending = config_hid_claimed = config_hid_published = false;
    config_hid_query_active = false;
    memset(config_hid_response, 0, sizeof(config_hid_response));
    config_hid_response[0] = 0xa3;
}

static int config_hid_set(const struct device *dev, struct usb_setup_packet *setup,
                          int32_t *len, uint8_t **data)
{
    ARG_UNUSED(dev);
    if (!setup || !len || !data || !*data || setup->wValue != 0x0300 ||
        setup->wLength != 64 || *len != 64) return -EINVAL;
    if (memcmp(*data, config_read_version_request, 64)) return -ENOTSUP;
    unsigned int key = irq_lock();
    /* shortcut: one query per boot, expand after multi-command recovery is qualified. */
    if (config_hid_pending) { irq_unlock(key); return -EBUSY; }
    memcpy(config_hid_response, config_read_version_request, 64);
    config_hid_response[0] = 0xa0;
    config_hid_pending = true;
    irq_unlock(key);
    return 0;
}

static int config_hid_get(const struct device *dev, struct usb_setup_packet *setup,
                          int32_t *len, uint8_t **data)
{
    ARG_UNUSED(dev);
    if (!setup || !len || !data || setup->wValue != 0x0300 || setup->wLength != 64) return -EINVAL;
    unsigned int key = irq_lock();
    memcpy(config_hid_snapshot, config_hid_response, 64);
    irq_unlock(key);
    *data = config_hid_snapshot;
    *len = 64;
    return 0;
}

/* Main owns RF state; USB callbacks only touch the small IRQ-locked mailbox. */
static void config_hid_poll(void)
{
    unsigned int key = irq_lock();
    if (config_hid_pending && !config_hid_claimed) {
        memset(&config_read_state, 0, sizeof(config_read_state));
        config_hid_claimed = config_hid_query_active = true;
    }
    if (config_hid_query_active && !config_hid_published &&
        (config_read_state.complete || config_read_state.failed)) {
        if (config_read_state.complete && !config_read_state.failed) {
            memcpy(config_hid_response, config_read_state.response, 64);
        } else {
            config_hid_response[0] = 0xa2;
        }
        config_hid_published = true;
    }
    irq_unlock(key);
}

static int init_config_hid(void)
{
    static const struct hid_ops ops = { .get_report = config_hid_get, .set_report = config_hid_set };
    const struct device *dev = device_get_binding("HID_1");
    if (!dev || !device_is_ready(dev)) return -ENODEV;
    config_hid_reset();
    usb_hid_register_device(dev, config_hid_descriptor, sizeof(config_hid_descriptor), &ops);
    return usb_hid_init(dev);
}
