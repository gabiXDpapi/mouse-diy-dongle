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
