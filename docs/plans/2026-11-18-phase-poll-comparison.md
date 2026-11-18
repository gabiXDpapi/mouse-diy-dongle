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
