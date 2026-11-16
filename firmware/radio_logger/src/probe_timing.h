#include <nrfx_ppi.h>

BUILD_ASSERT(!DT_NODE_HAS_STATUS(DT_NODELABEL(timer2), okay));
BUILD_ASSERT(!IS_ENABLED(CONFIG_NRFX_TIMER2));
static uint32_t probe_rx_end_us, probe_software_age_us;

static int init_probe_timing(void)
{
    nrf_ppi_channel_t allocated[2];
    unsigned int count = 0;
    NRF_TIMER2->TASKS_STOP = 1;
    NRF_TIMER2->INTENCLR = 0xffffffffu;
    NRF_TIMER2->SHORTS = 0;
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    NRF_TIMER2->PRESCALER = 4;
    NRF_TIMER2->TASKS_CLEAR = 1;
    NRF_TIMER2->TASKS_START = 1;
    for (unsigned int i = 0; i < 2; i++) {
        if (nrfx_ppi_channel_alloc(&allocated[i]) != NRFX_SUCCESS) {
            goto fail;
        }
        count++;
        uint32_t event = (uint32_t)(i == 0 ? &NRF_RADIO->EVENTS_END : &NRF_RADIO->EVENTS_READY);
        if (nrfx_ppi_channel_assign(allocated[i], event,
                                   (uint32_t)&NRF_TIMER2->TASKS_CAPTURE[i]) != NRFX_SUCCESS ||
            nrfx_ppi_channel_enable(allocated[i]) != NRFX_SUCCESS) {
            goto fail;
        }
    }
    NRF_TIMER2->TASKS_CAPTURE[2] = 1;
    uint32_t before = NRF_TIMER2->CC[2];
    k_busy_wait(20);
    NRF_TIMER2->TASKS_CAPTURE[2] = 1;
    uint32_t elapsed = NRF_TIMER2->CC[2] - before;
    if (elapsed < 15 || elapsed > 1000) {
        goto fail;
    }
    return 0;
fail:
    for (unsigned int i = 0; i < count; i++) {
        nrfx_ppi_channel_free(allocated[i]);
    }
    NRF_TIMER2->TASKS_STOP = 1;
    return -EIO;
}

static void probe_timing_before_tx(void)
{
    probe_rx_end_us = NRF_TIMER2->CC[0];
    NRF_TIMER2->TASKS_CAPTURE[2] = 1;
    probe_software_age_us = NRF_TIMER2->CC[2] - probe_rx_end_us;
}

static void probe_timing_after_tx(bool ended)
{
    printk("TIMING completed=%u rx_end_to_handler_us=%u rx_end_to_tx_ready_us=%u rx_end_to_tx_end_us=%u modecnf0=%08x\n",
           ended, probe_software_age_us, NRF_TIMER2->CC[1] - probe_rx_end_us,
           NRF_TIMER2->CC[0] - probe_rx_end_us, NRF_RADIO->MODECNF0);
}

#ifdef LATENCY_DIAGNOSTICS
static uint32_t probe_timing_rx_end(void)
{
    return NRF_TIMER2->CC[0];
}

static void probe_timing_record_ack(void)
{
    latency_ack(probe_rx_end_us, NRF_TIMER2->CC[1], NRF_TIMER2->CC[0]);
}
#endif
