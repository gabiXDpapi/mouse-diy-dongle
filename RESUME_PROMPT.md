V11 ACTIVE USB-LOSS HELD-RELEASE FAILURE CONFIRMED 2026-11-16:
Job v11-activeusb-20261116-155201-22120a8b completed capture with0 read errors,
1795 receiver records. One arm/link,one receiver PnP restart(helper exit0),one
fresh passive v11 boot,no second p;post-reset ACKs0/hid_reports0 while2851 RF
packets observed. Software transport recovery succeeds,but overall hardware
held-release test FAIL:other-pointer movement extended selected text while
Glorious LEFT remained held. Other-pointer click/release did not clear;Esc,
release all buttons and physical receiver unplug cleared it. User confirms normal
pointer now. Read-only Windows left/right state false;ClickLock disabled.
Raw LEFT events:down23.717s/up23.772s/down28.174s,NO matching final up. Last
raw47.810s;PnP began07:54:25.315Z,postboot07:54:28Z. Physical no-resume assertion
confounded by held state;only log-based no rearm established. No injection/settings
changes. All capture/helper/serial processes finished. Receiver PHYSICALLY UNPLUGGED
per clearing instruction;last installed v11,postreset unarmed before poweroff.
NEXT SOFTWARE: test an isolated startup-neutral report hypothesis. Current v11
fresh boot has last_buttons0 and emits no neutral until armed/held;add exactly
one all-zero HID report after configuration,outside callback,before RF output/
arming,wait completion,gate output on success. Native zero/no-motion/error/
idempotence checks,build/validate,preservev11. This is unproven on Windows after
PnP reset (HID class may ignore unchanged zero state);no synthetic button press.
Do not claim fix until same active-reset physical test. No repeat v11 test now.

ACTIVE USB-LOSS HELD-RELEASE FAILURE OBSERVED 2026-11-16: during job
v11-activeusb-20261116-155201-22120a8b,user reports selected text followed OTHER
pointer while Glorious LEFT remained held after receiver restart. Thus Windows
held-state release FAILED,despite fresh passive boot/no second arm. Do not mark
whole trial PASS. User instructed release Glorious LEFT,click/release other
pointer LEFT on harmless text,reconnect Glorious USB or leave OFF. Clearing-state
popup pending. No new arm/serial writes. Existing capture finishing read-only;
helper restart already completed. Need inspect logs/physical no-resume separately.
Likely follow-up investigate sending neutral after USB reconfiguration before
allowing input;not implemented,not assumed proven. Keep hardware passive.

ACTIVE USB-LOSS CAPTURE IN PROGRESS 2026-11-16: job
v11-activeusb-20261116-155201-22120a8b. User confirmed normal receiver reboot,
Glorious OFF/unplugged,text/other pointer. Helper lifetime/fresh v11 boot checks
passed;ONE p sent,actual arm confirmed before mouse-ON popup. Raw150s and serial
running. Controller closes sole serial reader/signals one receiver restart at
link+25s,then opens new receiver CDC and observes passive boot/no second p.
User holding LEFT through restart,checking selection with other pointer,then
release/move Glorious to verify no auto-resume. Leave mouse ON/unplugged for RF
observation. No UAC interaction during held input. If user cancels,write exact
build-pairing-analysis/v11-activeusb-20261116-155201-22120a8b.cancel file.
No second reader/manual restart/rearm. Pending helper/capture/physical results.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-active-usb-loss-capture.md.
Inspect git status/log first. Branch:feature/active-usb-reset-validation.

ACTIVE USB-RESET HELPER PREPARED 2026-11-16: user confirmed UAC readiness
and approved helper. Job v11-activeusb-20261116-155201-22120a8b in ignored
build-pairing-analysis. Helper READY,expires2026-11-16T07:55:05Z (15:55:05 Manila).
No restart/arming/serial capture yet. Waiting for separate normal receiver reboot,
Glorious OFF/unplugged,text/other-pointer readiness popup. Capture requires75s
remaining helper life;refuse if too late (rough latest start07:53:45Z before
prearm drain). To cancel,write job.cancel;helper exits without PnP restart.
On readiness run research/capture_active_usb_reset.ps1 -Mode Capture -RunName
v11-activeusb-20261116-155201-22120a8b. Never substitute another job or reset
armed state. Fresh v11 startup required;one actual arm cue before mouse ON.

