Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (especially LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md, and docs/plans/2026-11-18-gameplay-qualification.md.

The project was paused at my request on November 18, 2026, 7:37 PM Manila.
No gameplay trial, timer, recording, build, serial reader or monitor is running.
Normal v14 remains installed and user-confirmed working automatically. Do not
repeat extraction, builds, flashing, arming or completed baselines.

Current branch: feature/timed-host-polling-measurement.
Latest tested result commit: e3ffb03 (pushed); pause checkpoint follows.
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
