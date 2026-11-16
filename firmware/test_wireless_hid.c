#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#define assert(c) do { if (!(c)) { \
    fprintf(stderr, "Assertion failed: %s at line %d\n", #c, __LINE__); exit(1); \
} } while (0)
#define BUILD_ASSERT(c) _Static_assert(c, #c)
#define ARG_UNUSED(x) ((void)(x))
#define K_MSEC(ms) (ms)
#define K_SEM_DEFINE(name, initial, limit) struct k_sem name = {initial}
#define UDC_STATIC_BUF_DEFINE(name, size) static uint8_t name[((size + 3) / 4) * 4]
typedef int atomic_t;
static int atomic_get(const atomic_t *v) { return *v; }
static void atomic_set(atomic_t *v, int n) { *v = n; }
static void atomic_clear(atomic_t *v) { *v = 0; }
struct k_sem { int count; };
struct device { int unused; };
struct hid_ops { void (*int_in_ready)(const struct device *); };
enum usb_dc_status_code { USB_DC_CONFIGURED, USB_DC_RESET, USB_DC_DISCONNECTED, USB_DC_ERROR };
static struct device fake_device;
static const struct hid_ops *registered_ops;
static bool device_available = true, device_ready = true, auto_complete;
static int init_result, write_result;
static bool short_write;
static unsigned int writes;
static unsigned int reboots;
#define SYS_REBOOT_COLD 0
static void sys_reboot(int mode) { assert(mode == SYS_REBOOT_COLD); reboots++; }
static uint8_t last_report[6];
static int k_sem_take(struct k_sem *s, int timeout)
{
    if (!s->count && timeout > 0 && auto_complete) s->count = 1;
    if (!s->count) return -EAGAIN;
    s->count--;
    return 0;
}
static void k_sem_give(struct k_sem *s) { s->count = 1; }
static const struct device *device_get_binding(const char *name)
{
    assert(strcmp(name, "HID_0") == 0);
    return device_available ? &fake_device : NULL;
}
static bool device_is_ready(const struct device *dev) { return dev && device_ready; }
static void usb_hid_register_device(const struct device *dev, const uint8_t *desc,
                                    size_t size, const struct hid_ops *ops)
{
    assert(dev && desc && size > 0);
    registered_ops = ops;
}
static int usb_hid_init(const struct device *dev) { assert(dev); return init_result; }
static int hid_int_ep_write(const struct device *dev, const uint8_t *data,
                            uint32_t size, uint32_t *written)
{
    assert(dev && size == sizeof(last_report));
    if (write_result) return write_result;
    memcpy(last_report, data, size);
    *written = short_write ? size - 1 : size;
    writes++;
    if (auto_complete) registered_ops->int_in_ready(dev);
    return 0;
}
#ifdef LATENCY_DIAGNOSTICS
static uint32_t fake_us;
static unsigned int irq_lock(void) { return 0; }
static void irq_unlock(unsigned int key) { assert(key == 0); }
static uint32_t latency_clock_us(void) { return fake_us; }
#include "radio_logger/src/latency_metrics.h"
#endif
#include "radio_logger/src/wireless_hid.h"

