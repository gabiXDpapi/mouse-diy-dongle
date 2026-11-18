Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-usb-frame-timing-review.md.

NORMAL V14 FIVE-MINUTE IDLE RECOVERY PASS - 2026-11-18 14:45 Manila:
User confirmed recovered promptly, controls smooth, no reconnect/powercycle. Idle
explicitstart14:36:35; wake cue AFTER14:41:35 threshold. Segmented passive captures
completed cleanly: pre2286/wake2381 receiver records, bothread_errors0/failed0.
Wake raw contains movement,left1/right2/middle2 balancedpairs,wheelUP29/DOWN40;
thumb edges0 and activity reaches capture end: do NOT claim post-idle thumb test.
Pre/post recordedinputgap613.099s includes uncaptured idle; only conservative
user-confirmed5min inactivity claimed, not uninterrupted10min/deep-sleep proof or
measured wake latency. Private v14-idle-20261118-143124.summary.json. Journal cue
UTC recorded after popup, not exact cue delivery. No continuous full-idle coverage.
Installed normal v14 automatic and user-confirmed; no recording/serial/build live.
NEXT released-button Windows sleep/resume reliability check on unchangedv14 if
physical readiness/time permits, otherwise future USBphase diagnostic per review.
Pause15:00Manila today. No further flash planned. No fast-poll promotion/appwrites.

Current branch: feature/input-conservation-diagnostics.
Normal v14 rollback: build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
User confirmed restored operation. No extra flash is needed.

V17 software9ecc2bf and hardware result4b99ed2 pushed. Accepted9585=host9585,
dx/dy/wheel and all5buttons3down/3up matched; user smooth/cleanstop.
Private base v17-latency-20261118-142055-af9a2982 summary/capture saved.
Historical A2 device27594/host27592 cause remains unknown. Aggregate equality
cannot prove individual-event preservation/order. V15/v16 ABBA showed faster
RF reaction but no consistent total-latency improvement; no fast-poll promotion.
True sensor/click-to-screen latency and sustained fresh1000Hz remain unqualified.

Keep context and copy-paste resume current after each meaningful step. Commit/push
tested milestones on feature branches. Private captures/addresses/binaries stay
ignored; leave unrelated .vscode/ and INFO_UF2.TXT untouched. Use question popups
and pause for physical intervention. On later resume refresh the day's deadline.
No mouse DPI/lighting writes; companion app deferred until gaming optimization.
