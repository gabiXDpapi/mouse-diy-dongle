PAIRED BACKGROUND RAW CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/receiver-v7-raw-20261116-001759. User performed movement
and reports speed still changed or paused;detail/clean-stop answer pending.
100s raw logger exit0,7198 receiver-only relative motion records,0 buttons,
0 read errors;median8000us,p99 16936us,max65459us. One serial arm/discovery,
58479 ACKs(5378 idle,1 status,53100 type1),52983 HID submissions,117 retries,
no ERROR. Firmware final neutral complete. Board now v7 expired/passive;COM12
and raw logger closed. No reset/rearm/build/flash. User asked to reconnect wired.
MEASUREMENT LIMITATION: logger uses background INPUTSINK/message-only HWND.
Microsoft documents background raw listeners may be throttled/coalesced:
https://blogs.windows.com/windowsdeveloper/2023/05/26/delivering-delightful-performance-for-more-than-one-billion-users-worldwide/
Thus125 events/s and raw-vs-HID counts cannot establish polling rate or loss.
NEXT SOFTWARE: minimal foreground capture mode and foreground/background event
labels;self-check/build/smoke,then physical foreground wired/wireless comparison.
Retain background captures;do not repeat them or infer DPI scaling.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md,especially LATEST HANDOFF / NEXT;README.md;
research/RADIO_ANALYSIS.md;and docs/plans/2026-11-15-raw-mouse-input.md.

Current branch: feature/raw-mouse-input-diagnostics. Inspect git status/log first.
Receiver hardware is still v7,idle ACK sequence hold,expired/passive after its
last trial:one arm/discovery and58479 ACK attempts used. COM12 is closed;no live
serial/build/Raw Input session. Do not send p again before a normal receiver
reboot. V7 limits remain65536 total ACKs/60s/eight discoveries/1s gap;initial
discovery arm30s,once per boot. Current UF2 hashb6a63b99667a764ed32efb2a3f46bf0721c99f43ecbd9131f81ea08370fde16a.

Wired motion is smooth. Wireless v7 felt better than v6,but user still reports
speed changing during steady movement. CORE wired Profile1 had1600 DPI selected
and was saved unchanged on request. No separate wireless DPI control exposed;
battery-mode DPI equivalence is unverified. Windows acceleration1/speed9 was
read only,not changed. Receiver X/Y mapping remains1:1. Do not guess scaling or
claim a fixed DPI/latency solution. Quiet-active candidate was deferred and its
source changes restored;private patch/plan exist under build-pairing-analysis.

New read-only Windows mouse diagnostic: research/capture_raw_mouse.c.
Built executable: ignored build-pairing-analysis/capture_raw_mouse.exe.
Build/self-test:
gcc -std=gnu11 -Wall -Wextra -Werror -O2 research/capture_raw_mouse.c -o build-pairing-analysis/capture_raw_mouse.exe -luser32 -lshell32
build-pairing-analysis/capture_raw_mouse.exe --self-test
Compile/self-tests,1s timed start/stop and existing-file protection PASS.
Wired controlled capture is complete (see NEXT checkpoint below). Logger registers mouse only,
filters wired258A:2018 and receiver2FE3:0008,does not inject input or change
settings. CSV records QPC elapsed microseconds/source/deltas/buttons/flags;
stdout includes UTC start. New CSV required;duration1..180s. Host arrival is
not physical-event latency,DPI or RF loss. Native execution may need elevation
because of sandbox restrictions. Raw metadata/binaries/captures stay ignored.

WIRED RAW INPUT BASELINE COMPLETE 2026-11-16: user performed movement and
confirmed smooth/consistent wired control. Private capture raw-wired-20261116-001333.csv,
.stdout.txt and .summary.json in build-pairing-analysis. 45s run exited0:
3674 wired-only records,0 read errors/failed0;all mouse_flags0(relative),3669
motion records and5 button-event records. X range-73..72,Y-33..26. Median
whole-run arrival gap8004us;steady seconds21..35 each125 records/s. Idle/user
pauses affect whole-run tails;not a physical latency,polling or DPI measurement.
No live logger/serial/build session. Receiver remains v7 expired/passive.
NEXT: see newest checkpoint at the top;background pair is complete and
foreground logger comparison is next. Do not rearm on the used boot.
Do not repeat wired baseline. Keep private captures ignored.


Later wireless comparison requires normal receiver reboot,separate Glorious
OFF/unplugged readiness,paired raw/serial capture,single p with DTR,and actual
arming confirmation BEFORE mouse power-on. Do not auto-retry or silently rearm.
Do not repeat completed extraction,firmware builds/flashes or passive baselines.

Continue until physical intervention is needed. Use question pop-ups for those
pauses. Keep PROJECT_CONTEXT.md AND this copy-paste RESUME_PROMPT.md current
after meaningful steps. Push tested milestones on feature-based branches.
Keep private captures,addresses,binaries ignored;leave .vscode/ and INFO_UF2.TXT
untracked. No messages to others. No actual1000Hz/latency/perfect-consistency claim.
