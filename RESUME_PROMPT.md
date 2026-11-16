PHYSICAL RECEIVER REMOVAL ALSO RETAINS HELD STATE 2026-11-16:
User confirms physical unplug with LEFT held;selection kept following OTHER
pointer,cleared afterward,normal now. Job v12-physicalusb-20261116-165002-295ec075:
one actual arm/link,physical serial loss,user confirmed,no x sent. Raw2050
records/errors0,LEFTdown2/up1. Serial loss~58.387s;host LEFTTrue across portloss,
false only~81.586s after cleanup. No rearm/reopen/PnP/UAC. Firmware unchanged.
Receiver remains PHYSICALLY UNPLUGGED;all capture/serial tasks ended. Do not
claim release PASS. This is not exclusively a PnP-restart symptom.
NEXT CONTROL:run the same held-left physical USB-unplug test on Glorious in
WIRED mode,DIY receiver unplugged. Read-only raw+host button capture,no RF arm.
Compare observed host behavior before any further firmware changes. If same,
retain scoped host-lifecycle limitation and use explicit x before planned
restart;do not claim firmware can transmit a release after power is removed.
Current break17:10,armed cutoff17:06;wired reference must also finish+save before
break. No rebuild/flash/repeat of v12 PnP/removal trials now.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-physical-usb-loss.md.
Inspect git status/log first. Branch:feature/physical-usb-loss-validation.

PHYSICAL USB-REMOVAL CAPTURE SOFTWARE READY 2026-11-16:
branch feature/physical-usb-loss-validation. V12 startup-neutral PnP held-state
fix FAILED (recorded1e1b1e5). User cleared host via Esc/releases/receiver unplug;
other pointer normal. Installed v12 remains physically unplugged,no live tasks.
No firmware changes/build/flash. New research/capture_physical_usb_loss.ps1
captures one explicit arm,raw120s/serial loss,global LEFT state1Hz. User physically
unplugs receiver while LEFT held;no UAC/PnP/automatic restart/reopen/rearm. If
cancelled/no link35s/no unplug60s,one x fallback while connected. Capturer130s
bound. Serial loss alone needs user physical confirmation,not assumed proof.
Self-checks PASS for live/cancel/timeout/no-link/disconnected action gate and
actual expired-cutoff CLI rejection before hardware access. Syntax/diff checks
PASS,firmware diff0. No physical test run. No native firmware test repeat needed.
Current user break17:10 Manila;script requires explicit future UTC arm cutoff,
check before hardware and again before p. Use -ArmBeforeUtc 2026-11-16T09:06:00Z
for this session (17:06 Manila). Stop/save/close by17:10. Refuse if cutoff passed.
NEXT physical readiness popup:reconnect receiver normally(no double-reset),
Glorious OFF/USB unplugged,selectable text/other pointer,receiver cable reachable
while holding LEFT. Actual arm cue before mouse ON;move5s,hold LEFT/drag,physically
unplug ONLY receiver around15s afterON. While still held move OTHER pointer with
no clicks,observe whether selection fixes. Then release LEFT,leave receiver
unplugged. Ifstuck Esc/release/click other pointer,confirm clear. Do not replug/
rearm until observation recorded. This separates physical removal from failed
PnP restart;not a firmware fix or generalized USB-recovery certification.

Installed v12 SHA2561e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30.
Image C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and
ignored receiver-v12-1e543890.uf2;v11/v10/v9/v8 preserved. V12 native8 checks,
receiver/probe/radio builds/UF2/source/config validation passed. V12 sends one
completed all-zero HID report before RF arming,but PnP-reset user test STILL left
LEFT held. Host before/after probes True;2353 raw/error0 records. User confirmed
stuck selection and cleared it by Esc/unplug;no host settings or input injection.
Failure base:v12-activeusb-20261116-162436-bb444906.* . V11 failed the same case.
Transport restart/unarmed RF recovery succeeded;overall held-release did not.
Do not call neutral a fix or create synthetic press pulses. Next physical unplug
comparison is on unchanged firmware;no extra UAC helper needed/running.

To selfcheck:pwsh -File research/capture_physical_usb_loss.ps1 -SelfTest
Only after separate physical readiness:pwsh -File research/capture_physical_usb_loss.ps1
-ArmBeforeUtc 2026-11-16T09:06:00Z (current17:06 Manila arm cutoff). If a later
session resumes,explicitly review/update the user's deadline;never silently reuse
a past cutoff. All outputs ignored under build-pairing-analysis,new unique job.
To cancel known active job create its .cancel file;one x fallback before removal.

Bounds327680 ACKs/5min,8 discoveries,1s gap,30s first arm,one p/boot,x stop.
No automatic resume after USB restart;fresh explicit arm is user policy. V10
full5min/two mouse-only recoveries passed,V9 x release/V8 expiry release passed.
Use question pop-ups for physical pauses,actual arm confirmation BEFORE mouse ON.
Keep PROJECT_CONTEXT.md and RESUME_PROMPT.md current,push tested milestones on
feature branches. Private captures/addresses/binaries ignored;leave .vscode/ and
INFO_UF2.TXT untracked. No guessed scaling. CORE wired1600 saved,wireless DPI
unverified;Windows accel1/speed9 unchanged,X/Y1:1. Background raw/host timing
not physical latency/DPI/polling/loss proof. Daily-use reliability not established.
