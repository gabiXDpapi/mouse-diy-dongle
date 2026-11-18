Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (especially LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-input-conservation.md.

V17 SOFTWARE VERIFIED / PHYSICAL FLASH READINESS NEXT - 2026-11-18 14:16 Manila:
Branch feature/input-conservation-diagnostics. Pause today at 15:00 Manila
(2026-11-18T07:00:00Z). Normal v14 remains installed and automatic; no hardware
access, serial reader, capture or build session is running. User ordinary wireless
use since last session: no issues noticed; duration unknown, not measured soak.
App input v17 built successfully. Preserved private input-v17-aea67abc.uf2,
SHA256 aea67abc7c748d90c3fde1cc2931343d3e81fcf5b2492c741de82cebfb81b01d.
196 application blocks 0x26000..0x32400 exclusive. Staged sources, flags and quiet
1ms USB configuration verified; normal control diagnostics OFF; tested rollback
93eb987a verified. All 10 native checks, 7 research tests, controller SelfTest and
syntax passed. Private v17-software-validation.json. V17 NOT flashed or hardware
qualified. Adds stopped-only accepted dx/dy/wheel and five-button edge accounting;
no performance change or normal fast-poll promotion. Aggregate equality cannot
prove individual-event preservation/order or historical A2 two-record cause.
NEXT physical popup: Glorious wired/OFF, receiver NICENANO. Verify board/exact
image, flash, verify USB product DIY Mouse Input v17 without serial. Separate OFF/
USB-unplugged readiness, then 100s foreground capture ExpectedProduct v17 with
FinishBeforeUtc 2026-11-18T07:00:00Z. No arm command; automatic startup. Cue ON,
activate capture with other pointer, each of five buttons 3 press/releases, wheel
UP5/DOWN3, then controlled motion until x at ~70s. Collect user feel/stop; analyze
both complete frames and all receiver rows. Restore EXACT tested normal v14
93eb987a after trial via fresh physical readiness, verify ordinary operation.
Prefer no diagnostic flash after 14:45; no capture at/after14:52. Companion app,
DPI writes and USB frame-phase instrumentation deferred. Private captures/binaries
ignored; unrelated .vscode/ and INFO_UF2.TXT untouched.

Keep context and this copy-paste handoff current after each meaningful step.
Commit/push tested milestones on feature branches. Pause with a question popup
when physical intervention is needed. Refresh today's deadline on a later resume.

Preserved tested rollback:
build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
Product DIY Mouse Dongle v14. Normal automatic until unplug; no DTR/arm needed.

Gaming goal: reliable low latency, approximately 1000Hz if achievable. Current
v15/v16 ABBA tests show faster RF reaction but no consistent total-latency benefit;
do not promote fast polling. A2 device 27594 / host 27592 records remains
unexplained. V17 measures aggregate content, not true end-to-end physical latency.
No long endurance, sleep/wake or broad RF qualification claimed. All five buttons,
wheel, smooth motion, two bounded 15min runs and normal 18min/replug passed earlier.
