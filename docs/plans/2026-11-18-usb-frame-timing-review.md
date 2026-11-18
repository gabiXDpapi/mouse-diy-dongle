# USB frame timing review

Status: reviewed only; no new firmware, build or flash. Normal v14 restored and
user-confirmed. The current goal is to distinguish USB frame phase from RF handler
reaction before interpreting further polling comparisons.

The checked-out Zephyr driver enables SOF notifications conditionally, queues
USBD_EVT_SOF in its peripheral event handler and dispatches USB_DC_SOF from
usbd_work_handler. An application callback timestamp includes work-queue delay.
Do not label that callback as hardware start-of-frame time.

TIMER2 currently has no free capture slot: CC0 RADIO END, CC1 RADIO READY,
CC2 software reaction/self-check, CC3 diagnostic clock. TIMER2 has four capture
registers; using CC4 would be invalid even though the shared CMSIS struct exposes
six fields for larger timer instances. A future diagnostic should use a verified
unowned six-channel timer (for example TIMER3) for all timestamps and a separately
allocated PPI channel from USBD SOF to a spare capture. Keep normal timing intact.

Before implementation: verify device-tree/Kconfig ownership, USBD event generation
with SOF IRQ disabled and PPI resource availability against the installed driver.
Prove the hardware timestamp advances at the expected USB frame cadence before
accepting phase samples. Do not add a 1kHz queued software callback just to observe
phase. Clean up partially allocated channels on startup failure; fail closed.

Measurement must associate a stable SOF reference with each accepted unique USB
submission and account for boundary races, stale/absent SOF, timer wrap, reset and
suspend. Preserve rejected samples as explicit invalid counts. Record per-phase
sample counts and RF-to-USB timing bins/sums, not just a global mean; keep both
input conservation and existing stopped-only diagnostic guards. No active text.
The host reader must require every new frame/footer and validate complete sample
coverage. Equal-phase comparisons still need independent repeated boots and
comparable motion/workload; one hardware run cannot promote an optimization.

Implementation would touch probe_timing.h, main.c, latency_metrics.h,
latency_dump.h, CMake/build/config, native timing/resource tests and host analyzer/
controller tests. Use a separate feature branch and preserve exact normal v14.
Do not build or flash until that change is implemented and tested as one coherent
candidate. No performance change is included in this measurement proposal.

References:
- Installed source: C:/Users/Gabriel/zephyrproject/zephyr/drivers/usb/device/usb_dc_nrfx.c
- Nordic TIMER register counts: https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/timer.html
- Nordic USBD SOF/PPI: https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/usbd.html

Additional source verification during the idle check: nrf_usbd_common_start(false)
removes SOF from the interrupt mask but leaves the USB peripheral operating. This
supports testing event-to-PPI capture without enabling queued SOF callbacks, but
the actual hardware cadence still needs a startup self-check. Endpoint IN success
also becomes EP_EVT_WRITE_COMPLETE in the same USB work queue before DATA_IN and
HID int_in_ready. Our existing RF-to-USB callback metric therefore includes that
dispatch delay; it must not be relabeled USB wire completion or physical latency.
The exact observed source chain is usb_dc_nrfx.c:usbd_event_transfer_data ->
usbd_work_process_ep_events -> HID core.c:hid_int_in -> wireless_hid_ready.
