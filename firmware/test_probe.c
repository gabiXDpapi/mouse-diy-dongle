#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
/* Avoid Windows' interactive assertion dialog in automated failing tests. */
#define assert(condition) do { if (!(condition)) { \
    fprintf(stderr, "Assertion failed: %s at line %d\n", #condition, __LINE__); \
    exit(1); \
} } while (0)
#ifndef EALREADY
#define EALREADY 114
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#define __aligned(n) __attribute__((aligned(n)))
#define __ASSERT(condition, message) assert(condition)
#define __DMB() ((void)0)
#define printk(...) ((void)0)
#define MOUSE_BASE 0x78563412u
#define DEVICE_DT_GET(x) ((const struct device *)0)
#define DT_CHOSEN(x) 0
#define RADIO_SHORTS_READY_START_Msk 1
#define RADIO_SHORTS_END_DISABLE_Msk 2
struct device { int unused; };
static struct {
    uint32_t TXADDRESS, TXPOWER, PACKETPTR, EVENTS_END, EVENTS_DISABLED, SHORTS, TASKS_TXEN;
} radio;
#define NRF_RADIO (&radio)
static const uint8_t channels[] = {3, 49, 78, 51};
static uint8_t packet[65];
static int64_t now;
static int command = -1;
static bool complete_tx;
static int turnaround_delay;
static int64_t k_uptime_get(void) { return now; }
static int uart_poll_in(const struct device *d, unsigned char *c)
{
    (void)d;
    if (command < 0) return -1;
    *c = (unsigned char)command;
    command = -1;
    return 0;
}
static void k_busy_wait(int us)
{
    if (us != 1) turnaround_delay = us;
    if (radio.TASKS_TXEN && complete_tx) radio.EVENTS_END = 1;
}
static int disable_radio(void) { radio.TASKS_TXEN = 0; return 0; }
static void probe_timing_before_tx(void) { }
static unsigned int timing_calls;
static void probe_timing_after_tx(bool ended) { (void)ended; timing_calls++; }
#ifdef WIRELESS_HID
static int hid_result;
static unsigned int hid_calls;
static int finish_result;
static unsigned int new_link_calls;
static int wireless_hid_finish(void) { return finish_result; }
static void wireless_hid_new_link(void) { new_link_calls++; }
static int wireless_hid_forward(const uint8_t *p)
{
    (void)p;
    hid_calls++;
    return hid_result;
}
#endif
#include "radio_logger/src/discovery_probe.h"
#ifdef WIRELESS_HID
_Static_assert(PROBE_ACK_LIMIT == 65536u, "Finite receiver budget accommodates observed quiet ACK cadence");
_Static_assert(PROBE_DATA_WINDOW_MS == 60000u, "Receiver session is sixty seconds total");
_Static_assert(RECEIVER_DISCOVERY_LIMIT == 8u, "Discovery retries are bounded");
_Static_assert(RECEIVER_LINK_GAP_MS == 1000u, "Release after one second without successful ACK");
#else
_Static_assert(PROBE_ACK_LIMIT == 256u, "Probe must retain its finite 256-ACK cap");
_Static_assert(PROBE_DATA_WINDOW_MS == 2000u, "Non-HID probe window must stay at two seconds");
#endif
int main(void)
{
    uint8_t report[12] = {11, 0xff, 0xff, 0x41};
    probe_self_test();
    assert(is_ack_candidate(report, 51, false, 1));
    assert(!is_ack_candidate(report, 49, false, 1));
    assert(!is_ack_candidate(report, 51, true, 1));
    assert(!is_ack_candidate(report, 51, false, 0));
    report[0] = 10; assert(!is_ack_candidate(report, 51, false, 1));
    report[0] = 11; report[3] = 0x42; assert(!is_ack_candidate(report, 51, false, 1));
    report[3] = 0x41;
    uint8_t idle[] = {2, 0x11, 0};
    assert(is_ack_candidate(idle, 51, false, 1));
    assert(!is_ack_candidate(idle, 49, false, 1));
    assert(!is_ack_candidate(idle, 51, true, 1));
    assert(!is_ack_candidate(idle, 51, false, 0));
    idle[0] = 1; assert(!is_ack_candidate(idle, 51, false, 1));
    idle[0] = 3; assert(!is_ack_candidate(idle, 51, false, 1));
    idle[0] = 2;
    uint8_t status_packet[] = {11, 0, 0, 0x45, 6, 1, 0, 0, 0, 0, 0, 0};
    assert(is_ack_candidate(status_packet, 51, false, 1));
    assert(!is_ack_candidate(status_packet, 49, false, 1));
    assert(!is_ack_candidate(status_packet, 51, true, 1));
    assert(!is_ack_candidate(status_packet, 51, false, 0));
    uint8_t compound[65] = {20, 0x0f, 0, 0x41, 0, 0, 0, 0, 0xff, 0xff, 0, 0,
                            0x45, 6, 1, 0, 0, 0, 0, 0, 0};
    assert(is_ack_candidate(compound, 51, false, 1));
    assert(!is_ack_candidate(compound, 49, false, 1));
    assert(!is_ack_candidate(compound, 51, true, 1));
    assert(!is_ack_candidate(compound, 51, false, 0));
    for (unsigned int length = 0; length <= 64; length++) {
        compound[0] = length;
        assert(is_ack_candidate(compound, 51, false, 1) ==
               (length == 2 || length == 11 || length == 20));
    }
    compound[0] = 20;
    compound[3] = 0x45; assert(!is_ack_candidate(compound, 51, false, 1));
    compound[3] = 0x41;
    compound[12] = 0x41; assert(!is_ack_candidate(compound, 51, false, 1));
    compound[12] = 0xc5; assert(!is_ack_candidate(compound, 51, false, 1));
    compound[12] = 0x45;
    assert(send_report_ack(report) == -EACCES && ack_count == 0);
#ifdef WIRELESS_HID
    assert(hid_calls == 0);
#endif
    assert(send_probe() == -EACCES && !probe_sent);
    now = 1000;
    command = 'x'; poll_probe_arm(); assert(probe_arm_deadline == 0);
    command = 'p'; poll_probe_arm(); assert(probe_arm_deadline == 31000);
    now = 40000;
    command = 'p'; poll_probe_arm(); assert(probe_arm_deadline == 31000);
    assert(now >= probe_arm_deadline);
    assert(send_probe() == -EACCES && !probe_sent);
    /* Simulate a fresh boot and arm it for the transmission-path checks. */
    probe_arm_deadline = 0;
    command = 'p'; poll_probe_arm();
    assert(send_probe() == -ETIMEDOUT);
#ifdef WIRELESS_HID
    assert(receiver_discoveries == 1 && receiver_session_deadline == 0 && new_link_calls == 0);
#endif
    assert(ack_deadline == 0 && send_report_ack(report) == -EACCES);
    assert(probe_sent && radio.PACKETPTR == (uint32_t)packet);
    assert(send_probe() == -EALREADY);
    /* A fresh boot has a new discovery and bounded ACK budget. */
    probe_sent = false;
#ifdef WIRELESS_HID
    receiver_discoveries = 0;
#endif
    complete_tx = true;
    assert(send_probe() == 0);
    assert(turnaround_delay == 250);
    assert(probe_sent && radio.TXADDRESS == 1 && radio.TXPOWER == 0);
    assert(ack_deadline == now + PROBE_DATA_WINDOW_MS);
    assert(radio.PACKETPTR == (uint32_t)packet && radio.TASKS_TXEN == 0);
    assert(send_probe() == -EALREADY);
    now = ack_deadline;
    assert(send_report_ack(report) == -EACCES && ack_count == 0);
    now--;
    int64_t original_deadline = ack_deadline;
#ifdef WIRELESS_HID
    receiver_last_ack_ms = now;
    unsigned int calls_before = hid_calls;
    hid_result = -EAGAIN;
    assert(send_report_ack(report) == -EAGAIN && ack_count == 0 && radio.TASKS_TXEN == 0);
    hid_result = -EIO;
    assert(send_report_ack(report) == -EIO && ack_count == 0 && radio.TASKS_TXEN == 0);
    assert(receiver_type1_acks == 0 && receiver_idle_acks == 0 && receiver_status_acks == 0);
    assert(hid_calls == calls_before + 2);
    hid_result = 0;
#endif
    assert(send_report_ack(report) == 0 && ack_count == 1);
#ifdef WIRELESS_HID
    assert(receiver_type1_acks == 1 && receiver_idle_acks == 0 && receiver_status_acks == 0);
#endif
    assert(turnaround_delay == 0);
    assert(ack_reply[0] == 2 && ack_reply[1] == 0 && ack_reply[2] == 0x3f);
    /* Idle sequence can stay unchanged while the outgoing queue is empty. */
    assert(send_report_ack(idle) == 0 && ack_count == 2);
    assert(ack_reply[0] == 2 && ack_reply[2] == 0);
#ifdef WIRELESS_HID
    assert(ack_reply[1] == 0x11); /* Idle must not release a newly queued report. */
#else
    assert(ack_reply[1] == 0x12); /* Preserve the non-HID control. */
#endif
    assert(send_report_ack(idle) == 0 && ack_count == 3);
    assert(send_report_ack(status_packet) == 0 && ack_count == 4);
#ifdef WIRELESS_HID
    assert(receiver_type1_acks == 1 && receiver_idle_acks == 2 && receiver_status_acks == 1);
#endif
    assert(ack_reply[1] == 1 && ack_reply[2] == 0);
    now = original_deadline;
    assert(send_report_ack(idle) == -EACCES && ack_count == 4);
    now--;
    report[1] = 0;
    for (unsigned int i = 4; i < PROBE_ACK_LIMIT; i++) {
        assert(send_report_ack(report) == 0 && ack_count == i + 1);
        assert(ack_reply[1] == (uint8_t)(report[1] + 1));
        assert(ack_deadline == original_deadline);
        report[1]++;
    }
    assert(send_report_ack(report) == -EALREADY && ack_count == PROBE_ACK_LIMIT);
#ifdef WIRELESS_HID
    assert(receiver_link_should_stop());
#endif
    command = 'p'; poll_probe_arm();
    assert(send_probe() == -EALREADY && ack_count == PROBE_ACK_LIMIT);
    /* Failure consumes all remaining attempts, so there is no automatic retry. */
    ack_count = 1; complete_tx = false;
    assert(send_report_ack(report) == -ETIMEDOUT && ack_count == PROBE_ACK_LIMIT);
#ifdef WIRELESS_HID
    assert(receiver_type1_acks + receiver_idle_acks + receiver_status_acks == PROBE_ACK_LIMIT);
#endif
    assert(send_report_ack(report) == -EALREADY);
#ifdef WIRELESS_HID
    /* New arm, total budgets and deadline survive rediscovery. */
    now = 100000; probe_sent = false; probe_arm_deadline = 0; ack_count = 0;
    ack_deadline = 0; receiver_session_deadline = 0; receiver_discoveries = 0;
    complete_tx = true; command = 'p'; poll_probe_arm();
    assert(probe_discovery_allowed());
    unsigned int links_before = new_link_calls;
    assert(send_probe() == 0 && new_link_calls == links_before + 1);
    int64_t session_deadline = receiver_session_deadline;
    assert(!receiver_diagnostics_due()); /* No periodic USB text in an active link. */
    assert(receiver_trace_next_ack);
    unsigned int timing_before_ack = timing_calls;
    assert(session_deadline == 160000 && receiver_discoveries == 1);
    now += 999; assert(!receiver_link_should_stop());
    assert(send_report_ack(idle) == 0 && ack_count == 1);
    assert(!receiver_trace_next_ack && timing_calls == timing_before_ack + 1);
    assert(transmit_probe_packet(ack_reply, 0) == 0 && timing_calls == timing_before_ack + 1);
    now += 999; assert(!receiver_link_should_stop());
    now++; assert(receiver_link_should_stop());
    assert(send_report_ack(idle) == -EACCES && ack_count == 1);
    finish_result = -EIO;
    assert(receiver_end_link() == -EIO && probe_sent && !probe_discovery_allowed());
    finish_result = 0;
    assert(receiver_end_link() == 0 && !probe_sent && ack_deadline == 0);
    assert(ack_count == 1 && receiver_discoveries == 1);
    now = 132000; /* Initial 30s arming expired; the original 60s session remains. */
    assert(probe_discovery_allowed() && send_probe() == 0);
    assert(ack_deadline == session_deadline && receiver_discoveries == 2);
    assert(receiver_trace_next_ack);
    hid_result = -EAGAIN;
    int64_t last_ack = receiver_last_ack_ms;
    now += 500;
    assert(send_report_ack(idle) == -EAGAIN && receiver_last_ack_ms == last_ack);
    assert(receiver_trace_next_ack); /* USB busy must not consume the timing sample. */
    hid_result = 0;
    while (receiver_discoveries < RECEIVER_DISCOVERY_LIMIT) {
        now += RECEIVER_LINK_GAP_MS;
        assert(receiver_link_should_stop() && receiver_end_link() == 0);
        assert(send_probe() == 0 && ack_deadline == session_deadline && ack_count == 1);
    }
    assert(send_report_ack(idle) == 0); /* Last allowed link may still carry data. */
    assert(new_link_calls == links_before + RECEIVER_DISCOVERY_LIMIT);
    now += RECEIVER_LINK_GAP_MS;
    assert(receiver_end_link() == 0 && !probe_discovery_allowed());
    assert(send_probe() == -EACCES && receiver_discoveries == RECEIVER_DISCOVERY_LIMIT);
    receiver_discoveries = 1; ack_count = PROBE_ACK_LIMIT;
    assert(!probe_discovery_allowed() && send_probe() == -EACCES);
    ack_count = 2; now = session_deadline - 1;
    probe_sent = true; ack_deadline = session_deadline; receiver_last_ack_ms = now;
    assert(!receiver_link_should_stop());
    now++;
    assert(receiver_link_should_stop() && receiver_end_link() == 0);
    assert(!probe_discovery_allowed() && send_probe() == -EACCES);
    command = 'p'; poll_probe_arm();
    assert(receiver_session_deadline == session_deadline && ack_count == 2);
    receiver_next_stats_ms = 0;
    assert(receiver_diagnostics_due());
    assert(!receiver_diagnostics_due());
    now += 999; assert(!receiver_diagnostics_due());
    now++; assert(receiver_diagnostics_due());
    assert(receiver_session_deadline == session_deadline && ack_count == 2);
#endif
    /* Idle and a subsequent data report may share a sequence,including wrap. */
    now = 500000; probe_sent = true; ack_count = 0; complete_tx = true;
    ack_deadline = now + PROBE_DATA_WINDOW_MS;
#ifdef WIRELESS_HID
    receiver_session_deadline = ack_deadline; receiver_last_ack_ms = now;
#endif
    const uint8_t idle_sequences[] = {0xff, 0};
    for (unsigned int i = 0; i < sizeof(idle_sequences); i++) {
        idle[1] = idle_sequences[i]; report[1] = idle[1];
        assert(send_report_ack(idle) == 0);
#ifdef WIRELESS_HID
        assert(ack_reply[1] == idle[1]);
        assert(ack_reply[1] != (uint8_t)(idle[1] + 1));
#else
        assert(ack_reply[1] == (uint8_t)(idle[1] + 1));
#endif
        assert(send_report_ack(report) == 0);
        assert(ack_reply[1] == (uint8_t)(report[1] + 1));
    }
    status_packet[1] = 0xff;
    assert(send_report_ack(status_packet) == 0 && ack_reply[1] == 0);
    compound[1] = 0xff;
    assert(send_report_ack(compound) == 0 && ack_reply[1] == 0);
    return 0;
}
