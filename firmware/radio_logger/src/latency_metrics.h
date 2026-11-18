#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define LATENCY_BINS 9u
#define LATENCY_COUNT_CAP 1000000u
enum latency_metric { LAT_CANDIDATE, LAT_RF_SUBMIT, LAT_SUBMIT_USB, LAT_RF_USB,
                      LAT_ACK_READY, LAT_ACK_END, LAT_METRIC_COUNT };
struct latency_histogram {
    uint32_t count, maximum, bins[LATENCY_BINS];
    uint64_t sum;
};
static struct latency_histogram latency_metrics[LAT_METRIC_COUNT];
static bool latency_capped;
static void latency_add(enum latency_metric metric, uint32_t us)
{
    static const uint32_t upper[] = {50,100,250,500,1000,2000,4000,8000};
    struct latency_histogram *h = &latency_metrics[metric];
    if (h->count == LATENCY_COUNT_CAP) {
        latency_capped = true;
        return;
    }
    unsigned int bin = 0;
    while (bin < LATENCY_BINS - 1 && us > upper[bin]) bin++;
    h->count++;
    h->sum += us;
    if (us > h->maximum) h->maximum = us;
    h->bins[bin]++;
}

static bool latency_collect, latency_rx_valid, latency_tracking_fault;
static uint32_t latency_rx_us, latency_failed_submissions;
static struct {
    bool active, accepted, completed;
    uint32_t rx_us, submit_us, complete_us;
} latency_pending;

static bool latency_reset(void)
{
    unsigned int key = irq_lock();
    if (latency_pending.active) { irq_unlock(key); return false; }
    memset(latency_metrics, 0, sizeof(latency_metrics));
#ifdef INPUT_CONSERVATION
    input_totals_reset();
#endif
    latency_capped = latency_rx_valid = latency_tracking_fault = false;
    latency_failed_submissions = 0;
    latency_collect = true;
    irq_unlock(key);
    return true;
}

static void latency_observe_rx(uint32_t rx_us)
{
    unsigned int key = irq_lock();
    if (latency_collect) {
        latency_rx_us = rx_us;
        latency_rx_valid = true;
        latency_add(LAT_CANDIDATE, latency_clock_us() - rx_us);
    }
    irq_unlock(key);
}

static bool latency_submit_begin(void)
{
    unsigned int key = irq_lock();
    if (!latency_collect || !latency_rx_valid) { irq_unlock(key); return false; }
    if (latency_pending.active) {
        latency_tracking_fault = true;
        latency_collect = latency_rx_valid = latency_pending.active = false;
        irq_unlock(key);
        return false;
    }
    latency_pending.rx_us = latency_rx_us;
    latency_pending.submit_us = latency_clock_us();
    latency_pending.accepted = latency_pending.completed = false;
    latency_pending.active = true;
    irq_unlock(key);
    return true;
}

/* Caller owns the short IRQ lock; USB completion may precede write return. */
static void latency_finish_pending(void)
{
    latency_add(LAT_SUBMIT_USB, latency_pending.complete_us - latency_pending.submit_us);
    latency_add(LAT_RF_USB, latency_pending.complete_us - latency_pending.rx_us);
    latency_pending.active = false;
}

static void latency_submit_result(bool measured, bool accepted)
{
    if (!measured) return;
    unsigned int key = irq_lock();
    if (latency_pending.active) {
        if (!accepted) {
            if (latency_failed_submissions != UINT32_MAX) latency_failed_submissions++;
            latency_pending.active = false;
        } else {
            latency_pending.accepted = true;
            latency_add(LAT_RF_SUBMIT, latency_pending.submit_us - latency_pending.rx_us);
            if (latency_pending.completed) latency_finish_pending();
        }
    }
    irq_unlock(key);
}

static void latency_usb_complete(void)
{
    unsigned int key = irq_lock();
    if (latency_pending.active && !latency_pending.completed) {
        latency_pending.complete_us = latency_clock_us();
        latency_pending.completed = true;
        if (latency_pending.accepted) latency_finish_pending();
    }
    irq_unlock(key);
}

static void latency_ack(uint32_t rx_us, uint32_t ready_us, uint32_t end_us)
{
    unsigned int key = irq_lock();
    if (latency_collect) {
        latency_add(LAT_ACK_READY, ready_us - rx_us);
        latency_add(LAT_ACK_END, end_us - rx_us);
    }
    irq_unlock(key);
}

static void latency_stop(void)
{
    unsigned int key = irq_lock();
    latency_collect = false;
    irq_unlock(key);
}

static bool latency_dump_ready(void)
{
    unsigned int key = irq_lock();
    bool ready = !latency_collect && !latency_pending.active;
    irq_unlock(key);
    return ready;
}
