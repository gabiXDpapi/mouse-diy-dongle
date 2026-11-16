/* Fresh power-on also sends standalone type5 status; ACK it without HID output. */
#ifdef WIRELESS_HID
#define PROBE_ACK_LIMIT 983040u
#define PROBE_DATA_WINDOW_MS 900000u
#define RECEIVER_DISCOVERY_LIMIT 8u
#define RECEIVER_LINK_GAP_MS 1000u
static unsigned int receiver_discoveries;
static int64_t receiver_session_deadline, receiver_last_ack_ms;
static int64_t receiver_next_stats_ms;
static bool receiver_trace_next_ack;
static bool receiver_disarm_requested, receiver_disarm_handled;
static int64_t receiver_next_command_ms;
static unsigned int receiver_idle_acks, receiver_status_acks, receiver_type1_acks;
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

static bool probe_discovery_allowed(void)
{
    if (probe_sent || probe_arm_deadline == 0) {
        return false;
    }
#ifdef WIRELESS_HID
    if (receiver_disarm_requested) {
        return false;
    }
    if (receiver_discoveries >= RECEIVER_DISCOVERY_LIMIT || ack_count >= PROBE_ACK_LIMIT) {
        return false;
    }
    if (receiver_session_deadline) {
        return k_uptime_get() < receiver_session_deadline;
    }
#endif
    return k_uptime_get() < probe_arm_deadline;
}

#ifdef WIRELESS_HID
static bool receiver_diagnostics_due(void)
{
    if (probe_sent || k_uptime_get() < receiver_next_stats_ms) {
        return false;
    }
    receiver_next_stats_ms = k_uptime_get() + 1000;
    return true;
}

static bool receiver_link_should_stop(void)
{
    return probe_sent && (receiver_disarm_requested || ack_count >= PROBE_ACK_LIMIT || k_uptime_get() >= ack_deadline ||
           k_uptime_get() - receiver_last_ack_ms >= RECEIVER_LINK_GAP_MS);
}

/* Caller disables radio before neutralization; failure must prevent rediscovery. */
static int receiver_end_link(void)
{
    int err = wireless_hid_finish();
    if (err) {
        return err;
    }
    printk("RECEIVER link stopped: ACKs=%u/%u discoveries=%u/%u; neutral complete; idle=%u status=%u type1=%u hid_reports=%u retries=%u busy=%u\n",
           ack_count, PROBE_ACK_LIMIT, receiver_discoveries, RECEIVER_DISCOVERY_LIMIT,
           receiver_idle_acks, receiver_status_acks, receiver_type1_acks,
           wireless_hid_reports, wireless_hid_retries, wireless_hid_busy);
    probe_sent = false;
    ack_deadline = 0;
    return 0;
}

static int receiver_handle_disarm(void)
{
    if (!receiver_disarm_requested || receiver_disarm_handled) {
        return 0;
    }
    int err = disable_radio();
    if (err) {
        return err;
    }
    err = probe_sent ? receiver_end_link() : wireless_hid_finish();
    if (err) {
        return err;
    }
    receiver_disarm_handled = true;
    printk("RECEIVER disarmed: neutral complete; reboot required\n");
    return 0;
}
#endif

static void poll_probe_arm(void)
{
    unsigned char command;
    const struct device *console = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (uart_poll_in(console, &command) != 0) {
        return;
    }
#ifdef WIRELESS_HID
    if (command == 'x') {
        receiver_disarm_requested = true;
        return;
    }
    if (receiver_disarm_requested) {
        return;
    }
#endif
    if (command == 'p' && !probe_sent && probe_arm_deadline == 0) {
        probe_arm_deadline = k_uptime_get() + 30000;
#ifdef WIRELESS_HID
        printk("RECEIVER armed: first discovery within 30000 ms; up to %u discoveries and %u ACKs total in %u ms; gap=%u ms\n",
               RECEIVER_DISCOVERY_LIMIT, PROBE_ACK_LIMIT, PROBE_DATA_WINDOW_MS, RECEIVER_LINK_GAP_MS);
#else
        printk("PROBE armed for 30 seconds; one discovery reply plus %u data ACKs maximum in %u ms\n",
               PROBE_ACK_LIMIT, PROBE_DATA_WINDOW_MS);
#endif
    }
}

#ifdef WIRELESS_HID
static void receiver_poll_commands(void)
{
    int64_t now = k_uptime_get();
    if (now >= receiver_next_command_ms) {
        receiver_next_command_ms = now + 10;
        poll_probe_arm();
    }
}
#endif

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
#ifdef WIRELESS_HID
    /* Keep one discovery/first-ACK timing sample per link without per-packet USB text. */
    if (delay_us || receiver_trace_next_ack) {
        probe_timing_after_tx(ended);
    }
    if (!delay_us) {
        receiver_trace_next_ack = false;
    }
#else
    probe_timing_after_tx(ended);
#endif
    return err ? err : (ended ? 0 : -ETIMEDOUT);
}

static int send_probe(void)
{
    if (probe_sent) {
        return -EALREADY;
    }
    if (!probe_discovery_allowed()) {
        return -EACCES;
    }
    /* Consume each budget even if transmission times out. */
    probe_sent = true;
#ifdef WIRELESS_HID
    receiver_discoveries++;
#endif
    int err = transmit_probe_packet(probe_reply, 250);
    if (!err) {
#ifdef WIRELESS_HID
        if (!receiver_session_deadline) {
            receiver_session_deadline = k_uptime_get() + PROBE_DATA_WINDOW_MS;
        }
        ack_deadline = receiver_session_deadline;
        receiver_last_ack_ms = k_uptime_get();
        receiver_trace_next_ack = true;
        wireless_hid_new_link();
#else
        ack_deadline = k_uptime_get() + PROBE_DATA_WINDOW_MS;
#endif
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
    if (receiver_link_should_stop()) {
        return -EACCES;
    }
    int hid_err = wireless_hid_forward(p);
    if (hid_err) {
        return hid_err;
    }
#endif
    ack_count++;
    /* Stock 0x16088..0x160b8 frees a queued report on sequence+1; length 2 carries no command. */
    ack_reply[0] = 2;
#ifdef WIRELESS_HID
    /* Avoid releasing a report enqueued while an idle packet was in flight. */
    ack_reply[1] = p[0] == 2 ? p[1] : (uint8_t)(p[1] + 1);
#else
    ack_reply[1] = (uint8_t)(p[1] + 1);
#endif
    ack_reply[2] = p[2] & 0x3f;
    int err = transmit_probe_packet(ack_reply, 0);
    if (err) {
        ack_count = PROBE_ACK_LIMIT;
    }
#ifdef WIRELESS_HID
    else {
        receiver_last_ack_ms = k_uptime_get();
        if (p[0] == 2) {
            receiver_idle_acks++;
        } else if (p[3] == 0x45) {
            receiver_status_acks++;
        } else {
            receiver_type1_acks++;
        }
    }
#endif
    return err;
}
