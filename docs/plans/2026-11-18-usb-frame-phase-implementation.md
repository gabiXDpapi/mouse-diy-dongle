# USB frame phase diagnostic implementation plan

**Goal:** measure accepted USB submissions by hardware SOF phase before another optimization comparison.
**Architecture:** opt-in phase build v18 moves the existing diagnostic clock/captures to unowned TIMER3. A third allocated PPI channel captures USBD SOF in CC4 without queued SOF notifications. Accepted completed inputs accumulate RF-to-USB callback histograms by 100us submission-phase bucket; invalid/stale/boundary samples remain explicit. Normal images keep TIMER2.
**Tech stack:** existing C/Zephyr/nrfx PPI, Python unittest/analyzers, PowerShell controller; no dependencies.

User authorized autonomous project continuation. Implement directly in this feature branch using the existing workspace and checks; no extra design approval or subagent is needed. Companion app remains deferred.

## Scope and constraints
Touches main/probe timing, latency collector/dump, build/CMake/config, native timing/resource/bridge checks and host analyzer/controller. Preserve input accounting and legacy v15/v16/v17 frame compatibility. No active serial text, new IRQ callback, normal performance change or mouse setting write. Keep private image/capture ignored and tested v14 rollback intact.

## Task 1: timer and PPI acquisition (native failure test first)
Create firmware/test_probe_timing.c/.py and a new diagnostic timer selector header. Exercise successful calibration, unavailable PPI, assign/enable failure, stalled timer, absent/stale/irregular SOF and cleanup. Modify probe_timing.h only for conditional third resource and bounded cadence check; assert TIMER3 DT/Kconfig ownership and SOF IRQ disabled. TIMER3 has six CC slots, TIMER2 four. Reuse CLOCK at1MHz; CC0/1RF,CC2age,CC3softwareclock,CC4SOF. Startup must fail closed unless at least three consecutive observed ~1ms SOF intervals pass within300 polling steps of50us (plus interrupt overhead).

## Task 2: accepted phase accounting
Extend existing latency tests (failure first) for valid phase buckets, boundary change/stale exclusion, wrap, failed write exclusion and callback-before-write-return. A sandwich SOF read around submission timestamp excludes changing references; phases>=1000us are explicit invalid. Only accepted USB completion records phase; bucket RF-to-USB histogram and RF-to-submit sum. Reset with existing protected session reset. Both aggregate timing and bucket sums/counts must reconcile; invalid coverage explicit.

## Task 3: stopped-only phase frame and host checks
Append PHASE_V1 plus eleven PHASE rows (ten valid phase bins and one invalid bucket) and PHASE_END after existing timing/input frames, same1.5sec output deadline. Test partial writer/DTR behavior and complete formatter. New research/analyze_usb_phase.py calls existing input/latency validation, checks width/bins/calibration, ordered unique bins, nonnegative/count/sum/hist consistency and complete report coverage. Return per-bucket timing means/upper bounds; no optimization inference. Tests cover zero/invalid buckets, signed input conservation, malformed/capped/inconsistent/partial phase frames. Extend existing controller ExpectedProduct v18 and require all three footers; self-test legacy and partial phase refusal.

## Task 4: build and checkpoint
Add App phase, product DIY Mouse Phase v18, LATENCY/INPUT/USB_PHASE ON and FAST OFF. Stage all new files, run all native and research tests and PowerShell syntax/SelfTest. Build phase and normal control, verify staged sources, flags, USB quiet1ms config, UF2/ranges/hash, preserve private image. Confirm exact normal rollback93eb hash. Update PROJECT_CONTEXT/RESUME/README/research and push software milestone before physical readiness popup.

## Task 5: hardware qualification and restore
Only within renewed user window: mouse wired/OFF, NICENANO/exact image verification, flash and USB identity. Separate OFF/unplugged readiness; existing100s foreground capture with oneCDCreader, noarm, active d silent, x70s, three complete frozen frames. Test normal controls/motion, validate input totals and phase coverage/calibration, collect userfeel/stop. Restore exact tested normalv14 through fresh physical readiness and verify ordinary wireless use. Update context/push each tested milestone. No blind repeat of prior baselines; one phase run qualifies instrumentation, not improved performance.