int main(void)
{
    uint8_t p[21] = {11, 0, 0, 0x41, 0xff, 0xff, 0, 0x80, 0xff, 0x7f, 0x55, 0xaa};
    uint8_t idle[] = {2, 1, 0};
    uint8_t status_packet[] = {11, 0, 0, 0x45, 6, 1, 0, 0, 0, 0, 0, 0};
    const uint8_t expected[] = {0x1f, 0, 0x80, 0xff, 0x7f, 0xff};
    device_available = false; assert(init_wireless_hid() == -ENODEV);
    device_available = true; device_ready = false; assert(init_wireless_hid() == -ENODEV);
    device_ready = true; init_result = -EIO; assert(init_wireless_hid() == -EIO);
    init_result = 0; assert(init_wireless_hid() == 0);
    assert(wireless_hid_forward(p) == -ENODEV && writes == 0);
    assert(!wireless_hid_new_session());
    wireless_hid_status(USB_DC_RESET, NULL); /* Initial enumeration reset is allowed. */
    wireless_hid_restart_if_needed(); assert(reboots == 0);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    assert(wireless_hid_new_session());
    assert(wireless_hid_forward(idle) == 0 && writes == 0);
    assert(wireless_hid_forward(status_packet) == 0 && writes == 0);
    assert(!wireless_hid_has_sequence);
    assert(wireless_hid_forward(p) == 0 && writes == 1);
    assert(wireless_hid_reports == 1 && wireless_hid_retries == 0);
    assert(wireless_hid_busy == 0);
    assert(memcmp(last_report, expected, sizeof(expected)) == 0);
    assert(wireless_hid_forward(p) == 0 && writes == 1); /* Retransmission. */
    assert(wireless_hid_reports == 1 && wireless_hid_retries == 1);
    assert(wireless_hid_forward(idle) == 0 && writes == 1);
    status_packet[1] = 1;
    assert(wireless_hid_forward(status_packet) == 0 && writes == 1);
    assert(wireless_hid_last_sequence == 0);
    p[1] = 1;
    assert(wireless_hid_forward(p) == -EAGAIN && writes == 1);
    assert(wireless_hid_busy == 1);
    assert(wireless_hid_reports == 1); /* Busy did not submit or count a report. */
    assert(wireless_hid_last_sequence == 0); /* Busy never consumes the next sequence. */
    registered_ops->int_in_ready(&fake_device);
    assert(wireless_hid_forward(p) == 0 && writes == 2); /* Idle seq1 did not consume data seq1. */
    registered_ops->int_in_ready(&fake_device);
    write_result = -EAGAIN; p[1] = 2;
    assert(wireless_hid_forward(p) == -EAGAIN && writes == 2);
    assert(wireless_hid_busy == 2);
    assert(wireless_hid_last_sequence == 1);
    write_result = -EIO;
    unsigned int reports_before_io_error = wireless_hid_reports;
    assert(wireless_hid_forward(p) == -EIO && writes == 2);
    assert(wireless_hid_busy == 2);
    assert(wireless_hid_reports == reports_before_io_error);
    write_result = 0;
    assert(wireless_hid_forward(p) == 0 && writes == 3);
    assert(wireless_hid_finish() == -ETIMEDOUT); /* Does not overwrite an in-flight report. */
    assert(memcmp(last_report, expected, sizeof(expected)) == 0);
    unsigned int reports_before_neutral = wireless_hid_reports;
    auto_complete = true;
    assert(wireless_hid_finish() == 0 && writes == 4);
    assert(wireless_hid_busy == 2); /* Neutral submission is not input-busy telemetry. */
    assert(wireless_hid_reports == reports_before_neutral); /* Neutral is not new mouse input. */
    const uint8_t released[6] = {0};
    assert(memcmp(last_report, released, sizeof(released)) == 0);
    assert(wireless_hid_finish() == 0 && writes == 4);
    p[1] = 0xff; assert(wireless_hid_forward(p) == 0 && writes == 5);
    p[1] = 0; assert(wireless_hid_forward(p) == 0 && writes == 6); /* Sequence wrap. */
    p[0] = 20; p[12] = 0x45; p[1] = 1;
    assert(wireless_hid_forward(p) == 0 && writes == 7);
    p[1] = 2; p[12] = 0x41; assert(wireless_hid_forward(p) == -EINVAL && writes == 7);
    p[0] = 11; p[3] = 0x46; assert(wireless_hid_forward(p) == -EINVAL && writes == 7);
    p[0] = 3; assert(wireless_hid_forward(p) == -EINVAL && writes == 7);
    p[0] = 11; p[3] = 0x41; short_write = true;
    unsigned int reports_before_error = wireless_hid_reports;
    assert(wireless_hid_forward(p) == -EIO && writes == 8);
    assert(wireless_hid_busy == 2);
    assert(wireless_hid_reports == reports_before_error);
    assert(wireless_hid_last_sequence == 1);
    short_write = false; assert(wireless_hid_finish() == 0 && writes == 9);
    p[1] = 3; assert(wireless_hid_forward(p) == 0 && writes == 10);
    /* A status-only gap can wrap the transport sequence back to the last report. */
    for (unsigned int seq = 4; seq <= 259; seq++) {
        status_packet[1] = (uint8_t)seq;
        assert(wireless_hid_forward(status_packet) == 0 && writes == 10);
    }
    assert(wireless_hid_forward(p) == 0 && writes == 11);
    assert(wireless_hid_finish() == 0 && writes == 12);
    unsigned int reports_before_link = wireless_hid_reports;
    wireless_hid_new_link();
    assert(wireless_hid_reports == reports_before_link); /* Cumulative across links. */
    assert(wireless_hid_forward(p) == 0 && writes == 13); /* Restarted link may reuse sequence. */
    assert(wireless_hid_reports == reports_before_link + 1);
    assert(wireless_hid_forward(p) == 0 && writes == 13); /* Retry still suppressed. */
    wireless_hid_status(USB_DC_DISCONNECTED, NULL);
    assert(!wireless_hid_new_session());
    assert(reboots == 0); /* USB callback only requests restart. */
    assert(wireless_hid_finish() == -ENODEV);
    assert(wireless_hid_forward(idle) == -ENODEV);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    assert(wireless_hid_forward(p) == -ENODEV && writes == 13); /* Reboot required. */
    wireless_hid_restart_if_needed();
    assert(reboots == 1 && writes == 13);
    /* Simulate fresh startup to exercise reset versus generic controller error. */
    atomic_clear(&wireless_hid_online); atomic_clear(&wireless_hid_failed);
    atomic_clear(&wireless_hid_restart);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    wireless_hid_status(USB_DC_ERROR, NULL);
    assert(!wireless_hid_new_session());
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    wireless_hid_restart_if_needed();
    assert(reboots == 1 && wireless_hid_forward(p) == -ENODEV && writes == 13);
    atomic_clear(&wireless_hid_failed);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    wireless_hid_status(USB_DC_RESET, NULL);
    assert(reboots == 1 && atomic_get(&wireless_hid_restart));
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    assert(wireless_hid_forward(p) == -ENODEV); /* No output before restart/rearm. */
    wireless_hid_restart_if_needed();
    assert(reboots == 2 && writes == 13);
    atomic_clear(&wireless_hid_online); atomic_clear(&wireless_hid_failed);
    atomic_clear(&wireless_hid_restart);
    wireless_hid_status(USB_DC_CONFIGURED, NULL);
    assert(wireless_hid_reports != 0 && wireless_hid_has_sequence);
    assert(wireless_hid_new_session());
    assert(!wireless_hid_has_sequence && wireless_hid_reports == 0 &&
           wireless_hid_retries == 0 && wireless_hid_busy == 0);
#ifdef LATENCY_DIAGNOSTICS
    fake_us = 100; latency_observe_rx(50); auto_complete = true; p[1]++;
    assert(wireless_hid_forward(p) == 0);
    assert(latency_metrics[LAT_RF_USB].count == 1 && !latency_pending.active);
    fake_us = 200; latency_observe_rx(150); auto_complete = false; p[1]++;
    assert(wireless_hid_forward(p) == 0 && latency_pending.active);
    assert(!wireless_hid_new_session());
    fake_us = 1200; registered_ops->int_in_ready(&fake_device);
    assert(latency_metrics[LAT_SUBMIT_USB].maximum == 1000 && !latency_pending.active);
    write_result = -EIO; fake_us = 1300; latency_observe_rx(1250); p[1]++;
    assert(wireless_hid_forward(p) == -EIO && !latency_pending.active);
    assert(latency_failed_submissions == 1 && latency_metrics[LAT_RF_USB].count == 2);
    write_result = 0; short_write = true; auto_complete = true;
    fake_us = 1400; latency_observe_rx(1350);
    assert(wireless_hid_forward(p) == -EIO && !latency_pending.active);
    assert(latency_failed_submissions == 2 && latency_metrics[LAT_RF_USB].count == 2);
    short_write = false;
    unsigned int neutral_count = latency_metrics[LAT_RF_USB].count;
    assert(wireless_hid_finish() == 0 && latency_metrics[LAT_RF_USB].count == neutral_count);
    latency_ack(100, 150, 200); latency_stop();
    assert(latency_dump_ready() && !latency_tracking_fault);
#endif
    return 0;
}
