#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/nrf_clock_control.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/onoff.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/printk.h>
#include <zephyr/usb/usb_device.h>
#include <nrfx.h>
#include "radio_parameters.h"
#ifdef LATENCY_DIAGNOSTICS
static uint32_t latency_clock_us(void)
{
    NRF_TIMER2->TASKS_CAPTURE[3] = 1;
    return NRF_TIMER2->CC[3];
}
#include "latency_metrics.h"
#include "latency_dump.h"
#endif
#ifdef WIRELESS_HID
#include <zephyr/drivers/usb/udc_buf.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/usb/class/usb_hid.h>
#include "wireless_hid.h"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static uint8_t packet[65] __aligned(4);
static uint32_t good, bad, invalid;
static struct onoff_client hf_client;

BUILD_ASSERT(ARRAY_SIZE(channels) == 19);
BUILD_ASSERT((OBSERVED_PCNF1 & 0xff) == sizeof(packet) - 1);
BUILD_ASSERT(!IS_ENABLED(CONFIG_BT) && !IS_ENABLED(CONFIG_IEEE802154));

static int disable_radio(void)
{
    NRF_RADIO->SHORTS = 0;
    if (NRF_RADIO->STATE == RADIO_STATE_STATE_Disabled) {
        return 0;
    }
    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->TASKS_DISABLE = 1;
    for (int i = 0; i < 1000; i++) {
        if (NRF_RADIO->EVENTS_DISABLED) {
            return 0;
        }
        k_busy_wait(10);
    }
    return -ETIMEDOUT;
}

static void start_rx(void)
{
    memset(packet, 0, sizeof(packet));
    NRF_RADIO->EVENTS_END = 0;
    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->SHORTS = RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;
    __DMB();
    NRF_RADIO->TASKS_RXEN = 1;
}

#ifdef DISCOVERY_PROBE
#include "probe_timing.h"
#include "discovery_probe.h"
#endif

