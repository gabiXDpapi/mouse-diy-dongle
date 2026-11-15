Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md,docs/plans/2026-11-16-receiver-five-minute-session.md
and docs/plans/2026-11-16-receiver-explicit-stop.md. Inspect git status/log first.
Branch:feature/receiver-five-minute-session. Project paused after a successful
trial on November16,2026;this continuation resumes it. No automatic hardware work.

PAUSED AFTER V10 FIVE-MINUTE HARDWARE PASS 2026-11-16:
User explicitly extended work until this trial finished,then pause;trial COMPLETE
and user confirms "All worked smoothly after both cycles;stopped cleanly".
Pause now;no further trial/build/flash until user resumes. Branch
feature/receiver-five-minute-session. Private base
build-pairing-analysis/receiver-v10-sustained-20261116-015246:full335s serial,
onlyfirst180s hidden/background raw (11004 receiver records,0 read errors).
Raw left down/up84/84,right8/8,wheel410 in initial portion;not full-session raw
or foreground cadence evidence. Serial one arm,three links/two recoveries:
link host-relative22.126s,108.760s(213365ms left),168.307s(153811ms left),
final neutral-complete stop322.095s (~300s after first link). All3 link stops
neutral complete,no ERROR;final282670/327680 ACKs(155757 idle,3 status,
126910 type1),126712 HID submissions,198 retries,busy7101. No x needed/sent.
Board v10 NOW EXPIRED/PASSIVE:one arm,three discoveries,282670 ACKs used.
COM12 closed,no live serial/raw/build. User told release buttons/reconnect USB.
V10 UF2 SHA25632713b13dff337956808746b952bad1fa27d108079fa73ba52584c703ea4c2a9,
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2;
private receiver-v10-32713b13.uf2,priorv9/v8 rollback images preserved.
Limits remain finite5min/327680 ACKs,8 discoveries,1s gap,30s initial arm,
one arm/boot,x stop latches until reboot. V9 x-stop/held release hardware PASS;
V8 held-left expiry/control/motion hardware PASS;no logic changes except v10 bounds.
NEXT AFTER RESUME: review USB reset/reconnect recovery (currently latches HID
output off and requires receiver reboot),design smallest safe change/tests using
v10 as stable baseline. No automatic rearm or unbounded operation/limit increase.
Current boot cannot be reused for p;new trial needs explicit physical readiness
and normal reboot. No repeated v10 baseline/extraction/build/flash without reason.
Five-minute success is not hours-long/daily-use/interference/physical-latency or
confirmed1000Hz gaming certification. Keep DPI/Windows/mapping unchanged.

V10 software:only receiver duration/cap and banner changed from v9. Native
bounds regression failed before change;all8 checks passed after. Receiver build/
UF2/staged-source/config/banner checks pass. Existing non-HID control hashes
unchanged vs v9,not rebuilt. V9 stop code polls at10ms checks (not a guaranteed
physical stop latency),blocks ACK/discovery/p after x,disables RF before neutral,
requires reboot. V8 quiet-active logging removed random overshoot by user report.

For later live work:question-popup physical flash/input readiness;confirm actual
new arming response before mouse power-on. One serial reader,DTR,drain2s,one p.
Full current session capture335s covers30s discovery+300s active+cleanup. Host raw
logger capture_raw_mouse.exe max180s unchanged:document partial coverage. Optional
--foreground requires user-activated window;input_code0 foreground/1 background.
Windows may throttle/coalesce background raw delivery;no polling/loss inference.
No guessed DPI scaling. CORE wired Profile1 selected1600 saved;wireless DPI
retention unverified. Windows accel1/speed9 unchanged;receiver X/Y1:1.

Use question pop-ups whenever physical intervention is needed. Keep
PROJECT_CONTEXT.md AND RESUME_PROMPT.md current after meaningful steps. Push
tested milestones on feature-based branches. Private captures/RF addresses/
binaries ignored in build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
Do not resume after break or run another trial until user asks. Any new time
boundary must be checked before arming;prior extension applied only to finished trial.