RESUMED / ACTIVE USB-RESET HOST TOOLS READY 2026-11-16:
User resumed later;old02:30 cutoff/pause no longer applies. No new cutoff given.
Branch feature/active-usb-reset-validation. Installed firmware remains v11,
last known disarmed/latched24544 ACKs;COM12 closed,no live sessions. No firmware
edit/build/flash or hardware/serial/admin action on this resumed turn.
New tracked scripts research/restart_receiver_usb_deferred.ps1 and
research/capture_active_usb_reset.ps1 reuse existing exact receiver PnP target.
Deferred helper gets UAC FIRST,waits180s for unique signal,expires/cancels without
restart,revalidates healthy exact USB root and safe ID characters before one
restart. PnP child limited15s;result private. Controller Prepare creates unique
job;Capture requires unused matching job/>75s helper life,fresh v11 startup and
explicit physical reboot/OFF readiness.150s hidden raw,one p/actual arm cue,
link+25s close sole serial reader and signal restart,reopen expected receiver CDC
and observe new passive startup without second p. Abort attempts x if armed and
recovery not verified;helper cancelled on exit. Background raw not cadence proof.
Self-checks PASS:exact/foreign/duplicate/mismatched targets,unsafe ID chars,
cancel/signal/expiry gates,used/wrong/expired/short-lived jobs,and existing-log
protection. JSON DateTime/string deadline conversion bug caught/fixed;fixture
preserves logs and touches no hardware. Syntax/diff checks PASS;firmware diff0.
Recorded private target ID fits strict format;raw executable exists. No helper
prepared or running yet. Runtime active USB loss/held-release remains UNTESTED.
NEXT PHYSICAL PAUSE:mouse wired/OFF,receiver normally connected;user ready for
UAC. Run controller -Mode Prepare,approve helper BEFORE any mouse holding.
Record RUN_NAME/helper expiry. Then promptly separate readiness popup for normal
receiver reboot(no double-reset),Glorious OFF/USB unplugged,selectable text/other
pointer. Capture -Mode Capture -RunName ... only with sufficient helper lifetime;
if expired cancel/defer,new job/UAC needed,no automatic arm retry. After real arm
cue,user briefly moves then holds LEFT through targeted restart,checks other
pointer selection fixed while still held,releases,then moves Glorious to verify
NO input resumes without explicit arm. Keep receiver connected/mouse ON afterward
for passive RF observation. Save scoped outcomes;MCU reset cause not instrumented.

Installed/preserved v11 SHA256:
b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and private
receiver-v11-b1d27489.uf2;v10/v9/v8 rollbacks preserved. No new flash needed.
V11 configured-passive Windows restart returned fresh unarmed boot(no loop);
fresh explicit arm afterward restored smooth controls and held-left x release.
Active USB-loss remains the missing hardware check. Firmware requests COLD reboot
outside callback after configured reset/disconnect,initial enumeration reset
ignored,generic USB_ERROR fatal. User policy:no automatic session continuation.
Limits327680 ACKs/5min,8 discoveries,1s gap,30s discovery,one arm/boot,x stop.
V10 five-minute/two mouse power-cycle test passed;V8 held-left expiry passed.

Host commands:
pwsh -File research/restart_receiver_usb_deferred.ps1 -SelfTest
pwsh -File research/capture_active_usb_reset.ps1 -Mode SelfTest
pwsh -File research/capture_active_usb_reset.ps1 -Mode Prepare
pwsh -File research/capture_active_usb_reset.ps1 -Mode Capture -RunName <ready-job>
Prepare alone never restarts/arms. UAC helper expires180s;at least75s must remain
when arming. Job state/ready/result/signal/cancel/captures are ignored under
build-pairing-analysis. To cancel known job,create its .cancel file;do not open
another serial reader or signal a restart manually. No automatic retries.

Use question pop-ups for every physical readiness/pause;confirm actual arm
BEFORE mouse ON. Keep PROJECT_CONTEXT.md and RESUME_PROMPT.md current,push tested
milestones on feature branches. Private captures/addresses/binaries ignored;
leave .vscode/ and INFO_UF2.TXT untracked. No guessed gain/DPI/ACK tuning. CORE
wired1600 saved,wireless equivalence unverified;Windows accel1/speed9 unchanged,
X/Y1:1. Host arrival/background raw is not physical latency/DPI/loss/polling proof.
