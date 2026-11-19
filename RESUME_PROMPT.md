Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-gameplay-qualification.md.

PRELIMINARY30min GAMEPLAY PASS / FOUR-HOUR READINESS NEXT - 2026-11-19 19:09 Manila

User confirmed30minutes completed, smooth movement/aim throughout, no issues or
interruptions, and actual use of left/right clicks,wheel,middle,boththumbbuttons.
Completion report arrived11:07UTC, more than30min after10:35:22UTC cue; manual
user-reported duration30min. Game/mode unspecified. No input/health recording,
serial, automated gameplay, driver/settings/firmware changes ran. Exact physical
play start/end are not independently observed. Private current-gameplay-diary.txt
points to gameplay-normal-v14-20261119-183522-482615bf.json, now user-reported PASS.

Installed receiver remains exact tested normal v14 93eb987a, automatic/noarm/no
limit. No active capture, reader, build, timer service or background monitor.
NEXT push preliminary milestone then ask readiness/available time for a SEPARATE
four-hour ordinary wireless-use window on unchanged firmware. No new flash/build
or repeated baseline. Mix desktop/gaming/natural idle if desired; no requirement
to game continuously. Record actual active use/breaks and all resets/reconnects/
sleep/wired charging/issues. Keep normal power/settings policies. Only start diary
after fresh readiness; early/interrupted window is not uninterrupted4h pass.

The30min pass is preliminary, not4h/8h/multiday or physical-latency qualification.
Guided near1kHz HOST delivery/configured1ms USB evidence stands with limitations;
rare15.279ms host gap and historicalA2two-record discrepancy remain unexplained.
Companion app/DPI/lighting writes deferred. Keepcontext/resume current, private
captures/addresses/binaries ignored; unrelated.vscode/INFO_UF2.TXT untouched.

Current branch: feature/timed-host-polling-measurement.
Prior tested result e3ffb03, pause da2bcff and manual start0f239db are pushed;
30-minute gameplay result milestone follows. No new physical trial has started.

Known installed/tested normal image (no restoration needed):
build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256: 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
USB product: DIY Mouse Dongle v14. No serial/arm/time limit.
Do not substitute softwarecontrolcb8bbb60 or diagnostic images.

Prior evidence: liveUSB7e23c76 verifies fullspeedHID2/IN0x83/max16/bInterval1/
activepipe match. Guided larger wireless run averaged964.9movementevents/s over
fixed20s,916-993each second, median1ms; user smooth/continuous. This is host-
delivery evidence, not actual fresh sensor polling or physical end-to-end latency.
PhaseABBA0a7c14e showed~18us RFstage reduction with10us radio checks but no reliable
wholegaminggain; normal keeps50us CPUwait, separate from USB1ms polling. No more
ABBA repeats needed. OneS3 and5mininactivity recovery passed; replug/powercycle/
allfivebuttons/wheel passed earlier. Long4h/8h/multiday testing is still pending.

Keep context and this copy-paste handoff current after meaningful steps. Push
tested milestones on feature branches. Pause with question popups for physical
readiness/results. Do not start a new use window before readiness or imply any
recording/continuous monitor. No recording or timer service is running now.
