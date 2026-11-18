# Phase-aware polling comparison

Goal: compare50us and10us radio polling with the SAME tested v18 hardware phase
and input accounting. V18's first calibration/accounting trial passed. No normal
promotion or physical gaming-latency claim is assumed.

Candidate Appphase-fast/productDIY Mouse Phase Fast v19 changes only the existing
FAST_POLL_DIAGNOSTICS flag toON (50->10us wait) relative to Appphase v18. Reuse all
C instrumentation/protocol/bridge code unchanged. Add build/config and exact
controller product acceptance; require the same three stopped frame footers.
Native/regression/SelfTest and stagedsource/config/UF2/hash checks before flash.
No new native behavior requires a duplicate test; existing fast-radio code and
phase tests apply. Never replace normal rollback93eb with softwarecontrolcb8bbb.

Preserve exact hardware-tested v18baseline05eaf02a. InitialA1 is private capture
v18-latency-20261118-174506-f45a9f45. If practical use B1/B2/A2 (two independent
boots each condition), same surface/DPI/settings and requested controls/motion.
Record each run separately and preserve stopped timing/input/phase validation and
userfeel. Restore exactnormalv14 after diagnostics. Userwindowuntiltheysaystop;
use bounded100s capture with operational30minrollbackguard, no arm command.

Report global and per-phase counts, RF reaction/submit means and USB-callback
means/histogram upper bounds. Show run-to-run variability; do not treat thousands
of packets as independent boots. Ten100us buckets leave within-bucket phase
variation, manual motion/hostload are uncontrolled, and earlyphaseA1 has n28.
Phase associations or weighted means alone cannot establish a20us causal gain.
Any claim of reliable total improvement requires repeated comparable evidence;
otherwise retain tested normal50us behavior and document the inconclusive result.
No sensor/OS/game/display or physical click latency is measured here.

Predeclared interpretation: primary descriptive outputs are radio reaction and
RF-to-submit, global RF-to-callback timing, per-phase sample counts/means and
backpressure. If phase-normalizing, disclose common supported bins/reference
weights and excluded mass; compare independent runs individually. The100us bucket
width permits within-bin phase skew larger than the expected~20us RF change.
Do not manufacture packet-level confidence or infer physical improvement from a
small weighted difference. Report callback-dispatch/instrumentation scope and
input conservation separately. Improvement remains unproven until repeatability
and reliability qualification support promotion.


### V19 phase-aware comparison: first hardware run

V19 B1 passed startup calibration and the stopped timing/input/phase guards.
All 19,376 accepted inputs matched foreground Windows record counts, aggregate
X/Y/wheel and five-button edges (three down/up pairs each). One phase reference
was classified invalid; its accepted timing sample remained in the explicit
invalid bucket and all totals reconciled. The user reported smooth controls,
clean stop and no issue. No failed/capped/tracking-fault flag or raw read error.

| Receiver-stage measurement | V18 A1, 50us wait | V19 B1, 10us wait |
|---|---:|---:|
| Mean RF END to handler (us) | 41.672 | 22.830 |
| Mean RF END to submit (us) | 50.774 | 33.196 |
| Mean RF END to USB callback (us) | 565.837 | 427.624 |
| Maximum RF END to USB callback (us) | 1150 | 1119 |
| p99 histogram upper bound (us) | 1000 | 1119 |
| Busy candidates (%) | 0.592 | 1.134 |
| Invalid phase references | 0 | 1 |

The unadjusted callback mean differs by138.214us. Using A1's report shares as
fixed weights over the ten valid phase bins gives565.837us for A1 and553.031us
for B1, a12.807us descriptive difference; B1's one invalid sample is excluded.
All valid phase bins have support, but some have few reports. The much smaller
phase-weighted difference shows this comparison is sensitive to phase mix.
It is not a causal estimate:100us buckets leave within-bin variation, manual
movement and CPU/USB workload are uncontrolled, and this is one boot per condition.
The tail upper bound and backpressure did not both improve. Keep normal50us v14;
no everyday fast-poll promotion or physical gaming-latency gain is established.

Private B1 base:v19-latency-20261118-175953-8719f360; comparison summary and weights
are in ignored phase-comparison-a1-b1.json. Preserve both exact images for an
independent reverse-order/repeated-boot check if practical. Restore exact tested
normalv14 after this diagnostic and consult PROJECT_CONTEXT for installed state.

Replication batch: after the user-confirmed normal restoration following B1, reuse exactsaved v19 for B2 then exactsaved v18 for A2, with separate physical flash/input readiness and stopped diagnostics between them. Restore exact normal v14 after both runs or immediately if the batch is abandoned. No intermediate normal flash or new build is needed.


### Phase-aware A1/B1/B2/A2 comparison complete

All four independently booted100-second trials passed input accounting, frozen
frame validation and user-reported smooth controls/clean stop. Device/host counts
and aggregate motion/wheel/five-button edges matched in every run. B1 had one
explicit invalid phase reference; the other runs had none. No raw read errors.

| Run | Wait(us) | Reports matched | RF to submit mean(us) | RF to callback mean(us) | A1-phase-weighted callback mean(us) | Callback p99 upper bound(us) | Busy(%) |
|---|---:|---:|---:|---:|---:|---:|---:|
| A1 | 50 | 21818 | 50.774 | 565.837 | 565.837 | 1000 | 0.592 |
| B1 | 10 | 19376 | 33.196 | 427.624 | 553.031 | 1119 | 1.134 |
| B2 | 10 | 22994 | 33.192 | 427.069 | 542.093 | 1113 | 0.293 |
| A2 | 50 | 29967 | 52.084 | 505.845 | 578.692 | 1000 | 2.943 |

The two50us runs averaged51.429us RF-to-submit, versus33.194us in the two10us
runs: a repeatable18.235us receiver-stage reduction in these measurements. Global
callback means differ more, but the phase mix also differs. Using A1's fixed valid
phase-bin weights, boot-mean callback timings are572.265us and547.562us, a24.703us
descriptive difference. All ten bins have support and no A1 reference mass is
excluded; B1's one invalid reference is excluded from phase weighting. Weighted
means do not resolve100us within-bin skew or manual-motion/host-load confounding.
Two independent boots per condition do not support packet-level confidence claims.
The callback tail bounds/backpressure do not show uniform improvement.

Conclusion: faster software radio checking improves the measured receiver stage;
a reliable physical or whole-system gaming-latency gain is not established. Keep
normal v14 at its tested50us wait. USB endpoint polling remains configured1ms in
both conditions:50/10us is a CPU wait between radio checks, not USB polling rate
or total latency. Do not equate these numbers with fresh1000Hz sensor reports.

Normal v14 exact93eb987a has now been restored, USB-verified without serial and
user-confirmed working automatically. No reader/capture/build/helper remains.
All images/captures and phase-comparison-abba-20261118.json remain private/ignored.
Next directly verify live USB endpoint interval/speed, then define sustained-motion
and longer ordinary-use/gaming checks. Companion app and mouse setting writes
remain deferred. No further baseline repetition is needed for this comparison.
