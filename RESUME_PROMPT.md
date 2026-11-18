Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-17-gaming-qualification.md.

NORMAL V14 RESTORATION USER-CONFIRMED / IDLE RECOVERY NEXT - 2026-11-18 14:32 Manila:
User confirmed restored normal wireless movement/click works. Installed tested
normal v14 93eb987a, automatic until unplug; no serial/capture/build running.
V17 accounting milestone4b99ed2 pushed, exact9585report/content match, no issues.
Reviewed SDK USB SOF path: usb_dc_nrfx.c queues USBD_EVT_SOF then delivers status
in usbd_work_handler; callback timestamp is NOT hardware SOF time. TIMER2 already
uses all4CC slots. Future phase instrumentation needs isolated diagnostic timer/
PPI hardware capture and native resource/race/wrap tests; not implemented/flashed.
Next useful unchanged-v14 reliability check: bounded five-minute mouse inactivity
then movement/button/wheel recovery with no receiver reset or arm, followed by
continued normal use. This only demonstrates chosen idle duration; not proof of
mouse deep sleep, OS suspend or overnight endurance. Need physical readiness popup.
Pause15:00Manila today; no long trial that overruns this deadline. User ordinary
wireless use no issues, duration unknown. Companion app remains deferred.

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
