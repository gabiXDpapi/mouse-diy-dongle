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
    poll_probe_arm();
#endif
    int err = disable_radio();
    if (err) {
        return err;
    }
    NRF_RADIO->BASE1 = pairing_address ? 0x89abcdefu : MOUSE_BASE;
    NRF_RADIO->FREQUENCY = channel;
    start_rx();
    int64_t deadline = k_uptime_get() + 40;
    while (k_uptime_get() < deadline) {
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
                if (!probe_sent && k_uptime_get() < probe_arm_deadline &&
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
                    deadline = k_uptime_get() + 2000;
                    printk("PROBE sent once: group=0 interval=500; listening ch=%u for 2000 ms\n",
                           channel);
                    /* Receive only after logging so an ACK never targets a packet queued during USB output. */
                    start_rx();
                    continue;
                }
                if (ack_count < PROBE_ACK_LIMIT && k_uptime_get() < ack_deadline &&
                    is_initial_report(packet, channel, pairing_address, NRF_RADIO->RXMATCH)) {
                    err = send_report_ack(packet);
                    if (err) {
                        printk("ERROR data ACK transmit %d; no retry\n", err);
                        return err;
                    }
                    printk("PROBE data ACK sent: count=%u/%u seq=%u next=%u\n",
                           ack_count, PROBE_ACK_LIMIT, packet[1], ack_reply[1]);
                }
#endif
                printk("RX t=%lld ch=%u base=%s match=%u len=%u data=",
                       k_uptime_get(), channel, pairing_address ? "pair" : "mouse",
                       NRF_RADIO->RXMATCH, length);
                for (uint8_t i = 0; i < length; i++) {
                    printk("%02x", packet[i + 1]);
                }
                printk("\n");
            } else {
                bad++;
            }
            start_rx();
        }
        /* shortcut: polling logger misses traffic; use an IRQ ring buffer for throughput work. */
        k_busy_wait(50);
        k_yield();
    }
    return disable_radio();
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
    err = usb_enable(NULL);
    if (err) {
        return err;
    }
    uint32_t dtr = 0;
    do {
        err = uart_line_ctrl_get(console, UART_LINE_CTRL_DTR, &dtr);
        if (err) {
            return err;
        }
        k_sleep(K_MSEC(100));
    } while (!dtr);
#ifdef DISCOVERY_PROBE
    probe_self_test();
    printk("DIY Mouse Discovery Probe v6: compound report filter; passive until 'p'; ONE discovery + 16 ACKs maximum in 2000 ms; no HID\n");
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
    while (true) {
        for (size_t i = 0; i < ARRAY_SIZE(channels); i++) {
            err = listen(channels[i], pairing_address);
            if (err) {
                printk("ERROR radio disable %d\n", err);
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
