/* Fresh power-on also sends standalone type5 status; ACK it without HID output. */
#ifdef WIRELESS_HID
#define PROBE_ACK_LIMIT 1024u
#define PROBE_DATA_WINDOW_MS 10000u
#else
#define PROBE_ACK_LIMIT 256u
#define PROBE_DATA_WINDOW_MS 2000u
#endif
static bool probe_sent;
static unsigned int ack_count;
static int64_t probe_arm_deadline;
static int64_t ack_deadline;
static uint8_t probe_reply[] __aligned(4) = {3, 0, 0xf4, 1};
static uint8_t ack_reply[3] __aligned(4);

static void poll_probe_arm(void)
{
    unsigned char command;
    const struct device *console = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (uart_poll_in(console, &command) == 0 && command == 'p' &&
        !probe_sent && probe_arm_deadline == 0) {
        probe_arm_deadline = k_uptime_get() + 30000;
        printk("PROBE armed for 30 seconds; one discovery reply plus %u data ACKs maximum in %u ms\n",
               PROBE_ACK_LIMIT, PROBE_DATA_WINDOW_MS);
    }
}

static bool is_discovery(const uint8_t *p, uint8_t channel, bool pairing_address,
                         uint32_t match)
{
    if (pairing_address || match != 1 || p[0] != 5 || p[1] != 0x20 ||
        (channel != channels[0] && channel != channels[1] && channel != channels[2])) {
        return false;
    }
    for (unsigned int i = 0; i < 4; i++) {
        if (p[i + 2] != (uint8_t)(MOUSE_BASE >> (8 * i))) {
            return false;
        }
    }
    return true;
}

static void probe_self_test(void)
{
    uint8_t request[] = {5, 0x20, MOUSE_BASE & 0xff, (MOUSE_BASE >> 8) & 0xff,
                         (MOUSE_BASE >> 16) & 0xff, MOUSE_BASE >> 24};
    __ASSERT(is_discovery(request, channels[0], false, 1), "Valid discovery rejected");
    __ASSERT(!is_discovery(request, channels[0], true, 1), "Wrong base accepted");
    __ASSERT(!is_discovery(request, channels[0], false, 0), "Wrong match accepted");
    __ASSERT(!is_discovery(request, channels[3], false, 1), "Data channel accepted");
    request[0] = 4;
    __ASSERT(!is_discovery(request, channels[0], false, 1), "Wrong length accepted");
    request[0] = 5;
    request[1] = 0x21;
    __ASSERT(!is_discovery(request, channels[0], false, 1), "Wrong type accepted");
    request[1] = 0x20;
    request[2] ^= 1;
    __ASSERT(!is_discovery(request, channels[0], false, 1), "Wrong mouse accepted");
}

static int transmit_probe_packet(uint8_t *reply, uint32_t delay_us)
{
    probe_timing_before_tx();
    NRF_RADIO->TXADDRESS = 1;
    NRF_RADIO->TXPOWER = 0;
    NRF_RADIO->PACKETPTR = (uint32_t)reply;
    NRF_RADIO->EVENTS_END = 0;
    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->SHORTS = RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;
    /* shortcut: software turnaround is approximate; use hardware timing for sustained ACKs. */
    k_busy_wait(delay_us);
    __DMB();
    NRF_RADIO->TASKS_TXEN = 1;
    bool ended = false;
    for (int i = 0; i < 2000; i++) {
        if (NRF_RADIO->EVENTS_END) {
            ended = true;
            break;
        }
        k_busy_wait(1);
    }
    int err = disable_radio();
    NRF_RADIO->PACKETPTR = (uint32_t)packet;
    probe_timing_after_tx(ended);
    return err ? err : (ended ? 0 : -ETIMEDOUT);
}

static int send_probe(void)
{
    if (probe_sent) {
        return -EALREADY;
    }
    if (probe_arm_deadline == 0 || k_uptime_get() >= probe_arm_deadline) {
        return -EACCES;
    }
    /* Consume each budget even if transmission times out. */
    probe_sent = true;
    int err = transmit_probe_packet(probe_reply, 250);
    if (!err) {
        ack_deadline = k_uptime_get() + PROBE_DATA_WINDOW_MS;
    }
    return err;
}

static bool is_ack_candidate(const uint8_t *p, uint8_t channel, bool pairing_address,
                              uint32_t match)
{
    return !pairing_address && match == 1 && channel == channels[3] &&
           (p[0] == 2 || (p[0] == 11 && (p[3] == 0x41 || p[3] == 0x45)) ||
            (p[0] == 20 && p[3] == 0x41 && p[12] == 0x45));
}

static int send_report_ack(const uint8_t *p)
{
    if (ack_count >= PROBE_ACK_LIMIT) {
        return -EALREADY;
    }
    if (!probe_sent || ack_deadline == 0 || k_uptime_get() >= ack_deadline) {
        return -EACCES;
    }
#ifdef WIRELESS_HID
    int hid_err = wireless_hid_forward(p);
    if (hid_err) {
        return hid_err;
    }
#endif
    ack_count++;
    /* Stock 0x16088..0x160b8 frees a queued report on sequence+1; length 2 carries no command. */
    ack_reply[0] = 2;
    ack_reply[1] = (uint8_t)(p[1] + 1);
    ack_reply[2] = p[2] & 0x3f;
    int err = transmit_probe_packet(ack_reply, 0);
    if (err) {
        ack_count = PROBE_ACK_LIMIT;
    }
    return err;
}
