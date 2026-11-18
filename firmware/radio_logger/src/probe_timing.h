#include <nrfx_ppi.h>

#include "timing_device.h"
static uint32_t probe_rx_end_us, probe_software_age_us;

static int init_probe_timing(void)
{
#ifdef USB_PHASE_DIAGNOSTICS
    nrf_ppi_channel_t allocated[3];
    usb_phase_calibrated = false;
#else
    nrf_ppi_channel_t allocated[2];
#endif
    unsigned int count = 0;
    PROBE_TIMER->TASKS_STOP = 1;
    PROBE_TIMER->INTENCLR = 0xffffffffu;
    PROBE_TIMER->SHORTS = 0;
    PROBE_TIMER->MODE = TIMER_MODE_MODE_Timer;
    PROBE_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    PROBE_TIMER->PRESCALER = 4;
    PROBE_TIMER->TASKS_CLEAR = 1;
    PROBE_TIMER->TASKS_START = 1;
    for (unsigned int i = 0; i < sizeof(allocated) / sizeof(allocated[0]); i++) {
        if (nrfx_ppi_channel_alloc(&allocated[i]) != NRFX_SUCCESS) {
            goto fail;
        }
        count++;
        uint32_t event = (uint32_t)(uintptr_t)(i == 0 ? &NRF_RADIO->EVENTS_END : &NRF_RADIO->EVENTS_READY);
        unsigned int capture = i;
#ifdef USB_PHASE_DIAGNOSTICS
        if (i == 2) {
            event = (uint32_t)(uintptr_t)&NRF_USBD->EVENTS_SOF;
            capture = 4;
        }
#endif
        if (nrfx_ppi_channel_assign(allocated[i], event,
                                   (uint32_t)(uintptr_t)&PROBE_TIMER->TASKS_CAPTURE[capture]) != NRFX_SUCCESS ||
            nrfx_ppi_channel_enable(allocated[i]) != NRFX_SUCCESS) {
            goto fail;
        }
    }
    PROBE_TIMER->TASKS_CAPTURE[2] = 1;
    uint32_t before = PROBE_TIMER->CC[2];
    k_busy_wait(20);
    PROBE_TIMER->TASKS_CAPTURE[2] = 1;
    uint32_t elapsed = PROBE_TIMER->CC[2] - before;
    if (elapsed < 15 || elapsed > 1000) {
        goto fail;
    }
#ifdef USB_PHASE_DIAGNOSTICS
    uint32_t previous = PROBE_TIMER->CC[4];
    unsigned int periods = 0;
    for (unsigned int i = 0; i < 300 && periods < 3; i++) {
        k_busy_wait(50);
        uint32_t current = PROBE_TIMER->CC[4];
        if (current != previous) {
            if (previous) {
                uint32_t interval = current - previous;
                if (interval < 950 || interval > 1050) goto fail;
                periods++;
            }
            previous = current;
        }
    }
    if (periods < 3) goto fail;
    usb_phase_calibrated = true;
#endif
    return 0;
fail:
    for (unsigned int i = 0; i < count; i++) {
        nrfx_ppi_channel_disable(allocated[i]);
        nrfx_ppi_channel_free(allocated[i]);
    }
    PROBE_TIMER->TASKS_STOP = 1;
    return -EIO;
}

static void probe_timing_before_tx(void)
{
    probe_rx_end_us = PROBE_TIMER->CC[0];
    PROBE_TIMER->TASKS_CAPTURE[2] = 1;
    probe_software_age_us = PROBE_TIMER->CC[2] - probe_rx_end_us;
}

static void probe_timing_after_tx(bool ended)
{
    printk("TIMING completed=%u rx_end_to_handler_us=%u rx_end_to_tx_ready_us=%u rx_end_to_tx_end_us=%u modecnf0=%08x\n",
           ended, probe_software_age_us, PROBE_TIMER->CC[1] - probe_rx_end_us,
           PROBE_TIMER->CC[0] - probe_rx_end_us, NRF_RADIO->MODECNF0);
}

#ifdef LATENCY_DIAGNOSTICS
static uint32_t probe_timing_rx_end(void)
{
    return PROBE_TIMER->CC[0];
}

static void probe_timing_record_ack(void)
{
    latency_ack(probe_rx_end_us, PROBE_TIMER->CC[1], PROBE_TIMER->CC[0]);
}
#endif
