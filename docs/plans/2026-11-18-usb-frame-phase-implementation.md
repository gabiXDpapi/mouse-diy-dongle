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


### November 18: v18 hardware USB frame-phase check passed

One 100-second foreground trial passed hardware SOF cadence calibration and
recorded zero invalid phase samples. All 21,818 accepted reports matched Windows
Raw Input counts; aggregate movement, wheel and all five button edges matched.
Every button had three down/up pairs. The active dump probe stayed silent, and
timing/input/phase frames arrived only after the explicit neutral stop. No failed,
capped or tracking-fault flag was set. The user reported smooth controls and a
clean stop. V18 kept 50us polling; no optimization was introduced.

| Submission phase after SOF (us) | Reports | Mean RF to submit (us) | Mean submit to callback (us) | Mean RF to callback (us) |
|---|---:|---:|---:|---:|
| 0–99 | 28 | 43.821 | 1014.429 | 1058.250 |
| 100–199 | 2536 | 43.239 | 893.684 | 936.923 |
| 200–299 | 5114 | 52.771 | 820.116 | 872.886 |
| 300–399 | 2217 | 57.739 | 735.489 | 793.229 |
| 400–499 | 848 | 49.184 | 621.952 | 671.136 |
| 500–599 | 150 | 58.733 | 539.627 | 598.360 |
| 600–699 | 151 | 35.980 | 397.960 | 433.940 |
| 700–799 | 2371 | 42.868 | 315.592 | 358.461 |
| 800–899 | 5638 | 49.136 | 229.560 | 278.696 |
| 900–999 | 2765 | 59.464 | 147.280 | 206.744 |

The overall RF-to-USB completion-callback mean was 565.837us, maximum 1,150us,
and p99 histogram upper bound 1ms. The phase-associated difference is much larger
than the RF-to-submit stage in this run. This supports treating USB frame position
as a major source of variation when interpreting earlier polling comparisons.
It does not establish causality for the historical two-record gap or a performance
improvement. The 0–99us bucket has only 28 reports, and later buckets still contain
outliers; phase alone does not explain every delay.

These are accepted-input receiver-stage timings, including driver work-queue
callback dispatch and instrumentation overhead. They exclude sensor, prior RF
queue/retry delay, Windows/game/display latency. SOF phase is sampled at software
submission, rather than at the physical USB transaction. Aggregate equality is
not proof of individual-event preservation/order. Further optimization comparisons
need repeated independently started, comparable workloads and phase-aware analysis.

Image SHA256:
`05eaf02aebbbb49163a2a8f9b95c1e5914ae06afd73a4793eba43ab48e2549cd`.
Private captures and image remain ignored. Restore exact tested normal v14 after
this diagnostic; latest installed state is in PROJECT_CONTEXT.md.