static int listen(uint8_t channel, bool pairing_address)
{
#ifdef DISCOVERY_PROBE
#ifdef WIRELESS_HID
    receiver_poll_commands();
#else
    poll_probe_arm();
#endif
#endif
    int err = disable_radio();
    if (err) {
        return err;
    }
    NRF_RADIO->BASE1 = pairing_address ? 0x89abcdefu : MOUSE_BASE;
    NRF_RADIO->FREQUENCY = channel;
    start_rx();
    int64_t deadline = k_uptime_get() + 40;
    while (
#ifdef NORMAL_DONGLE
           probe_sent ||
#endif
           k_uptime_get() < deadline) {
#ifdef WIRELESS_HID
        receiver_poll_commands();
        if (receiver_disarm_requested && !receiver_disarm_handled) {
            return receiver_handle_disarm();
        }
        if (!atomic_get(&wireless_hid_online)) {
            disable_radio();
            return -ENODEV;
        }
        if (receiver_link_should_stop()) {
            err = disable_radio();
            return err ? err : receiver_end_link();
        }
#endif
        if (NRF_RADIO->EVENTS_END) {
            err = disable_radio();
            if (err) {
                return err;
            }
            __DMB();
            uint8_t length = packet[0];
            if (length > sizeof(packet) - 1) {
                invalid++;
            } else if (NRF_RADIO->CRCSTATUS) {
                good++;
#ifdef DISCOVERY_PROBE
                if (probe_discovery_allowed() &&
                    is_discovery(packet, channel, pairing_address,
                                 NRF_RADIO->RXMATCH)) {
                    err = send_probe();
                    if (err) {
                        printk("ERROR probe transmit %d; no retry\n", err);
                        return err;
                    }
                    /* shortcut: stay on observed channel51; trace hopping before a sustained receiver. */
                    channel = channels[3];
                    NRF_RADIO->FREQUENCY = channel;
#ifdef WIRELESS_HID
                    deadline = ack_deadline;
                    printk("RECEIVER link started: discovery=%u/%u ACKs=%u/%u session_remaining_ms=%lld\n",
                           receiver_discoveries, RECEIVER_DISCOVERY_LIMIT, ack_count,
                           PROBE_ACK_LIMIT, receiver_session_deadline - k_uptime_get());
#else
                    deadline = k_uptime_get() + PROBE_DATA_WINDOW_MS;
                    printk("PROBE sent once: group=0 interval=500; listening ch=%u for %u ms\n",
                           channel, PROBE_DATA_WINDOW_MS);
#endif
                    /* Receive only after logging so an ACK never targets a packet queued during USB output. */
                    start_rx();
                    continue;
                }
                if (probe_ack_budget_remaining() && probe_ack_window_open() &&
                    is_ack_candidate(packet, channel, pairing_address, NRF_RADIO->RXMATCH)) {
                    err = send_report_ack(packet);
#ifdef WIRELESS_HID
                    if (err == -EAGAIN) {
                        /* Count backpressure without USB text in active RF processing. */
                        start_rx();
                        continue;
                    }
                    if (err == -EACCES && receiver_link_should_stop()) {
                        err = disable_radio();
                        return err ? err : receiver_end_link();
                    }
#endif
                    if (err) {
                        printk("ERROR data ACK transmit %d; no retry\n", err);
                        return err;
                    }
#ifndef WIRELESS_HID
                    printk("PROBE ACK sent: count=%u/%u len=%u seq=%u next=%u\n",
                           ack_count, PROBE_ACK_LIMIT, packet[0], packet[1], ack_reply[1]);
#endif
                }
#endif
#ifdef WIRELESS_HID
                if (receiver_diagnostics_due()) {
                    printk("RECEIVER stats t=%lld good=%u bad_crc=%u invalid_len=%u ACKs=%u/%u idle=%u status=%u type1=%u hid_reports=%u retries=%u busy=%u\n",
                           k_uptime_get(), good, bad, invalid, ack_count, PROBE_ACK_LIMIT,
                           receiver_idle_acks, receiver_status_acks, receiver_type1_acks,
                           wireless_hid_reports, wireless_hid_retries, wireless_hid_busy);
                }
#else
                printk("RX t=%lld ch=%u base=%s match=%u len=%u data=",
                       k_uptime_get(), channel, pairing_address ? "pair" : "mouse",
                       NRF_RADIO->RXMATCH, length);
                for (uint8_t i = 0; i < length; i++) {
                    printk("%02x", packet[i + 1]);
                }
                printk("\n");
#endif
            } else {
                bad++;
            }
            start_rx();
        }
        /* shortcut: polling logger misses traffic; use an IRQ ring buffer for throughput work. */
        k_busy_wait(50);
        k_yield();
    }
    err = disable_radio();
#ifdef WIRELESS_HID
    if (!err && receiver_link_should_stop()) {
        return receiver_end_link();
    }
#endif
    return err;
}

