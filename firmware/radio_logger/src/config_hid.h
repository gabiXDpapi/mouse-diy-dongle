/* Prototype identity only: validate USB->RF reads before CORE may write profiles. */
static const uint8_t config_hid_descriptor[] = {
    0x06, 0xff, 0xff, 0x09, 0x00, 0xa1, 0x01, 0x09, 0x00, 0x15,
    0x00, 0x25, 0x01, 0x75, 0x08, 0x95, 0x40, 0xb1, 0x02, 0xc0,
};
static uint8_t config_hid_response[64];
UDC_STATIC_BUF_DEFINE(config_hid_snapshot, 64);
static bool config_hid_pending, config_hid_claimed, config_hid_published;
static bool config_hid_query_active;
static bool config_hid_consumed, config_hid_locked_out;
static int64_t config_hid_deadline;
static uint8_t config_hid_request[64];
#ifdef CORE_PROTO_FULL
#define CONFIG_HID_QUEUE_CAP 4u
static uint8_t config_hid_queue[CONFIG_HID_QUEUE_CAP][64];
static uint8_t config_hid_q_head, config_hid_q_count;
#endif

static void config_hid_reset(void)
{
    config_hid_pending = config_hid_claimed = config_hid_published = false;
    config_hid_query_active = false;
    config_hid_consumed = config_hid_locked_out = false;
#ifdef CORE_PROTO_FULL
    config_hid_q_head = config_hid_q_count = 0;
#endif
    memset(config_hid_response, 0, sizeof(config_hid_response));
    config_hid_response[0] = 0xa3;
}

static int config_hid_set(const struct device *dev, struct usb_setup_packet *setup,
                          int32_t *len, uint8_t **data)
{
    ARG_UNUSED(dev);
    if (!setup || !len || !data || !*data || setup->wValue != 0x0300 ||
        setup->wLength != 64 || *len != 64) return -EINVAL;
    if (!config_request_valid(*data)) return -ENOTSUP;
    unsigned int key = irq_lock();
#ifdef CORE_PROTO_FULL
    if ((*data)[2] == 0 && (*data)[4] == 0 && (*data)[5] == 0x81) {
        if (config_hid_pending && !config_hid_published) { irq_unlock(key); return -EBUSY; }
        memset(config_hid_response, 0, sizeof(config_hid_response));
        config_hid_response[0] = 0xa1;
        config_hid_response[3] = 3;
        config_hid_response[5] = 0x81;
        config_hid_response[6] = 1; /* Receiver-local firmware version 1.0.0.0 */
        config_hid_pending = config_hid_claimed = config_hid_published = true;
        config_hid_query_active = config_hid_consumed = false;
        irq_unlock(key);
        return 0;
    }
    /* Queue back-to-back SET_REPORT calls (e.g. OpenMouse setDpi = stages + activeStage). */
    if (config_hid_pending && !config_hid_published) {
        if (config_hid_q_count >= CONFIG_HID_QUEUE_CAP) { irq_unlock(key); return -EBUSY; }
        uint8_t tail = (uint8_t)((config_hid_q_head + config_hid_q_count) % CONFIG_HID_QUEUE_CAP);
        memcpy(config_hid_queue[tail], *data, 64);
        config_hid_q_count++;
        irq_unlock(key);
        return 0;
    }
#else
    if (config_hid_locked_out || (config_hid_pending &&
        !(config_hid_published && config_hid_consumed))) { irq_unlock(key); return -EBUSY; }
#endif
    memcpy(config_hid_request, *data, 64);
    memcpy(config_hid_response, *data, 64);
    config_hid_response[0] = 0xa0;
    config_hid_claimed = config_hid_published = config_hid_consumed = false;
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
    if (config_hid_published) config_hid_consumed = true;
    irq_unlock(key);
    *data = config_hid_snapshot;
    *len = 64;
    return 0;
}

/* Main owns RF state; USB callbacks only touch the small IRQ-locked mailbox. */
static void config_hid_poll_link(bool mouse_linked)
{
    ARG_UNUSED(mouse_linked);
    unsigned int key = irq_lock();
    if (config_hid_pending && !config_hid_claimed) {
#ifdef CORE_PROTO_FULL
        if (!mouse_linked) {
            config_hid_response[0] = 0xa4; /* Receiver did not find mouse. */
            config_hid_claimed = config_hid_published = true;
            config_hid_query_active = false;
        } else {
            if (!config_read_begin(&config_read_state, config_hid_request)) config_read_state.failed = true;
            config_hid_claimed = config_hid_query_active = true;
            config_hid_deadline = k_uptime_get() + 4000;
        }
#else
        if (!config_read_begin(&config_read_state, config_hid_request)) config_read_state.failed = true;
        config_hid_claimed = config_hid_query_active = true;
        config_hid_deadline = k_uptime_get() + 4000;
#endif
    }
    if (config_hid_query_active && !config_hid_published) {
        if (!(config_read_state.complete && config_read_state.reply_released) &&
            k_uptime_get() >= config_hid_deadline) config_read_state.failed = true;
        if (config_read_state.complete && config_read_state.reply_released && !config_read_state.failed) {
            memcpy(config_hid_response, config_read_state.response, 64);
            config_hid_published = true;
#ifdef CORE_PROTO_FULL
            config_hid_query_active = false;
#endif
        } else if (config_read_state.failed) {
            config_hid_response[0] = 0xa2;
            config_hid_published = true;
#ifdef CORE_PROTO_FULL
            config_hid_query_active = false;
#else
            config_hid_locked_out = true;
#endif
        }
    }
#ifdef CORE_PROTO_FULL
    /* Drain queued back-to-back SET_REPORT requests as soon as the active RF slot finishes. */
    if (config_hid_published && !config_hid_query_active && config_hid_q_count > 0) {
        memcpy(config_hid_request, config_hid_queue[config_hid_q_head], 64);
        memcpy(config_hid_response, config_hid_request, 64);
        config_hid_response[0] = 0xa0;
        config_hid_q_head = (uint8_t)((config_hid_q_head + 1u) % CONFIG_HID_QUEUE_CAP);
        config_hid_q_count--;
        config_hid_claimed = config_hid_published = config_hid_consumed = false;
        config_hid_pending = true;
    }
#endif
    irq_unlock(key);
}

#ifndef CORE_PROTO_FULL
static void config_hid_poll(void)
{
    config_hid_poll_link(true);
}
#endif

static int init_config_hid(void)
{
    static const struct hid_ops ops = { .get_report = config_hid_get, .set_report = config_hid_set };
    const struct device *dev = device_get_binding("HID_1");
    if (!dev || !device_is_ready(dev)) return -ENODEV;
    config_hid_reset();
    usb_hid_register_device(dev, config_hid_descriptor, sizeof(config_hid_descriptor), &ops);
    return usb_hid_init(dev);
}
