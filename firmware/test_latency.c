#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#ifndef ENOTCONN
#define ENOTCONN 107
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#ifndef EOVERFLOW
#define EOVERFLOW 75
#endif
#define assert(c) do { if (!(c)) { fprintf(stderr,"Assertion failed: %s line%d\n",#c,__LINE__); exit(1); } } while (0)
static uint32_t fake_us;
static unsigned int locked;
static unsigned int irq_lock(void) { assert(!locked); locked = 1; return 0; }
static void irq_unlock(unsigned int key) { assert(key == 0 && locked); locked = 0; }
static uint32_t latency_clock_us(void) { assert(locked); return fake_us; }
#ifdef INPUT_CONSERVATION
#include "radio_logger/src/input_totals.h"
#endif
#include "radio_logger/src/latency_metrics.h"
struct device { int unused; };
static char output[4096];
static size_t output_size;
static unsigned int fake_dtr = 1;
static int64_t fake_ms;
static bool fifo_stalled, drop_dtr;
#define UART_LINE_CTRL_DTR 1
#define K_MSEC(ms) (ms)
#define snprintk snprintf
static int64_t k_uptime_get(void) { return fake_ms; }
static void k_sleep(int ms) { fake_ms += ms; }
static int uart_line_ctrl_get(const struct device *d, int control, uint32_t *value)
{ (void)d; assert(control == UART_LINE_CTRL_DTR); *value = fake_dtr; return 0; }
static int uart_fifo_fill(const struct device *d, const uint8_t *text, int size)
{
    (void)d;
    if (fifo_stalled) return 0;
    int n = size < 23 ? size : 23;
    assert(output_size + n < sizeof(output));
    memcpy(output + output_size, text, n); output_size += n; output[output_size] = 0;
    if (drop_dtr) fake_dtr = 0;
    return n;
}
#include "radio_logger/src/latency_dump.h"
int main(void)
{
    latency_add(LAT_CANDIDATE, 50);
    assert(latency_metrics[LAT_CANDIDATE].count == 1);
    assert(latency_metrics[LAT_CANDIDATE].bins[0] == 1);
    latency_add(LAT_CANDIDATE, 51);
    latency_add(LAT_CANDIDATE, 8001);
    assert(latency_metrics[LAT_CANDIDATE].bins[1] == 1);
    assert(latency_metrics[LAT_CANDIDATE].bins[8] == 1);
    assert(latency_metrics[LAT_CANDIDATE].sum == 8102 && latency_metrics[LAT_CANDIDATE].maximum == 8001);
    latency_metrics[LAT_CANDIDATE].count = LATENCY_COUNT_CAP;
    latency_add(LAT_CANDIDATE, 9000);
    assert(latency_capped && latency_metrics[LAT_CANDIDATE].count == LATENCY_COUNT_CAP);
    assert(latency_reset() && !latency_capped && !latency_dump_ready());
    assert(!latency_submit_begin()); /* No RF timestamp: no synthetic measurement. */
    fake_us = 80; latency_observe_rx(10);
    fake_us = 100; assert(latency_submit_begin());
    assert(!latency_reset()); /* Cannot overwrite an in-flight sample. */
    latency_submit_result(true, true);
    fake_us = 1100; latency_usb_complete();
    assert(latency_metrics[LAT_RF_SUBMIT].maximum == 90);
    assert(latency_metrics[LAT_SUBMIT_USB].maximum == 1000);
    assert(latency_metrics[LAT_RF_USB].maximum == 1090);
    latency_usb_complete();
    assert(latency_metrics[LAT_RF_USB].count == 1);
    assert(latency_reset());
    fake_us = 0xfffffff0u; latency_observe_rx(0xffffff00u);
    fake_us = 0xfffffff5u; assert(latency_submit_begin());
    fake_us = 20; latency_usb_complete(); /* Callback inside write, across wrap. */
    assert(latency_metrics[LAT_RF_USB].count == 0);
    latency_submit_result(true, true);
    assert(latency_metrics[LAT_SUBMIT_USB].maximum == 31);
    assert(latency_metrics[LAT_RF_USB].maximum == 276);
    latency_observe_rx(20); assert(latency_submit_begin());
    fake_us = 40; latency_usb_complete();
    latency_submit_result(true, false); /* Short/failed write with early callback. */
    assert(latency_failed_submissions == 1 && latency_metrics[LAT_RF_USB].count == 1);
    latency_submit_result(false, false);
    assert(latency_failed_submissions == 1);
    latency_ack(0xfffffff0u, 10, 20);
    assert(latency_metrics[LAT_ACK_READY].maximum == 26 && latency_metrics[LAT_ACK_END].maximum == 36);
    latency_observe_rx(40); assert(latency_submit_begin());
    latency_submit_result(true, true); latency_stop();
    assert(!latency_dump_ready());
    fake_us = 50; latency_usb_complete();
    assert(latency_dump_ready()); /* Pending completion drains after stop. */
    unsigned int count = latency_metrics[LAT_CANDIDATE].count;
    latency_observe_rx(50); latency_ack(50, 60, 70);
    assert(latency_metrics[LAT_CANDIDATE].count == count);
    assert(latency_reset()); latency_observe_rx(50); assert(latency_submit_begin());
    assert(!latency_submit_begin() && latency_tracking_fault); /* Corrupt ownership is flagged. */
    assert(latency_dump(NULL, 7, 2, 3) == 0);
    assert(strstr(output,"LATENCY_V1 cap=1000000") && strstr(output,"LATENCY_END failed=0 capped=0 tracking_fault=1"));
    assert(latency_reset()); output_size = 0; output[0] = 0;
    assert(latency_dump(NULL, 7, 2, 3) == -EAGAIN && output_size == 0);
    latency_observe_rx(50); assert(latency_submit_begin()); latency_stop();
    assert(latency_dump(NULL, 7, 2, 3) == -EAGAIN && output_size == 0);
    latency_submit_result(true, true); latency_usb_complete();
    fake_dtr = 0;
    assert(latency_dump(NULL, 7, 2, 3) == -ENOTCONN && output_size == 0);
    fake_dtr = 1; fifo_stalled = true;
    assert(latency_dump(NULL, 7, 2, 3) == -ETIMEDOUT && fake_ms == 1500);
    fifo_stalled = false; drop_dtr = true;
    assert(latency_dump(NULL, 7, 2, 3) == -ENOTCONN && !strstr(output,"LATENCY_END"));
    drop_dtr = false; fake_dtr = 1; output_size = 0; output[0] = 0;
    assert(latency_dump(NULL, 7, 2, 3) == 0 && strstr(output,"LATENCY metric=rf_usb count=1"));
    assert(strstr(output,"reports=7 retries=2 busy=3"));
#ifdef INPUT_CONSERVATION
    input_totals_reset();
    const uint8_t input[] = {1,0xfe,0xff,3,0,0xff};
    const uint8_t held[] = {1,0,0,0,0,0};
    input_totals_record(input); input_totals_record(input); input_totals_record(held);
    input_totals_neutral();
    output_size = 0; output[0] = 0;
    assert(latency_dump(NULL, 3, 0, 0) == 0);
    assert(strstr(output,"INPUT_V1 reports=3 unchanged=1 buttons=0 neutral=1"));
    assert(strstr(output,"dx=fffffffffffffffc dy=0000000000000006 wheel=fffffffffffffffe"));
    assert(strstr(output,"neutral_up=1,0,0,0,0") && strstr(output,"INPUT_END capped=0"));
#endif
    assert(!locked);
    return 0;
}
