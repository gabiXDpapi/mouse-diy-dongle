GAMEPLAY START CUE / AWAITING USER PLAY AND REPORT - 2026-11-19 18:35:22 Manila

User confirmed game loaded and30minutes available, GloriousON/USBunplugged with
normal receiver and otherpointer ready. Unique USB productDIY Mouse Dongle v14
verified without serial. Known installed tested image remains93eb987a.
Private manual diary: build-pairing-analysis/gameplay-normal-v14-20261119-183522-482615bf.json
Start cue recordedUTC 2026-11-19T10:35:22.181590+00:00; planned30min endUTC 2026-11-19T11:05:22.181590+00:00
(about 19:05:22 Manila). User starts their own30min manual timer.
This records the cue, not independently observed play. Actual duration/controls/
issues must come from the user afterplay. No input recording, serial, automated
gameplay, health monitor, build or timer service is running. Normal mouse operation
continues indefinitely; diary ending does not stop the receiver.

NEXT: send start/completed-or-stop popup; await actualuserplay. If returned early,
record actualduration/reason; do not claim30min pass based only on selection.
Collect game/mode optional, controls actually tried, stalls/disconnects/overshoot/
stuckbuttons and interruptions. A30min pass is preliminary, not longsoak or physical
latency qualification. Currentbranchfeature/timed-host-polling-measurement.
Keepcontext/resume current and push milestone after actualresults. No firmware,
settings, driver, game automation or companion app work is needed duringplay.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (especially LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md, and docs/plans/2026-11-18-gameplay-qualification.md.

The project resumed on November19,2026 after the November18 pause. Gameplay readiness is confirmed; a manual30-minute start cue is now being issued. See the diary state below.
A manual gameplay trial is awaiting user play/report. No recording, build, serial reader, timer service or continuous monitor is running.
Normal v14 remains installed and user-confirmed working automatically. Do not
repeat extraction, builds, flashing, arming or completed baselines.

Current branch: feature/timed-host-polling-measurement.
Latest tested result e3ffb03 and pause checkpoint da2bcff are pushed.
Exact installed/tested rollback image (private, ignored):
build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256: 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
USB product: DIY Mouse Dongle v14. No serial/arm/time limit. No restoration needed.
Do not substitute the software-only cb8bbb60 control or diagnostic images.

Next: ask readiness and available time for the preliminary 30-minute gameplay
check on unchanged normal v14. I will open a familiar practice/offline game with
Glorious ON/USB unplugged and another pointer available. Start the manual diary
only after readiness; record actual start/end, controls tried and any stalls,
disconnects, overshoot or stuck buttons. No recording, game automation, keyboard/
screen capture, settings changes or continuous health monitor is planned. An early
stop must be recorded as such. This is preliminary gameplay, not long-term proof.

Completed evidence:
- Live USB check 7e23c76: full speed, HID interface2, IN0x83, max packet16,
  bInterval1, matching active pipe: configured 1ms / target1000Hz.
- Guided larger wireless run: fixed20s, 19,298 nonzero movement updates,
  964.9/s overall, 916–993 each second, median gap1000us; user smooth/continuous.
  Private: v14-timed-motion-20261118-192310-503bc56c.summary.json.
- Larger wired reference: 842.85/s, median1007us; user smooth/continuous.
  Private: wired-timed-motion-20261118-192030-00e9c914.summary.json.
- First wireless zero-event interval did not recur after target-mouse activation
  and larger motion; original cause remains unexplained. Preserve all fixed epochs.
- Phase-aware ABBA 0a7c14e: 10us CPU radio-check wait reduced RF-to-submit~18us,
  but overall gaming benefit remains unproven. Normal retains50us CPU wait.
  That CPU wait is separate from USB1ms polling and total latency.
- One WindowsS3 and five-minute inactivity recovery passed. All five buttons,
  wheel, automatic replug and mouse power-cycle recovery passed earlier.

Limits: these are host-delivery/configuration observations, not sustained fresh
sensor1kHz, USBtoken or physical end-to-end latency certification. The guided
wireless maximum host gap15.279ms is unexplained. Historical A2 two-record gap
also remains unexplained. No 4h/8h/multiday soak or repeated sleep/hibernate pass.

Keep PROJECT_CONTEXT.md and this copy-paste handoff current after meaningful
steps. Commit/push tested milestones on feature branches. Pause with question
popups for physical intervention. Refresh the work window after this break;
previous deadlines/open-ended authorization do not start a new trial by themselves.
Keep captures, private addresses and binaries ignored; leave unrelated .vscode/
and INFO_UF2.TXT untouched. Companion app/DPI/lighting writes remain deferred.