int main(void)
{
    const struct device *console = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    const struct device *clock = DEVICE_DT_GET(DT_NODELABEL(clock));
    if (!device_is_ready(console) || !device_is_ready(clock) || !gpio_is_ready_dt(&led)) {
        return -ENODEV;
    }
    int err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (err) {
        return err;
    }
#ifdef WIRELESS_HID
    err = init_wireless_hid();
    if (err) {
        return err;
    }
    err = usb_enable(wireless_hid_status);
#else
    err = usb_enable(NULL);
#endif
    if (err) {
        return err;
    }
#ifdef NORMAL_DONGLE
    while (!atomic_get(&wireless_hid_online)) {
        wireless_hid_restart_if_needed();
        if (atomic_get(&wireless_hid_failed)) {
            return -ENODEV;
        }
        k_sleep(K_MSEC(100));
    }
#else
    uint32_t dtr = 0;
    do {
#ifdef WIRELESS_HID
        wireless_hid_restart_if_needed();
#endif
        err = uart_line_ctrl_get(console, UART_LINE_CTRL_DTR, &dtr);
        if (err) {
            return err;
        }
        k_sleep(K_MSEC(100));
    } while (!dtr);
#endif
#ifdef DISCOVERY_PROBE
    probe_self_test();
#ifdef WIRELESS_HID
    printk("DIY Mouse Wireless HID Probe v14: explicit rearm; bounded fifteen minutes; passive USB restart; explicit x stop; quiet active; idle sequence hold; passive until 'p'; %u discoveries + %u ACKs total in %u ms; release/recover after %u ms gap\n",
           RECEIVER_DISCOVERY_LIMIT, PROBE_ACK_LIMIT, PROBE_DATA_WINDOW_MS, RECEIVER_LINK_GAP_MS);
#else
    printk("DIY Mouse Discovery Probe v9: idle/status/report ACKs; passive until 'p'; ONE discovery + %u ACKs maximum in %u ms; no HID\n",
           PROBE_ACK_LIMIT, PROBE_DATA_WINDOW_MS);
#endif
#else
    printk("DIY Mouse RX Logger v2: receive-only, no movement reports\n");
#endif
    /* Share the HFXO manager with the USB driver rather than mixing clock APIs. */
    struct onoff_manager *hf = z_nrf_clock_control_get_onoff(CLOCK_CONTROL_NRF_SUBSYS_HF);
    if (!hf) {
        printk("ERROR no HF clock manager\n");
        return -ENODEV;
    }
    sys_notify_init_spinwait(&hf_client.notify);
    err = onoff_request(hf, &hf_client);
    if (err < 0) {
        printk("ERROR HF clock request %d\n", err);
        return err;
    }
    int64_t deadline = k_uptime_get() + 1000;
    int clock_result = -EIO;
    while (sys_notify_fetch_result(&hf_client.notify, &clock_result) == -EAGAIN) {
        if (k_uptime_get() >= deadline) {
            printk("ERROR HF clock timeout\n");
            onoff_cancel_or_release(hf, &hf_client);
            return -ETIMEDOUT;
        }
        k_sleep(K_MSEC(1));
    }
    if (clock_result < 0) {
        printk("ERROR HF clock start %d\n", clock_result);
        return clock_result;
    }
    NRF_RADIO->POWER = 1;
    NRF_RADIO->INTENCLR = 0xffffffffu;
    err = disable_radio();
    if (err) {
        printk("ERROR radio disable %d\n", err);
        return err;
    }
    /* Exact setup values at stock 0x15a50..0x15ae2; address mapping still needs RF validation. */
    NRF_RADIO->MODE = 4;
    NRF_RADIO->PCNF0 = 8;
    NRF_RADIO->PCNF1 = OBSERVED_PCNF1;
    NRF_RADIO->BASE0 = OBSERVED_BASE0;
    NRF_RADIO->PREFIX0 = OBSERVED_PREFIX0;
    NRF_RADIO->RXADDRESSES = 3;
    NRF_RADIO->CRCCNF = 2;
    NRF_RADIO->CRCPOLY = OBSERVED_CRCPOLY;
    NRF_RADIO->CRCINIT = 0xffff;
    NRF_RADIO->DATAWHITEIV = 0x40;
    NRF_RADIO->PACKETPTR = (uint32_t)packet;
#ifdef DISCOVERY_PROBE
    err = init_probe_timing();
    if (err) {
        printk("ERROR probe timing initialization %d\n", err);
        return err;
    }
    printk("TIMING hardware capture initialized at 1 MHz\n");
#endif
    __ASSERT(NRF_RADIO->PCNF1 == OBSERVED_PCNF1, "Radio setup readback failed");
    printk("mode=%u pcnf0=%08x pcnf1=%08x crc=%08x channels=%u dwell_ms=40\n",
           NRF_RADIO->MODE, NRF_RADIO->PCNF0, NRF_RADIO->PCNF1, NRF_RADIO->CRCPOLY,
           (unsigned int)ARRAY_SIZE(channels));
    bool pairing_address = false;
#ifdef NORMAL_DONGLE
    if (!receiver_start_session()) {
        wireless_hid_restart_if_needed();
        return -ENODEV;
    }
#endif
    while (true) {
        for (size_t i = 0; i < ARRAY_SIZE(channels); i++) {
            err = listen(channels[i], pairing_address);
#ifdef WIRELESS_HID
            int release_err = wireless_hid_finish();
            if (release_err) {
                printk("ERROR HID release %d\n", release_err);
                if (!err) {
                    err = release_err;
                }
            }
#endif
            if (err) {
#ifdef WIRELESS_HID
                wireless_hid_restart_if_needed();
#endif
                printk("ERROR receive loop %d\n", err);
                return err;
            }
        }
        printk("SCAN base=%s good=%u bad_crc=%u invalid_len=%u\n",
               pairing_address ? "pair" : "mouse", good, bad, invalid);
        gpio_pin_toggle_dt(&led);
        pairing_address = !pairing_address;
        k_sleep(K_MSEC(1));
    }
}
