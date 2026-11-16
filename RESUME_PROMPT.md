Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-startup-neutral.md.
Inspect git status/log first. Branch:feature/receiver-startup-neutral.

V12 STARTUP-NEUTRAL SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-startup-neutral. V12 BUILT/VALIDATED,NOT FLASHED.
V11 active USB-loss held-release failed;host cleared after Esc/released buttons/
physical receiver unplug. Read-only host left/right=false,ClickLock=false.
V12 sends exactly one six-byte all-zero neutral after first USB configuration,
main-thread wait for completion BEFORE DTR/RF arming. Pending blocks forwarding;
EAGAIN/I/O/timeout keeps output blocked. Duplicate config/service idempotent.
Reuses link-end neutral helper;no synthetic press,motion,scaling/protocol/budget
change. Passive startup now includes this zero HID report (not RF input).
Runtime startup-gate regression failed on v11;all8 native tests PASS including
zero bytes,no RF report/counter/sequence consumption,error/completion/offline
cases and prior bounds/stop/reset behavior. Receiver/probe/radio builds+UF2 PASS.
Staged source/config/HID separation/v12 banner+startup completion marker verified.
61988 flash/16128 RAM,243 UF2 blocks0x26000..0x352FF. Exact receiver image:
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA2561e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30.
Private receiver-v12-1e543890.uf2,priorv11/v10/v9/v8 preserved. Validation JSON
receiver-v12-build-validation.json. Probe/radio rebuilt;byte identity not claimed;
default HID existing image unchanged,not rebuilt. Host controller now expects
v12+neutral completion marker,records read-only global LEFT state before/after
reset;selfchecks/syntax PASS,no host input injection. No live helper/serial/raw/build.
Hardware STILL v11,receiver PHYSICALLY UNPLUGGED;no v12 flash or hardware result.
Windows may ignore initially zero button state after re-enumeration;this remains
an UNPROVEN experiment,not a guaranteed fix. Do not claim release success until
same active reset physical test. If it fails,clear host state via Esc/release/
receiver unplug and reassess;no synthesized press/release workaround.
NEXT PHYSICAL PAUSE:keep Glorious wired/OFF,reconnect only receiver and double-
reset into NICENANO. Verify bootloader identity/exact v12 hash/UF2,copy,then
passive startup showing startup neutral complete/no RF arm. Separate UAC Prepare
and normal-reboot/OFF held-test readiness for controller's v12 job. No new timing
cutoff supplied on this resumed afternoon session. Do not reuse old v11 job.

Failure capture:build-pairing-analysis/v11-activeusb-20261116-155201-22120a8b.*
One p,one receiver-only PnP restart/helper exit0,fresh passive boot,ACKs0/HID0
while2851 RF packets seen. Raw1795 records/errors0,LEFT down2/up1 (final down
unreleased). User selection followed OTHER pointer and remained after click/up;
Esc/release/physical receiver unplug cleared. Overall held-release FAIL despite
capture transport success. All old controller/helper processes ended.

V11 installed hashb1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b.
V11 configured-passive USB restart and fresh-arm x-release had passed;active
loss exposed this new gap. V10 full5min/two mouse-only recoveries passed.
Limits remain327680 ACKs/5min,8 discoveries,1s gap,30s first arm,one p/boot,x stop.
User policy:no automatic session continuation after USB restart;fresh explicit arm.

Host tools research/capture_active_usb_reset.ps1 (Prepare/Capture/SelfTest) and
restart_receiver_usb_deferred.ps1 (-SelfTest) are tracked. Prepare obtains UAC,
waits180s for signal;Capture needs75s left,unused job,fresh v12 startup. It arms
once,closes sole serial reader and signals receiver-only reset at link+25s,
reopens CDC and observes no automatic rearm;raw150s. Jobs/IDs/captures private
in ignored build-pairing-analysis. To cancel exact known job,create .cancel.
No helper currently running. All old v11 jobs consumed/finished;never reuse them.

Use question pop-ups for physical readiness/pause;actual arm cue BEFORE mouse
power-on. Keep both handoff files current,push tested milestones on feature
branches. Leave .vscode/ and INFO_UF2.TXT untracked. No guessed scaling or host
setting changes. CORE wired1600 saved,wireless equivalence unverified;Windows
accel1/speed9 unchanged,X/Y1:1. Host raw/background timing not physical latency/
DPI/RF-loss/polling proof. No daily-use/gaming reliability claim. Prior02:30
cutoff belonged to earlier paused session;current user resumed without a new one.
