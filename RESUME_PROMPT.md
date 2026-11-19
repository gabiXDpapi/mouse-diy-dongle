Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (especially LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-gameplay-qualification.md.
Also read ACCOUNT_HANDOFF.md if resuming through a different account/computer.

The project is PAUSED at my request after the November19 gameplay check. This
message explicitly resumes the project, but new physical testing still requires
fresh readiness. No four-hour trial, recording, serial reader, build, timer service
or background monitor is running. Do not repeat completed baselines or reflash.

Current branch: feature/timed-host-polling-measurement.
Latest tested milestone: e79e7de, pushed. The preliminary30-minute gameplay check
PASSED by user report: smooth throughout, no issues or interruptions; left/right,
wheel,middle,boththumbbuttons actually used. Game/mode unspecified. No continuous
monitor/input capture or objective latency measurement ran. Duration is user-
reported with a completion report received more than30minutes after the start cue.

Known installed/tested normal image (no restoration needed):
build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256: 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
USB product: DIY Mouse Dongle v14. Automatic/no serial/arm/time limit.
Do not substitute softwarecontrolcb8bbb60 or diagnostic images.

NEXT: ask readiness/available time for a SEPARATE four-hour ordinary wireless-use
window on unchanged normal v14. It may mix desktop use, gaming and idle breaks;
no need to game for four hours continuously. Start a private manual diary only
after readiness. Record actual duration/active use and all breaks,sleep,charging,
resets,reconnects or issues. An early/interrupted window is not uninterrupted4h
qualification. No recording/game automation/continuous monitor is implied.

Prior evidence: liveUSB7e23c76 fullspeedHID2/IN0x83/max16/bInterval1/activepipe match.
Guided larger wireless run964.9movementevents/s over fixed20s,916-993each second,
median1ms,user smooth/continuous. Host-delivery evidence, not fresh sensor cadence
or physical latency. Rare15.279ms host gap and historicalA2two-record discrepancy
remain unexplained. PhaseABBA0a7c14e found~18usRFstage gain with10us radio checks
but no reliable overallgaming gain; normal keeps50usCPUwait, separate fromUSB1ms.
OneS3/5mininactivity,replug/mousepowercycle/allbuttons/wheel passed earlier.
Long4h/8h/multiday reliability and physical latency remain unverified.

Keep context and this copy/paste handoff current after meaningful steps. Push
milestones on feature branches. Use question popups and pause for physical
intervention. Keep private captures, RF addresses and binaries ignored; leave
unrelated .vscode/ and INFO_UF2.TXT untouched. Companion app/DPI/lighting writes
remain deferred. SamePC/sameWindowslogin: retain the whole project folder and
existing toolchain. DifferentPC: Git clone alone omits ignored private data and
external tooling. No account/auth credential copying is required.
