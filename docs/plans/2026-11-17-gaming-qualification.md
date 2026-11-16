# Gaming performance and long-term reliability qualification

User prioritizes long-term reliability and gaming capability, targeting 1000Hz
if achievable. Keep the working normal v14 image as rollback. Today's work stops
by02:00AM Manila November17; no new flash or physical trial before that break.
These are project acceptance targets, not an external gaming certification.

## Current evidence and gaps

Normal v14 auto-start/replug/mouse power cycles and >18min operation passed.
V13 foreground host delivery reached1000events/fullsecond in parts of a manual
motion run;normal HID endpoint is configured1ms. Neither fact proves sustained
fresh1000Hz reports,RF-to-USB latency,click-to-photon latency or long-term stability.
Background Raw Input counts are unsuitable for polling qualification. Existing
RF polling uses50us busy-wait/yield and software turnaround;busy USB withholds
ACK to preserve retries. Those are measurement candidates,not proven bottlenecks.

## Qualification sequence

1. Verify actual USB interrupt descriptor/speed and1ms interval,without replacing
   drivers. Establish current wired and normal-wireless foreground delivery under
   documented sustained motion. Reuse old references for context; any new paired
   capture must answer a defined consistency question,not repeat baseline blindly.
   Predefine active epochs;publish median,p95,p99,max and missed/long intervals,
   not only peak or average. Natural pauses/zero-motion suppression are not RF loss.
2. Add an opt-in diagnostic build with fixed-memory counters/histograms for RF END
   ->handler,RF END->HID enqueue,ACK turnaround,and HID enqueue->USB completion.
   Use existing TIMER/PPI capture;handle32bit timestamp wrap and async callback
   ownership in native behavioral tests. Report idle/retry/busy separately from
   unique motion/click input. No per-packet text,allocation or file output in the
   hot path;read summaries only after test stop. Preserve normal no-DTR startup.
3. Correlate accepted unique input with USB submissions/completions. Check that
   fast flicks,diagonals,reversals,button chords and wheel do not lose/duplicate
   accepted motion or reorder button edges. A1ms USB poll is not1ms total latency;
   external timing is needed for physical click/motion-to-screen claims.
4. Optimize only measured bottlenecks,one change per candidate. Consider prompt
   IRQ-driven RX/buffering and hardware-assisted radio transitions if measurements
   justify them. Keep USB backpressure/ACK ownership,sequence handling and neutral
   release correct. No extra delay from smoothing or pointer acceleration.
5. Validate cold plug,receiver replug,mouse power cycle,mouse idle/sleep wake,
   Windows USB suspend/system sleep-resume and hibernation where relevant. Inspect
   USB status handling before claiming sleep support. Test first with released
   buttons;keep abrupt-unplug held-state limitation scoped to observed baseline.
6. Run a first4hour normal-use/gaming soak,then8+hours and multi-day ordinary use.
   Record firmware hash,placement,settings,uptime,reconnects,stalls/resets/errors
   and stuck-state observations. Durations are proposed qualification stages,
   not current passed tests. Longer captures should use bounded file rotation or
   sparse summaries;do not simply remove Raw Input logger's1200s safety cap.
7. Repeat useful checks at intended receiver placement under ordinary nearby
   Wi-Fi/Bluetooth activity and PC workload. Record context;do not fabricate
   interference or claim universal range from a single placement.
8. Push tested firmware/tool milestones and update context/resume at each stage.
   Each candidate needs native regression,appropriate variant build/config/UF2
   validation,verified physical readiness and a clear rollback before flashing.

## Practical feature scope

Prioritize stable1000Hz,low jitter,correct input,recovery and endurance. Optional
later features include selectable report rates,button remapping,profiles and
battery indication. Lower report rates need correct motion accumulation and
button-edge ordering. Actual sensor DPI,debounce,lift-off distance and battery
status depend on the existing mouse firmware/protocol;do not equate receiver
scaling with true DPI or guess undocumented write commands. No new dependency,
firmware change,mouse write or setting change is authorized by this plan alone
beyond the user's ongoing project scope;prepare concrete work within that scope.

## Primary references

- Microsoft USB endpoint descriptor/poll interval:
  https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor
- Microsoft Raw Input/high-definition mouse input:
  https://learn.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
- Nordic nRF52840 RADIO events/shortcuts:
  https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/radio.html
- Nordic TIMER/PPI:
  https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/timer.html
- NVIDIA system-latency measurement distinction:
  https://www.nvidia.com/en-us/geforce/guides/gfecnt/202010/system-latency-optimization-guide/
