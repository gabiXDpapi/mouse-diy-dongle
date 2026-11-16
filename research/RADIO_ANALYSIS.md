# First radio capture experiment

## V12 startup-neutral experiment built

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


## V11 active-loss held-button release failed

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

Windows raw button fields are transition flags: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse


## Active USB-loss capture preparation

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


## V11 post-recovery arm passed;paused

PAUSED BEFORE02:30 / V11 POST-RECOVERY ARM PASS 2026-11-16:
User's02:30 Manila work window is ending;pause after completed test. No further
hardware actions until user resumes. Branch feature/receiver-usb-reset-restart.
User confirms all controls smooth and held-left release after NEW explicit arm
following receiver-only Windows USB restart. Configured-passive restart PASS;
active USB-loss/held-button behavior across reset is NOT yet tested.
Private base build-pairing-analysis/receiver-v11-rearm-20261116-022118:
100s hidden raw1975 receiver-only background records,0 read errors;left down/up
15/15,wheel45. Right click user-confirmed but no independent right transition
in this short capture. Link21.144s,x46.166s,raw up46.176s,neutral/disarmed chunk
46.199s;host timing is not physical latency. One arm/discovery,24544 ACKs,
8065 HID submissions,48 retries,busy75,no ERROR;one disarmed/reboot response.
Board v11 NOW DISARMED/LATCHED,one arm/discovery,24544 ACKs used. COM12 closed,
no live serial/raw/build/admin helper/UAC session. User told release/reconnect USB.
Installed v11 SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and private
receiver-v11-b1d27489.uf2. V10/v9/v8 rollback images preserved. Bounds remain
327680 ACK/5min,8 discoveries,1s gap,30s initial discovery,one p/boot,x stop.
NEXT WHEN USER RESUMES: prepare ACTIVE USB-reset test,including old-session
abort,no automatic resume,fresh explicit arm and held-button behavior. Review
UAC sequencing first:approval must occur before held-input observation,otherwise
consent clicks confound the test. Bounded deferred receiver-only admin helper
may be needed;NOT implemented. Existing helper restarts immediately. Exact
receiver root privately identified;never replace drivers/target other devices.
Configured-passive PnP restart returned one fresh unarmed boot/no loop;MCU reset
cause not separately instrumented. Active-reset release/startup neutral remains
unverified. Do not claim complete USB recovery or daily-use/gaming reliability.
Current used boot needs explicit normal-reboot readiness before another arm.
No repeated v11 build/flash/passive baseline without a new reason.


## V11 configured-passive host restart passed

V11 CONFIGURED-PASSIVE WINDOWS USB RESTART PASS 2026-11-16:
User approved UAC for fixed receiver-only helper. Run private base
build-pairing-analysis/receiver-v11-usbrestart-20261116-021713. Exact receiver USB
root revalidated;pnputil restart exit0/helper exit0. No other device/driver change.
After restart,COM12 reenumerated;8s passive DTR capture has ONE fresh Zephyr boot
and v11 banner,zero traffic,no ERROR/arming/link or restart loop. COM12 closed.
Scope:configured-passive host-restart scenario only;MCU reset cause not separately
instrumented,active USB-loss/held-button behavior NOT tested. No automatic arm.
Board v11 fresh boot UNARMED,unused327680 ACK/300000ms/8 discovery budgets.
Flash metadata references restart validation;raw/result files ignored. No sessions.
NEXT physical readiness:post-recovery explicit-arm controls/x-stop check. Harmless
scrollable/selectable doc,other pointer,Glorious OFF/USB unplugged,receiver stays
connected without reset. Prepared capture-v11-rearm.ps1 syntax to check;100s raw/
95s serial,one p,actual arm cue,x ONCE25s after link. Try motion/clicks/wheel then
hold LEFT through x stop,other pointer checks selection fixed. No second p.
No short arm after02:26;pause/save by02:30. Active USB-reset scenario deferred.


## V11 flash/passive startup verified

V11 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user confirmed NICENANO
visible/mouse wired-OFF. Bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and exact
v11 hash b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b verified;
242 UF2 blocks0x26000..0x351FF validated,copy complete,COM12 USB identity matched.
6.5s passive capture receiver-v11-passive-20261116-021434.txt:correct v11 passive
USB-restart banner,zero traffic,no ERROR/arming/link/disarm. COM12 closed.
Board NOW v11 UNARMED,unused327680 ACK/300000ms/eight-discovery budgets.
Metadata receiver-v11-flash-validation.json. No host USB-restart test yet.
NEXT physical/UAC readiness:keep Glorious wired/OFF,receiver unchanged. Windows
administrator token absent;launch fixed receiver-only restart-receiver-usb-admin.ps1
via Start-Process -Verb RunAs -WindowStyle Hidden,with validated run-name and
exact target from receiver-usb-restart-target.json. User approves Windows UAC.
Read private result,wait for expected CDC re-enumeration,open sole serial reader
DTR and capture new startup passively without p. Verify fresh v11 boot/unarmed,
no errors or reset loop. If approval/restart fails,no blind retry or driver change.
Configured-passive restart test alone doesn't prove active held-button recovery;
record scope honestly. Then separate explicit-arm readiness/control trial if time.
Pause02:30;no short armed trial after02:26,5min after02:22. No new firmware build.


## V11 passive USB restart candidate built

V11 PASSIVE USB RESTART SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-usb-reset-restart. V11 BUILT/VALIDATED,NOT FLASHED.
User policy:stay passive after USB reconnect,explicit arm required. Configured
RESET/DISCONNECTED requests COLD software reboot via main-thread service;callback
only latches off. Initial enumeration reset doesn't restart;USB_ERROR remains
fatal. Service runs in DTR wait and fatal receive path;new boot uses existing
unarmed/one-arm rule.5min/327680 ACK/8 discoveries/1s gap/x stop unchanged.
New missing-service regression failed compilation on v10;all8 native checks PASS
including reset/disconnect/error split,quick-config output block and deferred
restart. Receiver/probe/radio builds+UF2 PASS;receiver staged sources/config/
v11 banner verified,CONFIG_REBOOT=y. Probe/radio hashes changed with shared
edits,not byte-identical controls;default HID existing image unchanged.
V11 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b.
61812 flash/16128 RAM,242 blocks0x26000..0x351FF. Private image
receiver-v11-b1d27489.uf2;v10/v9/v8 preserved. Metadata receiver-v11-build-validation.json.
Board STILL v10 expired/passive282670 ACKs/3 discoveries;COM12 closed,no sessions.
NEXT physical flash readiness:Glorious wired/OFF,receiver double-reset NICENANO,
verify identity/hash,copy then passive startup(no arm). Then receiver-only
Windows host restart test requires administrator/UAC because current tokennotadmin.
Exact USB root identified privately in receiver-usb-restart-target.json.
Ignored restart-receiver-usb-admin.ps1 prepared/syntax PASS,NOT EXECUTED;it
revalidates unique exact receiver root and runs pnputil /restart-device only on
that instance,writes private result. No driver replacement/other devices touched.
Begin with passive configured receiver;fresh v11 startup after host restart and
no arm/input required before further tests. Power unplug alone not proof. Active
USB-loss/held-button release remains separate hardware evidence if not tested.
Pause by02:30 Manila;no short armed trial after02:26 or5min after02:22.


## V10 five-minute controls/recovery pass; paused

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


## V10 flashed and passive startup verified

V10 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: exact v10 hash/UF2 and
NICENANO0.6.0/nRF52840-nicenano/S1406.1.1 verified;copy complete. COM12 expected
USB identity confirmed. Passive6.5s capture receiver-v10-passive-20261116-014706.txt
shows correct bounded-five-minute/x-stop banner,zero traffic,no errors/arm/link.
Board NOW v10 UNARMED,unused327680 ACK/300000ms/eight-discovery budgets.
COM12 closed,no live sessions. Metadata receiver-v10-flash-validation.json.
Prepared capture-v10-sustained.ps1:335s serial+first180s hidden raw;single p/DTR,
2s drain,actual arm confirmation. Per-capture .stop-request file sends x ONCE
through same reader if user reports issue. Arm cutoff01:52AM Manila2026-11-16;
pause/save by02:00. No full-session raw/foreground cadence claim.
NEXT physical trial readiness: harmless scrollable document/other pointer,
Glorious OFF/USB unplugged,same settings,receiver connected unchanged. Trial
movement/clicks/wheel,mouse-only OFF3s/ON around1min and3min,then observe original
5-minute stop. Stop early with x on trouble;never reopen serial/automatically rearm.


## V10 bounded five-minute candidate built

V10 FIVE-MINUTE SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-five-minute-session. V10 BUILT/VALIDATED,NOT FLASHED.
Only receiver cap/session constants327680 ACKs/300000ms and banner changed;
8 discoveries,1s gap,30s initial arm,one arm/boot,x latch/neutral,quiet active,
mapping/ACK timing retained from hardware-passed v9. Bounds regression failed
on v9;all8 native checks PASS including original shared deadline/stop errors.
Receiver build/UF2/source/config/banner checks PASS:61560 flash/16128 RAM,
241 blocks0x26000..0x350FF. Existing probe/radio/default HID hashes verified
unchanged vs v9,not rebuilt because non-HID paths not modified.
V10 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA25632713b13dff337956808746b952bad1fa27d108079fa73ba52584c703ea4c2a9.
Private receiver-v10-32713b13.uf2,receiver-v9-ecc725eb.uf2 and v8 preserved;
metadata receiver-v10-build-validation.json. Hardware benefit not tested.
Board STILL v9 DISARMED/latch,25000 ACKs used;COM12 closed,no live sessions.
NEXT physical flash readiness: mouse wired/OFF,receiver double-reset NICENANO;
verify identity/hash/copy then passive startup,no arming. Separate5-minute
trial readiness if time fits. Full335s serial,onlyfirst180s optional hidden raw
(control events,not cadence). Exercise controls and mouse OFF3s/ON around1 and
3minutes,observe original5-minute stop. x early stop available through same reader.
HARD PAUSE02:00 Manila;do not arm5-minute trial after01:52. Recheck clock before
arming;defer if too late. Update handoff/push all tested milestones before pause.


## V9 explicit-stop hardware pass

V9 EXPLICIT X STOP HARDWARE PASS 2026-11-16: user confirms selection fixed
while LEFT physically held after x stop;circles smooth like v8/wired.
Private base build-pairing-analysis/receiver-v9-xstop-20261116-013634.
100s hidden raw2393 receiver records,0 read errors. Link observed22.335s;
host x sent47.365s,raw left-up47.372s,serial neutral/disarmed chunk47.397s.
Host/USB/serial chunk times are not physical stop latency. One arm/discovery,
25000 ACKs(11151 idle,0 status,13849 type1),13840 HID submissions,9 retries,
busy4011,no ERROR;one disarmed/reboot-required response. No second p sent.
Board v9 DISARMED/LATCHED,one arm/discovery,25000 ACKs used. COM12 closed,
no live raw/serial/build. User instructed reconnect mouse USB. V9 preserved.
NEXT SOFTWARE: v10 bounded5-minute experiment,327680 ACK cap/300000ms,keep
8 discoveries/1s gap/30s initial arm/one arm per boot/x stop/neutral/quiet logic.
Only constants/banner and dependent tests change. All normal checks/builds.
No automatic flash/arm. Physical readiness later explains5-minute bounds.
Hard stop02:00 Manila;no new5-minute arm after01:52 (full335s capture+wrap).
Host raw logger stays1..180s;first180s optional raw is partial,full335s serial
covers5-minute session. Do not claim full-session raw coverage or gaming latency.


## V9 flash/passive startup verified

V9 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user confirmed NICENANO
visible/Glorious wired-OFF. Bootloader0.6.0,nRF52840-nicenano,S1406.1.1 verified;
exact v9 hash ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e,
UF2 partition validation241 blocks0x26000..0x350FF PASS;copy completed.
COM12 expected USB identity verified.6.5s passive capture privately saved as
receiver-v9-passive-20261116-013345.txt:correct v9 explicit-x/quiet banner,
zero traffic,no errors/arm/link/disarm. COM12 closed,no live serial/raw/build.
Board NOW v9 UNARMED with unused65536 ACK/60s/eight-discovery budgets.
V8 rollback preserved. Metadata receiver-v9-flash-validation.json:armed=false.
NEXT PHYSICAL PAUSE: Glorious OFF/USB unplugged,selectable harmless text and
other pointer,same DPI/settings;leave receiver connected without reset.
Prepared ignored capture-v9-xstop.ps1 syntax PASS,not executed. Single p/DTR,
2s drain,95s serial+100s hidden raw;enforces01:56AM arm cutoff. Actual arm
confirmation BEFORE mouse ON. User circles5-10s,then hold LEFT dragging text.
Script sends x ONCE25s after observed link start,logs command host UTC/elapsed,
expects disarmed/neutral/reboot-required response. While LEFT physically held,
other pointer without clicks should leave selection fixed,then release.
No second p or automatic retry. Background raw for release events only,no
cadence claim. Verify motion stays v8-like by user report. End/save by02:00.


## V9 explicit-stop prerequisite ready for hardware

V9 EXPLICIT-STOP SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-explicit-stop. User resumed and requested break after
next step;software preparation complete,now at physical flash/break boundary.
V9 BUILT/VALIDATED,NOT FLASHED. Adds receiver-only lowercase x cancellation:
10ms paced CDC checks during scan/active loops,latches off ACKs/discovery/p
arming,disables RF before neutral,prints disarmed success only after release.
Reboot required to rearm. Poll interval is not a physical stop-latency guarantee.
Limits remain65536 ACKs/60s/8 discoveries/1s gap;no sustained limit increase yet.
Native active-stop regression failed on v8;all8 tests now PASS (prearm/active/
rediscovery cancellation,disable/neutral errors,no ACK after latch,no p rearm,
repeated handling and paced polling). Receiver/probe/radio builds/UF2 checks PASS.
Staged sources/config/HID separation/v9 banner+disarmed response verified.
Probe/radio controls not byte-identical after shared-source edits;native probe
behavior checked,new stop code receiver-only;default HID existing image unchanged.
V9 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e.
61536 flash/16128 RAM;241 blocks,0x26000..0x350FF. Private rollback/candidate
receiver-v8-5df75330.uf2 and receiver-v9-ecc725eb.uf2;metadata
build-pairing-analysis/receiver-v9-build-validation.json. No v9 hardware claim.
Board STILL validated v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used.
COM12 closed,no live serial/raw/build. No new flash/arming on this resumed turn.
NEXT PHYSICAL PAUSE when user continues: Glorious wired/OFF,double-reset only
RECEIVER into NICENANO;verify bootloader identity/exact v9 image before copy,
then passive startup without arming. Separate explicit motion/held-left x-stop
readiness and one arm;capture x command timestamp,neutral/reboot-required response
and Windows release while physically held. Also check motion remains v8-like.
Do not extend limits until explicit-stop hardware pass;preserve v8 as baseline.
Do not repeat completed extraction/v8 tests/builds without a new reason.


## V8 held-left timeout confirmed; project paused for break

PAUSED FOR BREAK / V8 BOUNDED VALIDATION COMPLETE 2026-11-16:
User requested break around01:10 AM Manila;paused early after final trial.
Resume ONLY when user returns;no automatic hardware action. Branch
feature/receiver-quiet-active-window. User now confirms held LEFT beyond70s,
other pointer did not extend selection:forced timeout release HARDWARE PASS.
Raw up at80.928s matches neutral-complete stop~80.947s after link~20.954s.
V8 motion(no overshoot/wired-like),movement/clicks/wheel,mouse-only OFF/ON
recovery and held-left timeout are all user-confirmed with clean stops.
Last private base receiver-v8-held-20261116-005751:2016 receiver-only raw records,
0 read errors;one arm/discovery,59958 ACKs,5613 HID submissions,23 retries,busy2,
no ERROR,neutral complete. No further build/flash needed for these completed checks.
Board v8 EXPIRED/PASSIVE;current boot one arm/discovery,59958 ACKs used. COM12
closed;no live serial/raw/build. User instructed release buttons/reconnect USB.
V8 UF2 SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and ignored
receiver-v8-5df75330.uf2;v7 rollback preserved. Source/tests/software milestones pushed.
NEXT WHEN RESUMED: design a bounded sustained-session experiment using v8 as
stable baseline,including explicit stop/neutral handling and finite safety limits.
Review existing guards/tests and decide smallest change before building. Do not
silently enable continuous/unbounded operation,increase limits or arm. New live
trial requires explicit physical readiness and normal reboot of used receiver.
No repeated extraction,baseline,flashing or completed v8 tests without new reason.
No physical1000Hz/end-to-end latency/DPI equivalence/daily-use reliability claim.


## V8 forced-release capture before break

V8 HELD-LEFT CAPTURE COMPLETE / BREAK PAUSE 2026-11-16: final trial before
requested01:10 AM Manila break. No further trial planned;resume only when user
returns. Private base build-pairing-analysis/receiver-v8-held-20261116-005751.
100s hidden raw exit0,2016 receiver-only records,0 read errors. Button events:
left-down21.635s/up21.690s,then held-down22.116s and up80.928s. Host serial
chunks show link start20.954s and neutral-complete stop80.947s (about60s).
One arm/discovery,59958 ACKs(54322 idle,0 status,5636 type1),5613 HID submissions,
23 retries,busy2,no ERROR. Forced release observed at stop;user confirms trial
performed,but physical-held/selection-fixed observation is PENDING in popup.
Do not mark physical held-left PASS until that response. User asked release
LEFT/reconnect USB. Board v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used;
COM12 closed,no live serial/raw/build. No new firmware changes.
NEXT on resume: collect pending held-left observation,then record final bounded
v8 validation. If PASS,next scope is sustained-session design/testing,not an
unbounded/daily-use claim. No automatic reset/rearm/flash or limit increase.
Keep successful v8 motion(wired-like,no overshoot),controls and mouse-only
OFF/ON recovery milestones;all already user-confirmed. Saved v8 image unchanged.


## V8 controls and mouse-only recovery passed

V8 CONTROLS/RECOVERY HARDWARE PASS 2026-11-16: user confirms movement,
left/right clicks and wheel UP/DOWN worked before/after ONLY mouse OFF/ON,
receiver untouched;clean stop,no stuck buttons. Private base
build-pairing-analysis/receiver-v8-controls-20261116-005207.100s hidden raw
exit0,4982 receiver-only background records,0 read errors,4865 motion.
Left down/up32/32,right4/4,wheel63(41 negative120,22 positive120). Both wheel
directions captured in later segment;before-recovery wheel supported by user
report,not independently present in raw events. No cadence conclusions from
background capture. Serial one arm,two discoveries/links;second discovery had
30371ms remaining in original session. Final53698 ACKs(21225 idle,2 status,
32471 type1),32437 HID submissions,34 retries,busy396,no ERROR. Both link stops
neutral complete. V8 motion previously user-confirmed wired-like/no overshoot.
Board v8 EXPIRED/PASSIVE,one arm/two discoveries,53698 ACKs used. COM12 closed,
no live raw/serial/build. User told to reconnect mouse USB. No new firmware changes.
NEXT PHYSICAL PAUSE: v8 held-left timeout regression on harmless selectable
plain text and other pointer. Normal receiver reboot(no double-reset),Glorious
OFF/unplugged. One confirmed arm;hold LEFT over text beyond60s window,while
still held use other pointer without clicks to confirm selection fixed,then
release. Hidden raw/serial capture can verify release event at stop. Do not
increase limits/build/flash/rearm on current used boot. This checks forced
release on unchanged v8 before considering sustained sessions.


## V8 motion trial: overshoot absent, wired-like feel

V8 FIRST MOTION HARDWARE PASS 2026-11-16: user reports "Overshoot gone /
like wired;stopped cleanly" with capture window active. One successful trial;
not calibrated physical latency/DPI or final gaming-performance validation.
Private base build-pairing-analysis/receiver-v8-foreground-20261116-004654:
100s raw exit0,50748 receiver-only relative foreground records,50746 motion,
2 button records,0 read errors,no background records. Median arrival1001us,
p90 1087us,p99 2926us;whole-run max includes pauses. Max axis6/5 vs v7's31/14,
hand movements not mechanically matched. Serial one arm/discovery,59468 ACKs
(8706 idle,1 status,50761 type1),50757 HID submissions,4 retries,busy895,
no ERROR,neutral complete. Active-link stats/busy TEXT counts both0 as intended.
Busy total retained;895 vs v7's171 messages is not a like-for-like loss metric.
Board v8 NOW EXPIRED/PASSIVE,one arm/discovery used,59468 ACKs consumed.
COM12 closed,no live raw/serial/build. User instructed reconnect Glorious wired.
V8 image SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
no further build/flash/tuning needed before regression check. V7 preserved.
NEXT PHYSICAL PAUSE: v8 controls/recovery regression. User normal receiver
unplug/replug(no double-reset),harmless scrollable document/other pointer,
Glorious OFF/USB unplugged,same DPI/settings. Capture100s foreground+95s serial,
one p/DTR with actual arm confirmed before mouse ON. User tries movement,
left/right-click,wheel up/down,then MOUSE OFF3s/ON after about15s,tries controls
again,and observes clean60s stop. Keep receiver connected. New boot/readiness
must be explicitly confirmed before clearing used capture checkpoint;do not
reuse current arm or flash-validation false state automatically. No flash.
Analyze only foreground input_code0;activation with other pointer. Preserve
successful wired/v7/v8 captures;do not repeat baseline/extraction/build.


## V8 flashed and passive startup verified

V8 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: v8 NOW FLASHED on receiver.
User confirmed NICENANO visible with Glorious wired/OFF. Verified D: NICENANO,
bootloader0.6.0/nRF52840-nicenano/S1406.1.1,and exact v8 UF2 hash
5df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
UF2 validation PASS240 blocks0x26000..0x34FFF. Copy completed;COM12 reenumerated
with expected receiver USB ID.6.5s passive DTR capture privately saved as
build-pairing-analysis/receiver-v8-passive-20261116-004355.txt. Verified v8 quiet
banner,timing init,zero packets/CRC/invalid length,no ERROR/arming/link.
Initial check incorrectly demanded ACKs=0 stats,which passive scan does not emit;
saved log/source reviewed and validation corrected without repeat capture/flash.
No p sent since flash,so UNARMED,unused65536 ACK/60s/eight-discovery budgets.
COM12 closed,no live serial/raw/build. Metadata receiver-v8-flash-validation.json.
NEXT PHYSICAL PAUSE: same harmless document/surface/saved DPI/other pointer;
Glorious OFF and USB unplugged;leave receiver connected without reset. After
readiness,paired100s foreground Raw Input/95s serial drain2s,one p/DTR,actual
v8 arming confirmation before mouse ON. User activates capture window using
other pointer,then30s circles/straight strokes,no clicks/wheel/DPI changes.
Capture script must be updated for v8/previously consumed startup banner:
verify flash checkpoint plus passive scans,no preexisting arm/link/errors,then
confirm actual new arm. Do not require a second startup banner or repeat baseline.
Compare input_code0 motion and user overshoot with preserved v7/wired captures.
Hardware benefit of v8 remains untested;all firmware/host software checks passed.


## V8 quiet-active experiment ready for physical flash

V8 QUIET-ACTIVE SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-quiet-active-window. Receiver v8 BUILT/VALIDATED,NOT FLASHED.
Change only active diagnostics:skip periodic stats while linked,replace per-busy
text with cumulative EAGAIN counter in final summary. Keep v7 idle ACK rule,
motion mapping,ACK timing,first timing samples,link/error prints and all limits.
Regression failed on v7;all8 native tests PASS with v8. Receiver/probe/radio
builds and UF2 checks PASS. Probe/radio hashes unchanged;default HID image hash
unchanged,not rebuilt. Receiver staged source/config/v8 banner verified.
V8 UF2: C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd.
61256 flash/16128 RAM;240 UF2 blocks,0x26000..0x34FFF. Private preserved image
build-pairing-analysis/receiver-v8-5df75330.uf2;metadata receiver-v8-build-validation.json.
V7 rollback receiver-v7-b6a63b99.uf2 hash verified. V8 is an unproven A/B logging
overhead experiment,not an established overshoot fix;less active-time telemetry.
Board STILL v7 expired/passive after foreground trial:one arm/discovery,59129
ACKs used. COM12 closed,no live raw/serial/build. Glorious asked to reconnect wired.
NEXT PHYSICAL PAUSE via popup: keep Glorious wired/OFF,double-reset only RECEIVER
into NICENANO. Verify bootloader identity and exact v8 hash/UF2 before copy,then
passive startup only,no arming. Separate later motion readiness for paired
foreground raw100s/serial95s,one p/DTR,actual arming confirmation BEFORE mouse ON.
Raw logger accepts --foreground;user must activate window with other pointer;
analyze input_code0. No repeat v7 baseline/build/extraction. DPI/Windows unchanged.


## Foreground wireless overshoot reproduced

FOREGROUND WIRELESS COMPARISON COMPLETE 2026-11-16: user kept window active;
random overshoot persists;clean stop confirmed. Private base
build-pairing-analysis/receiver-v7-foreground-20261116-003012 (csv/txt/host.jsonl/
raw.stdout.txt/summary.json).100s raw exit0,39180 receiver-only relative records,
39076 foreground(39075 motion,1 button),104 background,0 read errors. Foreground
median gap1025us,p90 2054us,p99 4044us;whole-run max273019us includes pauses.
One arm/discovery,59129 ACKs(19317 idle,1 status,39811 type1),39709 HID
submissions,102 retries,171 HID-busy messages,no ERROR;neutral complete.
Wired foreground was user-confirmed smooth. Wireless max axis31/14 vs wired3/2;
median motion magnitude1 in both,p99 wireless2.236 vs wired2. Occasional larger
wireless steps occur near busy serial chunks;host chunks buffered,hand motion
not identical,so correlation is not proof of logging cause or fixed DPI error.
No live raw/serial/build. Board v7 expired/passive,59129 ACKs used. User told
reconnect wired. NEXT SOFTWARE: resume preserved quiet-active-window A/B candidate
(v8),keeping all protocol/timing/mapping/limits. Test actual suppression and busy
counters,build/validate,preserve v7,then pause for physical flash readiness.
Do not repeat foreground/background v7 baselines or guess motion scaling.


## Foreground wired capture complete

FOREGROUND WIRED CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/raw-wired-foreground-20261116-002746.45s exit0,20389
wired-only relative records/errors0.20109 foreground(input_code0;20106 motion,
3 button records),280 background. Foreground median arrival1079us,p90 2947us,
p99 4950us;whole-run max includes movement pauses. Background median8007us.
User confirmed capture window stayed active;subjective smoothness answer pending.
No physical polling/latency/DPI claim. This verifies background cadence confound.
No live logger/serial/build. Firmware remains v7. NEXT pop-up already pending:
user wired observation plus normal receiver reboot/Glorious OFF-unplugged
readiness for matching foreground wireless pair. Do not assume readiness;do not
repeat wired capture. Pair script must pass --foreground and show its window;
user activates with other pointer after confirmed arm then moves Glorious.
One p/DTR,95s serial+100s raw;analyze input_code0 only. Do not reuse old boot.


## Foreground diagnostic software validated

FOREGROUND RAW INPUT SOFTWARE CHECKPOINT 2026-11-16: branch
feature/foreground-raw-input. User clarified wireless symptom: occasional random
overshoot during circles;stop was clean. Background paired capture completed as
recorded below. Do not infer polling or dropped motion from background cadence.
research/capture_raw_mouse.c now accepts optional final --foreground,shows an
ordinary capture window without forcing focus,and appends input_code to CSV:
0 foreground /1 background from WM_INPUT wParam. Default hidden mode preserved.
Known-device filtering,mouse-only registration,exclusive-create output,1..180s
limit and cleanup unchanged. No keyboard/injection/firmware/settings changes.
Serializer regression failed on old code;new self-check passes both delivery
codes plus prior bounds/filter/signed tests. GNU11 warnings-as-errors build PASS.
Hidden1s lifecycle PASS;visible1s lifecycle verified exit0/records0/errors0 and
DONE marker. Invalid option rejected before creating file. Explicit stdout flush
added after DONE because initial visible smoke omitted buffered completion text.
Final ignored exe SHA256
1e6ed06a5b447461d77d2fa20add18269634bcc8475ea6aeea8a59130c832d16.
Smoke is not movement/performance evidence. No controlled foreground capture yet.
No live capture/serial/build sessions. Board v7 expired/passive after one arm,
one discovery,58479 ACKs;COM12 closed. User told to keep mouse wired.
NEXT PHYSICAL PAUSE: wired readiness,same surface/saved1600 profile/other pointer.
Start45s logger --foreground;after readiness cue user clicks capture window and
keeps it active while doing20-30s circles/straight strokes. Analyze input_code0
only and record activation fraction. This is a new foreground measurement,not
a repeat of background baseline. Later wireless needs reboot/OFF-unplugged
readiness and one actual-confirmed arm. Do not rearm on current used boot.


## Background wireless capture and measurement limitation

PAIRED BACKGROUND RAW CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/receiver-v7-raw-20261116-001759. User performed movement
and reports speed still changed or paused;later clarified random overshoot during circles;stop clean.
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


## Wired Raw Input baseline complete

WIRED RAW INPUT BASELINE COMPLETE 2026-11-16: user performed movement and
confirmed smooth/consistent wired control. Private capture raw-wired-20261116-001333.csv,
.stdout.txt and .summary.json in build-pairing-analysis. 45s run exited0:
3674 wired-only records,0 read errors/failed0;all mouse_flags0(relative),3669
motion records and5 button-event records. X range-73..72,Y-33..26. Median
whole-run arrival gap8004us;steady seconds21..35 each125 records/s. Idle/user
pauses affect whole-run tails;not a physical latency,polling or DPI measurement.
No live logger/serial/build session. Receiver remains v7 expired/passive.
NEXT: physical pop-up for normal receiver reboot(no double-reset),Glorious
OFF/USB unplugged,same surface/profile/document and other pointer. Then paired
raw/serial wireless capture with one p/DTR and confirmed arming before mouse ON.
Do not repeat wired baseline. Keep private captures ignored.


## Host Raw Input tool ready; controlled wired baseline pending

2026-11-16 software checkpoint on feature/raw-mouse-input-diagnostics:
`research/capture_raw_mouse.c`,existing MinGW GNU11/user32/shell32,no new package.
Mouse-only INPUTSINK/DEVNOTIFY,message-only window,no NOLEGACY/keyboard/input
injection. Known wired258A:2018 and receiver2FE3:0008 labels only;private CSV
time_us/deltas/buttons/flags plus UTC startup in stdout. Buffered output,
exclusive-create protection,1..180s stop and registration/window/file cleanup.
Self-tests validate filtering/packet bounds/type/signed extremes/flags.1s timed
smoke0 records/0 read-errors/failed0;existing capture overwrite rejected without
file change. Final exe SHA25641a46ce524380cc1f50c3542889729008ff5cc1e556fdd85bc59411021c45a39.
No controlled raw movement sample yet. WindowsAPI references:
https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse
Firmware/source/Windows/DPI unchanged. Next wired USB readiness and45s host
capture with20-30s deliberate motion;then a separately armed paired wireless
comparison. Host arrival times are not physical-event latency/sensor DPI proof.

## Saved-1600 profile retry: variable speed persists

95s drained/single-arm ignored capture
`build-pairing-analysis/receiver-v7-dpi1600-retry-20261115-234324.txt`,
`.host.jsonl`,`.summary.json`. One link,58365 ACKs:9813 idle,one status,48551
type1.48444 unique HID submissions+107 retries.375 busy events,no ERROR or
rediscovery,neutral-complete stop. User specifically reports speed changing
during steady movement,not a uniform DPI offset. No independent sensor DPI,
Windows delivery of each report or physical-event latency measurement.
Next local Raw Input capture compares wired/custom HID counts/host arrival
timing without firmware/Windows/DPI changes. Mouse-only known-device filtering;
private CSV ignored,no input injection. Receiver expired/passive,COM12 closed.
Microsoft API references: https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
and https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getrawinputdata

## Saved-1600-DPI attempt missed cue; no active link

Unchanged v7/Windows settings,CORE wired Profile1 saved1600. User confirmed
normal receiver reboot/mouse OFF-unplugged readiness.95s drained single-arm
capture ignored at `build-pairing-analysis/receiver-v7-dpi1600-20261115-233835.txt`,
`.host.jsonl`,`.summary.json`. Actual arming confirmed before power-on pop-up;
no discovery reply/link start,zero ACKs/HID submissions,no errors. User chose
could-not-perform/missed cue. Later passive RX does not establish active input;
last counters good1247/bad6/invalid3 are cumulative. Comparison inconclusive,
not a firmware/DPI failure. Arming expired;COM12 closed. Need normal receiver
reboot/fresh readiness with user ready at power switch;no automatic rearm or
repeat build/flash/baseline/DPI save. Wireless DPI equivalence still unverified.

## DPI profile inspection/save after wired connection

Official Glorious CORE recognized wired Series One PRO. Performance UI shows
PROFILE1,selected1600-DPI(red outlined stage),presets400/800/1600/3200,one DPI
set with no separate wired/wireless selector. User explicitly requested matching
wired DPI;clicked SAVE without changing existing values,then refreshed UI still
shows selected1600. No other setting manually changed;Windows speed/acceleration
and v7 receiver code/binary unchanged. Save may apply current mouse profile,
but battery-mode retention/equivalence is not independently verified.
Next controlled v7 trial uses this saved profile without DPI-button changes;
normal receiver reboot/fresh readiness required because prior arm consumed.
Do not add receiver motion scaling or issue guessed vendor configuration writes.

## DPI clarification before further firmware tuning

User clarified high DPI may explain difficulty drawing fine circles,then asked
to match wireless DPI to wired if possible. Quiet-active software candidate was
tested natively but NOT built/flashed;deferred privately and own changes restored
to exact tested v7 source. Current receiver UF2 remainsb6a63b99...,hardware v7
expired/passive. Do not claim remaining variation from circle precision alone.
Installed Glorious CORE opened through computer-use skill;UI currently asks to
connect Glorious products. No wired DPI value read,no DPI/Windows setting changed.
Next physical wired USB connection,then official UI inspection/matching if
supported. Do not invent receiver gain or issue guessed vendor configuration writes.

## V7 motion comparison: subjective consistency improvement,not complete

Ignored `build-pairing-analysis/receiver-v7-motion-20261115-231733.txt`,
`.host.jsonl`,`.summary.json`.95s drained/single-arm capture,one continuous link,
59121 ACKs:11305 idle,one status,47815 type1.47750 unique HID submissions+65
retries;191 USB-busy events,no ERROR/rediscovery,neutral-complete final stop.
Two completed timing samples. First stats good2534/bad18/invalid6,last61999/22/7
are cumulative since boot;do not call them capture totals.
User reports definitely more consistent than v6 but further tuning needed.
No independent stop confirmation and no proven race/latency/lossless-delivery
claim. Board v7 expired/passive,COM12 closed. Next minimal comparison removes
active-window periodic stats/busy text while retaining counters/final summaries,
v7 idle rule and all RF/USB behavior. Active logging is a potential timing
disturbance,not an established cause of residual variation.

## V7 verified flash/passive startup (2026-11-15,23:12 Manila)

User confirmed NICENANO/mouse wired-OFF readiness;verified bootloader0.6.0,
nRF52840-nicenano/S1406.1.1 and finalb6a63b99... UF2 hash/bounds,application copy.
Five-second DTR COM12 startup passed:v7 idle-sequence-hold banner/expected
65536/60s/eight-discovery/1s-gap limits,1MHz timing,19channels,six zero scans,
no errors/arming. Windows composite/CDC/HID/Input statuses OK. Ignored
`build-pairing-analysis/receiver-v7-passive-20261115-231231.txt` and `.summary.json`.
COM12 closed;v7 fresh/unarmed,budgets unused. Next separate readiness/single-arm
95s steady-circle comparison with unchanged surface/DPI/Windows settings.
Do not repeat completed flash/passive check. No v7 input/fix claim yet.

## Wired control and idle-ACK queue-release hypothesis

User reports wired circles smooth/consistent on same surface/settings,wireless
different. Acceleration alone is not a sufficient explanation;settings unchanged.
Existing local snapshot disassembly:0x1890A..0x18950 reads queue indices while
building outgoing packet and sends length2 if empty.0x16088..0x160B8 matches
ACK=outgoing sequence+1,then rereads current queue indices and pops a report if
nonempty. A report arriving after idle construction but before ACK processing
could therefore be popped unsent by current idle+1 ACK. This is an inferred
interleaving risk,NOT observed/proven loss or recovered original-receiver ACK.
Receiver-only bounded experiment will hold sequence for length2 idle ACK;
data/status ACK+1,second ACK byte,timing,guards,budgets and USB mapping unchanged.
Non-HID probe retains previously tested behavior. Native checks/builds precede
separate verified flash and controlled steady-motion comparison. No Windows
setting change or speculative scaling/filtering. Exact state/progress in context.

V7 receiver-only experiment implemented on feature/idle-ack-queue-guard;actual
native receiver assertion failed before code,then all8 tests passed. Repeated
idle ACK holds sequence,255/0 cases preserve subsequent same-sequence data ACK+1;
non-HID mode still uses idle+1. Only receiver idle ACK first byte and banner
changed. All8 native tests/builds/UF2 bounds/mode-source separation pass. Final
cases cover status/compound ACK wrap too. Probe/radio/default HID artifacts are
byte-identical to v6 checkpoint. V7 UF2 SHA256
b6a63b99667a764ed32efb2a3f46bf0721c99f43ecbd9131f81ea08370fde16a,
flash61220/RAM16128,240 blocks,0x26000..0x34FFF. V6 preserved c0e18e29... .
Hardware still v6 expired/passive,COM12 closed. Pause for verified v7 flash/
passive startup then separate input readiness. No confirmed race/fix or v7
hardware claim. Exact next steps/full metadata in context.

## Focused v6 motion: no hard freeze noticed; pointer-speed variation persists

Unchanged v6,normal reboot/fresh readiness,95s drained/single-arm capture:
ignored `build-pairing-analysis/receiver-v6-motion-20261115-225106.txt`,
`.host.jsonl`,`.summary.json`. One continuous link,58171 ACKs:4769 idle,one
status,53401 type1;53314 unique HID submissions+87 retries.387 USB-busy events,
no ERROR or rediscovery;normal neutral-complete stop.71 stats lines,last
cumulative good59017/bad_crc6/invalid_len1. Two completed timing samples only.
User noticed no hard freezes in this run,but perceived varying sensitivity/
slower pointer than hand movement during circles. Aggregate/subsecond and motion
payload coverage is insufficient to assign cause. Some1s intervals had high idle
ACK progress but low type1/HID progress;intentional movement timing is unmeasured.
Read-only SystemParametersInfoW SPI_GETMOUSE returned[6,10,1] and SPI_GETMOUSESPEED
returned9. Registry corroborated;no settings changed. Ignored settings snapshot:
`build-pairing-analysis/windows-pointer-settings-20261115.json`. API documentation:
https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-systemparametersinfow
Nonzero acceleration may affect perceived speed,not proven as cause. Next same
circles wired/same surface/settings before firmware guesses. Receiver expired/
passive,COM12 closed;no reset/rearm/flash needed for wired comparison.

## V6 input/recovery result: responsive with intermittent motion stalls

95s drained/single-arm capture ignored at
`build-pairing-analysis/receiver-v6-input-20261115-224452.txt`,`.host.jsonl`,
`.summary.json`. Two links,55417/65536 ACKs:40808 idle,one status,14608 type1.
14557 unique HID submissions+51 suppressed retries exactly match type1 ACKs.
Four completed timing samples,no ERROR;95 USB-busy events withheld ACK.
Link1 neutral stop22573 ACKs;link2 starts at22573 with33244ms left,then final
neutral stop55417. No extra rediscovery observed.62 stats lines,last cumulative
good55748/bad_crc2/invalid0. Median1s ACK-counter rate999/s includes idle;max
1s HID submission rate864/s. These do not establish USB polling,packet loss,
physical-event latency or host delivery of each individual report.
User confirms controls before/after mouse OFF3s/ON,clean stop,and noticeably
better responsiveness/no obvious v4-style delay. However pointer sometimes
stops despite continued movement then resumes. Cause unresolved;USB-busy count
alone cannot establish causality. Board expired/passive,COM12 closed. Next a
focused steady-motion/no-click/no-wheel/no-power-cycle trial on unchanged v6,
correlating approximate freeze times with counters,busy bursts and link logs.
Requires normal receiver reboot/fresh readiness;no build/flash/baseline repeat.

## V6 verified flash/passive startup (2026-11-15,22:41 Manila)

After user NICENANO/mouse wired-OFF readiness,verified bootloader0.6.0,
nRF52840-nicenano,S1406.1.1 and finalc0e18e29... UF2 hash/240 blocks/application
bounds,then copied application only. Five-second DTR COM12 startup passed:v6
input counters,8 discoveries/65536 ACKs/60000ms/1000ms gap,1MHz timing,19channels,
six zero scans,no errors/arming. Windows composite/CDC/HID/Input statuses OK.
Ignored `build-pairing-analysis/receiver-v6-passive-20261115-224120.txt` and
`.summary.json`. COM12 closed;v6 fresh/unarmed,budgets unused. Next separate
input readiness/one95s capture;do not repeat flash/baseline. No input/latency claim.

## V6 input diagnostics software checkpoint; paused before hardware

After inconclusive v5 trial,user explicitly requested one software-only next
step then break. Branch `feature/receiver-input-diagnostics`;v5 preserved at
ignored `build-pairing-analysis/receiver-v5-ab8e3001.uf2`. V6 has65536 finite
shared ACK attempts,unchanged60s/eight-discovery/1s-gap/manual-arm behavior.
Existing1s/link-stop summaries now include cumulative successful idle/status/
type1 ACK counts and unique HID submission/suppressed retry counts. RF bytes/
timing,packet filters and USB report mapping unchanged. Neutral reports and
busy/error/short writes do not count as successful unique input submissions.
New native assertions failed before implementation;all8 tests then pass.
Receiver/probe/radio builds,UF2 bounds,mode/binary/source separation PASS.
Receiver v6 SHA256c0e18e29413dd720c70f3779f25edaee9513253235a3bfdf0e5f8ad1f38bfcef,
flash61200/RAM16128,240 blocks,payload0x26000..0x34FFF. Full regression hashes in
context/ignored receiver-v6-software-validation.json. Default HID unchanged.
Board still v5 expired/passive;COM12 closed,no live sessions. V6 NOT FLASHED.
PAUSED for user break;next verified v6 flash/passive readiness after return,
then single-arm95s input test. Submission counts do not prove Windows delivery;
no v6 input success/latency claim. Do not silently reuse v5 consumed arm.

## V5 comparison inconclusive: ACK cap exhausted before usable input

Ignored `build-pairing-analysis/receiver-v5-comparison-20261115-142958.txt`,
`.host.jsonl`,`.summary.json`.95s single-arm capture,one link,8192 ACK budget
exhausted,neutral-complete stop,no ERROR/USB busy. ACK1 at315034ms and first
observed ACK8192 at324753ms:9719ms counter-rise span,not exact active duration.
Two completed timing SAMPLES (discovery/first ACK).49 cumulative stats lines.
User reports missed/could-not-perform cue,no pointer/button response and no
usable control. Do not claim improved latency or v5 HID success. Quiet counters
cannot identify ACKed packet types or USB report submission. Fast idle ACKs
could exhaust budget before user action;this is a hypothesis,not established
cause. Board v5 expired/passive,COM12 closed. User requested next software-only
diagnostic step then break: ACK-type/HID-submit/retry counters and finite65536
ACK cap while retaining60s deadline/recovery and unchanged RF/USB mapping.

## V5 quiet-diagnostics comparison in progress

V4 recovery milestone pushed78315c9. Tested v4 preserved at ignored
`build-pairing-analysis/receiver-v4-2162a852.uf2`. Branch
`feature/receiver-quiet-diagnostics`,plan `docs/plans/2026-11-15-receiver-quiet-diagnostics.md`.
V5 receiver omits raw RX/per-ACK text,emits cumulative stats at most once/second
on valid RX,and retains discovery/first-ACK timing samples per link plus link,
busy and error logs. Per-packet verbose probe/radio behavior stays unchanged.
Native assertions failed before implementation,then all8 passed. Receiver/probe/
radio builds and UF2 bounds pass;compiled-image/config/source separation confirms
per-packet formats removed only from receiver. Receiver v5 SHA256
ab8e3001d0a075a2e2480379894f868e9987000a3752965e41bc93984c6dbf2d,
flash60904/RAM16128,238 blocks,payload0x26000..0x34DFF. Probe hash2676573f...
and radio a7e6609c...;full hashes/state in context. Default HID unchanged.
Hardware still tested v4 expired/passive,COM12 closed. Pause for physical
flash/readiness;no new armed trial after14:26 for user's14:30 departure.
This only tests a possible logging contribution to reported delay;no latency
improvement claim yet. V5 timing lines are SAMPLES,not a total TX-completion count.
V5 subsequently flashed around14:25 Manila after user NICENANO readiness;
reverified bootloader0.6.0/nicenano/S1406.1.1 and ab8e3001... UF2 hash/bounds.
Five-second DTR COM12 passive log passed:v5 limits,1MHz timing,19channels,six
zero scans,no errors/arming. Windows composite/CDC/HID/Input statuses OK.
Ignored `build-pairing-analysis/receiver-v5-passive-20261115-142507.txt` and
`.summary.json`. COM12 closed;v5 fresh/unarmed,budgets unused. PAUSED for user's
14:30 departure;no input test started. After return,separate readiness/one95s
capture for perceived delay comparison and recovery. Do not repeat flash/baseline.


## V4 power-cycle recovery and reported pointer delay (2026-11-15)

95s continuous/drained single-arm capture ignored at
`build-pairing-analysis/receiver-v4-recovery-20261115-140838.txt`,`.host.jsonl`,
`.summary.json`. Two links,3348 ACKs,2310 unique type1 reports,28 type1 retries,
3350 completed TXs,no ERROR/USB busy. First link1586 ACKs then neutral-complete
gap stop;second discovery starts at ACK1586 with28632ms left,uses1762 more ACKs,
then neutral-complete final stop. User confirms all controls before/after mouse
OFF3s/ON with receiver untouched,and final stop without stuck buttons.
User also reports visible motion-to-pointer delay. Both links have16ms median
unique-report gaps and360ms maximum (mixed inputs/idle included),about44Hz
overall unique-report cadence including pauses. TX handler/READY/END ranges
20..487/151..618/196..663us include discovery. None measures end-to-end latency.
Do not infer the cause yet. Next comparison reduces receiver per-packet text
while preserving RF bytes/timing,shared budgets/recovery and USB semantics.
Board v4 expired/passive;COM12 closed. No automatic rearm.

## Receiver v4 software work (2026-11-15; hardware still v3)

Plan: `docs/plans/2026-11-15-bounded-link-recovery.md`;branch
`feature/bounded-link-recovery`. Receiver-only60s session from first successful
discovery,8192 total ACK attempts,eight total discovery attempts. After1000ms
without successful ACK,disable radio and finish neutral USB release before
scanning for the same mouse's observed discovery. Rediscovery never resets
deadline/budgets. Successful new discovery resets report retry marker;idle/status
still never produce input. RF bytes,identity/CRC/packet filters and delays unchanged.
USB reset/disconnect stays fatal/reboot-required. Initial30s arm remains once/boot.
Native actual-header tests pass,including gap boundary,idle refresh,busy not
refreshing,failed neutral preventing rediscovery,shared budgets/deadline across
links,discovery exhaustion/TX failure and new-link same-sequence delivery.
All8 tests,receiver/probe/radio builds,UF2 bounds and binary/config/source
separation checks pass. Default HID hash unchanged;tested v3 preserved4bc43f35...
Receiver v4 SHA2562162a852cb11c8a6c5b26630cdcaa1456390cdb56223dc2c447c38b727362ff3,
flash60968/RAM16064 bytes,239 blocks,application payload0x26000..0x34EFF.
Probe v9 regression SHA25616cb6a37d503d09846616a176ad0af6817ca5e38a735842650ffe4f9068072c4;
radio SHA256466aa22444354b85d1749e312ec0b40e59a4dc17fcfe45a63f8fad4902695a47.
No hardware flash or v4 recovery claim. Next physical flash/passive baseline,
then separate readiness/single-arm95s capture for sustained controls/power-cycle
recovery. New 'RECEIVER armed:'/'link started:'/'link stopped:' logs identify
session phases;reset analysis dedup at each link. Exact next steps in context.
The1000ms threshold is experimental,not measured stock protocol timing.

V4 subsequently flashed after user confirmed NICENANO/mouse wired or OFF.
Bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and final2162a852... UF2 hash/bounds
reverified before application copy. COM12/HID/composite statuses OK. Five-second
DTR passive log passed:v4 limits,1MHz timing,19channels,six zero-counter scans,
no errors/arming. Ignored `build-pairing-analysis/receiver-v4-passive-20261115-140431.txt`
and `.summary.json`. COM12 closed;v4 fresh/unarmed,budgets unused. Next separate
input readiness/single-arm95s recovery capture;do not repeat flash/passive check.

---

Date: 2026-11-14 (Asia/Manila).

## Input and method

Input is the user's successfully read mouse application, not a substitute firmware:
`build-pairing-analysis/mouse-app-6000-1cc9f.bin` (ignored).
SHA256: `965c8f8cc17e912d0ede5f0ed82b4c12a15d21fae931aeaf59921c94794e1738`.
GNU ARM objdump from Zephyr SDK 0.17.2 disassembled it as Thumb code at base 0x6000.
Literal pools and tables were read as data, not interpreted as instructions.

Register meanings were cross-checked against Nordic's nRF52840 product specification
and the installed Nordic MDK headers/SVD:
https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/radio.html

## Static evidence

The routine at `0x15A1C` initializes RADIO. A call at `0x19B84` occurs on the path
next to the `USB power removed` string. This supports testing it for battery-mode
radio traffic; it does not prove every later state keeps the same settings.

| Register | Observed value | Code/evidence |
| --- | --- | --- |
| MODE | 4 (BLE 2 Mbit/s modulation) | 0x15A50..0x15A52 |
| PCNF0 | 0x00000008 | 0x15AA2..0x15AA6 |
| PCNF1 | 0x03040040 | store 0x15AAC, literal 0x15B68 |
| PREFIX0 | 0xB3B2B1B0 | store 0x15A78, literal 0x15B5C |
| PREFIX1 | 0xB7B6B5B4 | store 0x15A7E, literal 0x15B60 |
| BASE0 | 0x01234567 | store 0x15A86, literal 0x15B64 |
| BASE1 | FICR DEVICEID[0] initially | 0x15A88..0x15A8E |
| TXADDRESS | 1 | 0x15A94 |
| RXADDRESSES | 2 | 0x15A9A |
| CRCCNF | 2 | 0x15AB2 |
| CRCINIT | 0xFFFF for two-byte CRC | 0x15AD0..0x15AD4 |
| CRCPOLY | 0x00011021 | 0x15AD6..0x15AE2, literal 0x15B6C |

PCNF1 specifies maximum payload 64, four base-address bytes plus a prefix,
MSB-first packet fields, and whitening enabled. PCNF0 specifies an 8-bit length
field and no S0/S1 field. The observed PCNF0 leaves PLEN at zero even though MODE
is 4; the first logger deliberately reproduces the observed raw value rather
than replacing it with a conventional BLE packet format. BLE modulation here
does not establish a standard Bluetooth connection or packet protocol.

The routine at `0x983C` copies 16 channel bytes from `0x1A966` into a RAM table.
The path at `0x15F4C..0x15F5A` uses the RAM table for RADIO.FREQUENCY:

```text
51 27 76  4
48 24 73  8
52 26 77  2
47 22 72  7
```

These are offsets from 2400 MHz. Group selection and hop timing are not yet
implemented or fully recovered. Scanning all entries is a reception experiment.
`0x134B2..0x134B6` also writes `0x89ABCDEF` to BASE1 on another state path.

## Logger choices and hypotheses

Source: `firmware/radio_logger/`. It is separate from the working HID prototype.

- Receive-only: starts TASKS_RXEN; no TXEN, ACK, pairing, or HID movement code.
- Uses MODE/PCNF/CRC values above and scans each listed channel for 40 ms.
  Logger v2 prepends discovery channels 3, 49, 78 to the 16-channel table.
- DATAWHITEIV is set to the Nordic reset value 0x40; no alternative seed has
  been established for this path. This assumption needs over-air validation.
- Logical address 0 listens on the observed common BASE0/prefix B0.
- Logical address 1 alternates between the queried four-byte mouse address
  interpreted little-endian (prefix B1) and observed BASE1 0x89ABCDEF (prefix B1).
  The first capture validated reception with the mouse-address mapping; no packets
  were recorded with the alternate pairing base.
- CRC-valid packets are emitted as raw hex; failed CRC and oversize reports are counted.
- It waits for a USB serial connection with DTR enabled before scanning.
- Board LED is initially on, then toggles after each complete scan.
- Polling and text logging intentionally miss traffic. This is not a packet-loss,
  latency, throughput, or 1000 Hz measurement tool.
- No CRC-valid packets does not prove the mouse is silent or incompatible.
  It may need a different address/seed/state or an acknowledgment before sending movement.

`generate_radio_header.py` verifies the exact firmware hash and obtains literal
parameters and channel bytes from the image. It validates the recorded USB identity
and address before emitting a private header only into the staged build source.
No device-specific address or vendor binary is added to tracked firmware source.

## Hardware procedure

Build verified on 2026-11-14: five Python tests passed; 56,688 bytes flash,
15,872 bytes RAM. UF2 contains 222 blocks (113,664 bytes) and writes only
`0x26000..0x33DFF`. SHA256:
`9daaa154dd51db6a6d77d2784ba38c400b6f79a2e3196a07f471cf2bede565fd`.
The radio and USB use the same Zephyr HFXO on/off manager.

1. Build with `firmware/build.ps1 -App radio`; use only its validated UF2.
2. Double-reset the nice!nano into NICENANO and copy the new radio logger UF2.
3. Keep the Glorious mouse wired initially so it remains usable for setup.
4. Have Codex identify/open the logger's serial port with DTR enabled and save its output.
5. During the capture, unplug the Glorious mouse's USB cable, switch it on,
   place it near the board, and move/click it for the requested interval.
6. Reconnect the Glorious mouse afterward. Save capture output locally under an ignored build folder.

The logger product is `DIY Mouse RX Logger`, local test USB ID `2FE3:0001`.
It appears as a serial device, not a HID mouse.

Hardware baseline on 2026-11-14: flashed the hash-verified UF2 after verifying
NICENANO bootloader identity. Windows enumerated COM11 (2FE3:0001, MI_00).
Opening with DTR enabled produced the expected banner and radio register settings,
followed by 12 SCAN lines over eight seconds, all counters zero and no errors.
The mouse was still wired during this baseline.
Evidence: ignored `build-pairing-analysis/radio-baseline-20261114-121111.txt`.
The earlier HID UF2 remains available in `C:\Users\Gabriel\zephyrproject\build-mouse-hid\zephyr\zephyr.uf2`.

## First battery-powered capture

User confirmed mouse unplugged and switched on, and was asked to move/click it.
Saved 30-second capture: ignored
`build-pairing-analysis/radio-wireless-20261114-121312.txt` and `.summary.json`.

- 595 CRC-valid packets, all logical address 1 with the mouse BASE1 candidate.
- All payloads: byte `0x20` followed by the four address bytes read over USB.
  Exact device-specific values remain in ignored evidence.
- Logger frequencies/counts: 4/149, 48/151, 77/147, 2/147, 47/1.
- Packet timestamps span 243430..273198 ms since board startup.
- Counters are cumulative, including scanning while the serial port was closed:
  first SCAN good=804/bad_crc=1/invalid_len=0, last SCAN 1384/1/2.
  These totals are not the capture packet count; final RX lines follow the last SCAN.
- No movement-varying payload or receiver response was captured. No transmission
  was made by the board. This validates reception, not pairing or pointer control.

## Discovery state traced after capture

The DISABLED handler calls `0x15EA8` from `0x6722`. Its state byte is at
`0x20003620`; the table at `0x15EE0` dispatches state 4 to `0x15F66`.
That path selects a frequency from the three-byte table at `0x1A950`:
**3, 49, 78** (2403, 2449, 2478 MHz). It constructs length=5, payload byte
0x20, then copies FICR DEVICEID[0] into the remaining four bytes at
`0x15F70..0x15F84`. This matches the captured payload structure.

The original scan omitted these exact discovery frequencies. Adjacent-channel
reception is a plausible explanation for the frequencies logged above; a new
capture on the exact frequencies is needed before assigning transmit channels.

State 5 (`0x15FA4`) switches PACKETPTR to the receive buffer and starts RX,
with timeout derived from state offset 32 times four. State 6 (`0x15FAE`)
checks CRCSTATUS; when receive length is 3 it reads a channel-group byte and
a little-endian 16-bit value into state offsets 23 and 44. It then regenerates
the channel table and moves toward normal operation. This is static evidence
for a discovery response, not a validated response recipe: timing, accepted
values, and subsequent acknowledgments still require investigation.

Logger v2 only adds the three discovery frequencies. Six Python tests and the
build/UF2 bounds check pass. Flash 56,704 bytes; RAM 15,872 bytes; 222 UF2 blocks,
payload range 0x26000..0x33DFF. SHA256:
`d5122f0ad3b6061c6db82be77b0c661d04c1c333b964595d3e3af83c34417882`.
Built at the same radio UF2 path above. Flashed after verifying the bootloader
identity and UF2 hash. COM11 returned; five-second baseline confirmed the v2
banner, 19 frequencies, and six SCAN lines with all counters zero and no errors.
Evidence: `build-pairing-analysis/radio-v2-baseline-20261114-121813.txt` (ignored).
Battery-powered v2 capture completed:
`build-pairing-analysis/radio-v2-wireless-20261114-122244.txt` and `.summary.json`.
855 valid packets over timestamps 294555..324531 ms, all mouse-base/address 1,
all the same 0x20 plus four address bytes. Channel counts: 3=123, 49=123,
78=119, 4=119, 48=120, 77=125, 2=126. Reception on the three exact discovery
frequencies is confirmed; adjacent-channel reception also occurs. First/last
SCAN totals were 1091/6/1 and 1923/8/1 (good/bad_crc/invalid_len), cumulative
since boot and not per-capture totals. User was told to reconnect the mouse cable.

## One-shot discovery probe v1 (hardware tested)

`firmware/build.ps1 -App probe` builds a separate artifact in `build-mouse-probe`.
The `radio` build remains receive-only; the probe's TX code is compile-time gated.
The probe remains passive until serial ASCII `p` arms a 30-second window, with
one transmission maximum per boot and no rearming. It only responds to a
CRC-valid, exact-length 0x20/address payload, matched on logical address 1,
mouse BASE1, and one of the three exact discovery frequencies.

The test response is length 3, payload `00 F4 01`: channel group 0 and timing
value 500, chosen from the stock channel-group indexing and initial 500 value.
This is an inferred candidate, NOT a captured original-receiver response.
It transmits at 0 dBm after an experimental 250 us software delay, then listens
on group zero's first frequency (51) for two seconds, before resuming passive scans.
There are no subsequent ACKs or HID reports, so a transient transition is the
expected observable result, not a working receiver. Absence of a transition
would not disprove the response layout: turnaround/scheduling may be wrong.

No persistent mouse configuration writes are implemented. A mouse power cycle
is the recovery step if the experiment leaves it in a temporary connection state.
Transmit completion is bounded; a timeout consumes the one-shot budget too.
The native C harness tests the actual packet matcher, explicit arming, inability
to extend/rearm, transmit timeout, receive-buffer restoration, and no retry.
Seven tests pass. Hardware assertions also exercise the packet matcher at startup.

Final probe build passes (2026-11-14): 57,564 bytes flash, 15,936 bytes RAM,
225 UF2 blocks, application payload 0x26000..0x340FF. Path:
`C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`.
SHA256: `6e66aba83badd6014ca26d1dd8a10e374f3df4904983b7e681fe79a96f47d69b`.
The first intermediate probe build is superseded.
Receive-only regression build also passes with DISCOVERY_PROBE=OFF, while the
probe cache is ON. Binary banner/arming-string checks confirm the builds are
distinct. Rebuilt receive-only UF2 SHA256:
`b12e5867f7b4f36a68827e5905a5b0cbce4f824ea468abd43b61045266197559`.
Probe subsequently flashed after verifying bootloader identity and its final hash.
COM11 returned. Passive startup check passed with the expected probe banner,
19 frequencies, and six zero-counter SCAN lines over five seconds; no errors or
assertions. Evidence: ignored
`build-pairing-analysis/probe-passive-baseline-20261114-181929.txt`.
After battery-mode readiness, sent serial `p` once and captured 35 seconds in
`build-pairing-analysis/probe-armed-20261114-182039.txt` and `.summary.json`.
Exactly one ARMED and one discovery-SENT line, no ERROR lines. Of 990 captured
CRC-valid packets, 31 were new eleven-byte packets on channel 51:
`00 00 41 00 00 01 00 00 00 00 00` (timestamps 93111..93352 ms).
The remaining 959 were discovery packets. The first subsequent discovery was
seen at 94074 ms. This supports acceptance of the discovery response and a
temporary transition to data traffic; it does not establish a sustained link.
The probe has consumed its transmission budget and is passive until reboot.
User was told to reconnect the mouse cable.

## Probe v2: one bounded data acknowledgment

Stock `0x18938..0x18974` prefixes normal outgoing data with two sequence bytes.
The tag builder at `0x15BA0` encodes type in bits 0..2, size in bits 3..6,
and continuation in bit 7. Tag 0x41 is type 1, size 8, no continuation.
The type-1 eight-byte report is constructed at `0x14D20..0x14D66` and passed
to the radio queue at `0x14DBE..0x14DC8`; the wired path sends the same eight
bytes at `0x14D7A..0x14D82`. The observed report was repeated unchanged;
movement/click interpretation is not yet empirically validated.

Stock `0x16088..0x160B8` releases the queued outgoing report when the received
first payload byte equals outgoing sequence+1 (eight-bit wrap). A receive
length below 3 skips the incoming application-message branch at `0x16044`.
The next probe therefore sends one length-2 ACK with payload
`[outgoing_sequence + 1, peer_sequence & 0x3F]`. High bits are cleared because
`0x160CC..0x160D2` interprets them as scheduling flags. This ACK is inferred
from the mouse code, not yet tested over air.

Probe v2 keeps the proven discovery response, then permits exactly one data ACK
within two seconds, only for a CRC-valid eleven-byte/type-1 report on channel 51
and the mouse address. At most two transmissions per boot total, with no automatic
rearm or retries. Both use the existing bounded TX path and 250 us delay.
It logs the acknowledged sequence and keeps listening for a changed sequence or
report. No Windows HID output or continuous ACK loop is implemented.
The C harness now also checks report filtering, sequence wrap, cleared scheduling
flags, ACK expiry, pre-discovery rejection, timeout consumption, and no repeat ACK.

V2 build and seven tests pass. Flash 57,960 bytes, RAM 15,936 bytes; 227 blocks,
payload range 0x26000..0x342FF. Current probe UF2 (same path above) SHA256:
`be6109e58a548ed6104f257d4bf35d950ab11ad45db1079b40287935a644c3f1`.
V2 subsequently flashed after verifying bootloader identity and the be6109...
hash. COM11 returned. Five-second passive startup confirmed v2 banner, 19
channels and six zero-counter SCAN lines without errors. No arming command sent.
Evidence: `build-pairing-analysis/probe-v2-passive-20261114-182615.txt` (ignored).
V2 armed/capture completed in ignored
`build-pairing-analysis/probe-v2-armed-20261114-182719.txt` and `.summary.json`.
One ARMED, one discovery SENT, one ACK SENT (seq=0,next=1), no errors.
974 CRC-valid packets: 31 repeated `00 00 41 00 00 01 00 FF FF 00 00` on
channel 51 and 943 discovery packets. No sequence advance or changed data
within this capture, so ACK acceptance was NOT demonstrated. The type-1 payload
differs from v1 at the second 16-bit field (FFFF vs 0000), consistent with a
movement report but not yet a controlled axis/button validation.
User was told to reconnect mouse cable. Board v2 budgets are consumed/passive.

## Probe v3: shorter data-ACK turnaround

V2 applied the successful discovery turnaround delay (250 us) to the data ACK
as well. Normal data uses a shorter timer-driven exchange than discovery; the
extra delay plus TX ramp-up may miss its RX window. Additionally, logging the
discovery transition after enabling RX could leave the first received packet
waiting while USB output completes, making a subsequent ACK stale.

V3 keeps discovery delay 250 us but removes the added data-ACK delay. It logs
the transition before starting channel-51 RX. The same two-transmission limit,
exact packet filters, expiry, and no-retry guards remain. The native test checks
the separate discovery and ACK delay arguments. This is a timing hypothesis to
test; the negative v2 result does not prove the ACK payload is correct or wrong.

V3 build and seven tests pass: flash 57,936 bytes, RAM 15,936 bytes, 227 UF2
blocks, payload 0x26000..0x342FF. Current probe artifact SHA256:
`1df362581ae875af62762513bb6c0022058de179b263c0844586bd496c99966f`.
V3 flashed after verifying bootloader identity and the 1df362... hash. COM11
returned. Five-second passive baseline passed: expected v3 banner, 19 channels,
six zero-counter SCAN lines, no errors. No arming command sent.
Evidence: `build-pairing-analysis/probe-v3-passive-20261114-183203.txt` (ignored).
V3 test completed: ignored `build-pairing-analysis/probe-v3-armed-20261114-183305.txt`
and summary JSON. One discovery SENT, one ACK SENT seq0->1, no errors. 975 valid
packets: 31 identical type-1 reports (`00 00 41 00 00 01 00 FF FF 00 00`),
944 discovery packets. No sequence advance observed. Thus removing the delay
did not demonstrate ACK acceptance. User was told to reconnect the mouse cable.

## V4 hardware timing instrumentation / handoff

V4 retains V3 response bytes, delays and two-transmission limit. New
`probe_timing.h` configures unclaimed TIMER2 at 1 MHz and dynamically allocates
two PPI channels through nrfx (without taking channels from the USB driver).
RADIO.END captures CC0; RADIO.READY captures CC1. Before TX the RX END timestamp
is saved and software age sampled; after TX a TIMING line reports handler age,
TX READY and TX END offsets relative to RX END, plus MODECNF0. Hardware startup
checks timer progress; setup failure stops the app without arming/transmission.
TIMER2 must remain disabled in devicetree and unused by nrfx's timer driver.

All seven tests and the V4 build/UF2 checks pass. Timing hardware is not yet tested;
the native protocol harness stubs the timing helpers. Flash58,680 / RAM15,936
bytes; 230 blocks; payload0x26000..0x345FF. Current probe UF2 SHA256:
`2bef74865e0f59502cdb1d6c7117708ddd5efd6c14ca693c8d3e0dfbed8309e3`.
Path remains `C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`.
V4 flashed after verifying NICENANO bootloader0.6.0, S1406.1.1 and the hash above.
Five-second DTR-enabled COM11 passive baseline passed: V4 banner, `TIMING hardware
capture initialized at 1 MHz`, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v4-passive-20261114-232306.txt`.
No `p` sent; both TX budgets unused. Serial port closed. Mouse last confirmed wired.
This baseline state was superseded by the capture below.

## V4 armed capture: first observed sequence advance

User confirmed battery readiness; one `p` sent and35seconds captured. Evidence:
ignored `build-pairing-analysis/probe-v4-armed-20261114-232436.txt` and summary JSON.
Exactly one arm, discovery TX, and data ACK TX; no errors.991 CRC-valid packets:
960 discovery and31 eleven-byte reports on channel51. Initial report at t119735:
`00 00 41 00 00 01 00 00 00 00 00`. Subsequent30 reports at t119749..119974:
`01 00 41 00 00 00 00 FF FF 00 00`. Sequence0->1 and changed payload support
acceptance of the one-shot ACK. This is the first positive ACK observation;
V2/V3 negative observations remain valid for their captures.

| Response | RX END to handler (us) | RX END to TX READY (us) | RX END to TX END (us) |
| --- | --- | --- | --- |
| Discovery | 19 | 401 | 449 |
| Data ACK | 63 | 194 | 239 |

Both completed=1; MODECNF0=0x200. Hardware timing capture works. These measurements
describe this exchange, not a proven timing window. V4 timing instrumentation
changes execution compared with V3; the reason for different outcomes is not isolated.
No second data ACK was permitted, so repeated sequence1 and eventual discovery
do not establish sustained connectivity or measured mouse report throughput.
Movement/button interpretation remains unverified.

Board V4 budgets consumed/passive until reboot; COM11 closed. User told to
reconnect cable, physical reconnection not confirmed. Next: smallest bounded
multi-ACK diagnostic with explicit arm, finite time/packet budget, preserved
timing/response bytes and native guard tests. Check stock channel/scheduling
behavior before assuming a fixed channel. No HID output yet.
Do not repeat earlier reads/captures on resume.

## V5: bounded multi-ACK diagnostic

V5 retains V4 radio initialization, hardware timing capture, discovery bytes,
250us discovery delay and immediate data ACK bytes. It allows at most16 data ACK
attempts in the existing two-second post-discovery window, in addition to one
discovery TX. One30-second arm per boot; no deadline extension or automatic rearm.
Every attempt consumes budget; any TX failure exhausts the remainder and stops
the app. CRC-valid exact eleven-byte/type1 reports must still match this mouse's
base, logical address1 and channel51. Repeated packets may be acknowledged again
within the budget because an over-air ACK can be lost. No persistent writes or HID.

Static check: `0x983C` copies the fixed16-channel table, ignoring its passed
DEVICEID argument. `0x15FDA..0x15FE8` selects group*4 on discovery transition;
`0x16088..0x160B8` applies sequence+1 release and `0x160CC..0x160D2` copies
scheduling flags from the second ACK byte. V4 observed both sequence0 and1 on51.
These observations support a bounded same-channel follow-up, not a complete hop
schedule. V5 deliberately retains channel51 and cleared scheduling flags.
Polling/USB logging still misses traffic; do not interpret this as a throughput test.

All seven tests pass with native checks for finite count, fixed deadline,
sequence wrap/flag masking, repeated-packet ACKs, timeout exhaustion and no rearm.
V5 build/UF2 checks pass: flash58,716/RAM15,936 bytes,230blocks,
payload0x26000..0x345FF. Current `build-mouse-probe/zephyr/zephyr.uf2` SHA256:
`a1b78c57b5010fb7d7de5e7eb997e1f96e012de0040ee4455b4e1abae49031fe`.
Preserved V4 artifact: ignored `build-pairing-analysis/probe-v4-2bef7486.uf2`.
Receive-only regression build passes: flash56,704/RAM15,872 bytes,222blocks,
payload0x26000..0x33DFF; new radio UF2 SHA256:
`8a19a4c22319ddb93fd8f2e49eabb6dd6548f973a8eed519cda28a0b188f3339`.
CMake flags and binary banner/arming/ACK string checks confirm compile separation.
V5 flashed after user confirmed NICENANO ready/mouse wired, verifying bootloader
0.6.0,board-ID nRF52840-nicenano,S1406.1.1 and a1b78c57... UF2 hash.
Five-second DTR-enabled COM11 passive baseline passed: expected V5 banner,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/probe-v5-passive-20261114-233754.txt`.
No `p` sent; discovery and16 ACK budgets unused. COM11 closed, mouse last wired.
Next: await battery readiness, then one35-second armed capture. Multi-ACK behavior
is not yet hardware tested. Do not repeat flash/baseline on resume.

## V5 armed capture: multi-ACK acceptance and compound packet

After user battery readiness, one arm/35-second capture saved in ignored
`build-pairing-analysis/probe-v5-armed-20261114-233953.txt` and summary JSON.
1003 CRC-valid packets:958 discovery and45 data packets on51. First15 data packets
were length11,sequence0..14 once each at139159..139383ms,16ms apart. Fifteen ACKs
sent(seq0->1 through14->15),no errors. This supports repeated ACK acceptance.
Do not infer report throughput from this polling/USB-text experiment.

Next30 packets were length20,sequence15,unchanged payload:
`0f 00 41 00 00 00 00 ff ff 00 00 45 06 01 00 00 00 00 00 00`,
at139397..139623ms. Two sequence bytes plus tag41/eight-byte type1 and
tag45/eight-byte type5 match the tag construction at0x15BA0 and message appending
at0x15E5C..0x15E92. Type5 meaning remains unknown. V5 rejected length20 by design,
so its unused16th ACK and return to discovery are explained by the packet filter.
This is not evidence that the15th ACK failed: sequence15 was observed.

Timing(handler/READY/END relative to RX END,us): discovery42/424/472; data ACK
ranges24..70/155..201/199..245. All16 transmissions completed,MODECNF0=0x200.
Board V5 discovery consumed,15 ACKs attempted,ACK window expired/passive. COM11
closed. User told to reconnect cable; reconnection unconfirmed.

Next: V6 narrowly accepts the observed length20/tag41+45 shape in addition to
length11/tag41, retaining16-attempt/two-second bounds and V4 timing/response bytes.
No general packet parser, type5 command handling, HID output or extra TX budget.

## V6 preparation

The matcher now accepts length 11/tag 0x41 or length 20/tag 0x41+0x45 at the
observed offsets. Address, channel, CRC and all arming/budget/deadline guards are
unchanged. The tag 0x45 block is not interpreted. Native tests use the captured
compound shape and reject every unsupported length 0..64, wrong tags and wrong
channel/base/logical address. All seven tests pass.

V6 probe build and UF2 validation pass: flash 58,740 / RAM 15,936 bytes;
230 blocks, payload 0x26000..0x345FF. Current probe UF2 SHA256:
`dbf0b69cd0e4d61656d3bb6ef61b4782783d97c80b3293222d881a2de8c66020`.
V5 preserved locally at ignored `build-pairing-analysis/probe-v5-a1b78c57.uf2`.
Receive-only regression build and compile flag/binary string
separation checks pass. Radio UF2 hash remains 8a19a4c2... above.
V6 flashed after user confirmed NICENANO ready/mouse wired, rechecking bootloader
0.6.0, nRF52840-nicenano, S140 6.1.1 and dbf0b69c... UF2 hash/bounds.
Five-second DTR-enabled COM11 baseline passed: V6 banner, 1 MHz timing capture
initialization, 19 channels and six zero-counter SCAN lines; no errors, no arming.
Evidence: ignored `build-pairing-analysis/probe-v6-passive-20261114-234710.txt`.
Board V6 passive/unarmed, all TX budgets unused. COM11 closed; mouse last wired.
Next: battery readiness, one armed 35-second capture; check compound ACK and
sequence advance past 15. Do not repeat flash/baseline.

## V6 capture: compound ACK acceptance

After battery readiness, captured once for 35 seconds in ignored
`build-pairing-analysis/probe-v6-armed-20261114-234923.txt` and summary JSON.
1004 valid RX: 958 discovery, 46 data on channel 51. Sequences 0..15 appeared
once each, then sequence 16 repeated 30 times. Exactly 16 ACKs and one discovery
TX completed with no errors. The sole length-20 packet was sequence 15:
`0f00410000ffffffff0000450601000000000000`; all other data were length 11.
Sequence 16 confirms acceptance of the compound packet ACK. The subsequent
repetition is expected after exhausting the ACK budget. Data span 153381..153860 ms.
Final repeated payload `1000410100bdfb4e130000` has changed type-1 bytes, but no
controlled button/axis decoding is claimed.

Timing (handler/READY/END after RX END, us): discovery 18/399/447; data ACK ranges
13..72/144..203/189..248. All 17 completed, MODECNF0=0x200. Board V6 consumed/passive;
COM11 closed, user told to reconnect cable, reconnection unconfirmed.
Next diagnostic: V7 raises ACK cap to 256, retaining the two-second deadline and
observed packet filters. No continuous/unlimited TX, HID, or timing changes.

## V7 preparation and candidate movement layout

V7 raises only the ACK cap to 256 within the unchanged two-second deadline;
banner prints that constant. Native tests check the finite cap, expiry after
partial use, wrap, timeout exhaustion and existing exact packet filters. All
seven tests and probe build/UF2 validation pass: flash 58,732 / RAM 15,936 bytes,
230 blocks, payload 0x26000..0x345FF. Current probe UF2 SHA256:
`e9e79cce535839d72fc4ca338ee1fc85a62f8b9ee68194af6139deaad69c8cee`.
V6 artifact preserved locally at ignored `build-pairing-analysis/probe-v6-dbf0b69c.uf2`.
V7 not flashed/armed; receive-only regression and binary separation checks pass.
Radio: flash 56,704 / RAM 15,872 bytes, 222 blocks, payload 0x26000..0x33DFF;
new UF2 SHA256 `3b459262846fbae3f30ded047c5a768bc9e44ad7bf2490fdbddfee85c6f584d1`.
V7 flashed after user confirmed NICENANO ready/mouse wired, checking bootloader
0.6.0, nRF52840-nicenano, S140 6.1.1 and e9e79cce... UF2 hash/bounds.
Five-second DTR-enabled COM11 passive baseline passed: V7 banner with256-ACK/
two-second limit, 1 MHz timing initialization,19 channels,six zero-counter SCAN
lines,no errors. Evidence: ignored
`build-pairing-analysis/probe-v7-passive-20261114-235645.txt`.
No `p` sent. Board V7 passive/unarmed, all budgets unused. COM11 closed; mouse
last wired. Next: battery readiness, then one35-second armed capture. Check the
ACK window separately from post-window RX traffic. Do not repeat flash/baseline.

The hash-checked snapshot contains a 91-byte HID mouse report descriptor at
0x1A750..0x1A7AA. At 0x1A7AC the length/type/pointer words are
`91, 0x22, 0x1A750`. This supports identifying it as descriptor data, rather than
interpreting another incidental byte-pattern match as a descriptor. The report
builder at 0x14D20..0x14D66 packs the same eight bytes for the wired USB send at
0x14D7A..0x14D82 and type1 RF queue at 0x14DBE..0x14DC8.

| Type1 report byte(s) | Descriptor field | Static encoding |
| --- | --- | --- |
| 0 | Buttons1..5, then padding | Five low bits, three padding bits |
| 1 | Generic Desktop Wheel | Signed8, relative, -127..127 |
| 2..3 | Generic Desktop X | Signed16 little-endian, relative |
| 4..5 | Generic Desktop Y | Signed16 little-endian, relative |
| 6 | Consumer usage0x238 | Signed8, relative, -127..127 |
| 7 | Consumer usage0 | Eight bits, logical0..1, relative; semantic role unresolved |

This table concerns the eight bytes after tag0x41, not the sequence/tag prefix.
There is no Report ID item in this descriptor. This is static evidence; verify
each axis/button/wheel with controlled actions before enabling Windows input.
No type5 meaning is inferred and no decoding/HID code was added.

## V7 capture: bounded two-second link

After user battery readiness, one arm/35-second capture saved as ignored
`build-pairing-analysis/probe-v7-armed-20261114-235814.txt` and summary JSON.
1059 CRC-valid packets:913 discovery,146 data (145 length11,one length20).
124 ACKs were sent(seq0..123),then sequence124 was received. No errors and all
125 transmissions completed. One compound report was successfully ACKed.
ACKed RX timestamps112128..114097 span1969ms, consistent with the two-second
deadline ending below the256 cap. The exact discovery/ACK-window start is not
timestamped; these RX times alone are not an exact deadline measurement.

Channel51 had130 data packets:seq0..123 once each,then seq124 six times.
Another16 copies of seq124 appeared at114135..114326 on logged frequencies
27(4),76(6),4(2),49(2),78(2). The first three match group0's retry channels;
49/78 may be adjacent-channel reception. This supports retry/channel changes
after ACKs stop, not a complete receiver hopping/recovery implementation.
Stock0x1889A..0x188DC increments retry/channel indices after unsuccessful
cycles and writes the table-selected frequency. No hopping code was added.

Hardware timing(handler/READY/END relative to RX END,us): discovery40/421/469;
data ACK ranges12..73/143..204/187..248. MODECNF0=0x200 throughout. ACKed RX gaps
were15..17ms; USB text/polling miss traffic, so this is not a throughput or1000Hz test.

Applying the descriptor layout offline to124 unique ACKed reports gives masks0/1,
wheel0,X range-43..412,Y-142..196,and both consumer fields0. Movement/click actions
were uncontrolled; these ranges support further testing but do not validate axes.
Board V7 discovery consumed,124 ACK attempts used,window expired/passive until
reboot. COM11 closed; user told to reconnect cable, reconnection unconfirmed.
Next: normal board reboot (no new firmware), passive startup, then a distinct
controlled battery capture holding left button and moving only right/lifting
to return. Verify candidate button/X mapping before USB pointer integration.

Controlled-test preparation on 2026-11-15: user confirmed normal V7 reboot/mouse
wired. Five-second DTR-enabled COM11 startup passed with expected V7 banner,
256-ACK/two-second limit,1 MHz timing initialization,19 channels,six zero-counter
SCAN lines,no errors and no arming. Evidence: ignored
`build-pairing-analysis/probe-v7-input-passive-20261115-000319.txt`.
Board V7 unarmed with fresh budgets; COM11 closed. Await battery readiness while
holding left button and moving only right/lifting to return, with no scrolling
or other buttons. This will be a distinct controlled capture, not another generic
link test. No source or firmware change.

## Controlled left-button/rightward test (2026-11-15)

User confirmed holding left button and moving only right, lifting to return,
with no wheel/other buttons. One arm/35-second capture saved as ignored
`build-pairing-analysis/probe-v7-left-right-20261115-000519.txt` and summary JSON.
1070 valid RX:924 discovery,146 data (145 length11,one length20).124 ACKs with
unique sequences0..123 over1968ms,no errors. Only unique ACKed reports are used
for candidate movement interpretation, avoiding duplicate motion from retries.

Button byte0 was0 for sequences0..15,then1 for all108 reports at16..123.
Wheel and consumer fields were0 throughout. All X values from16 onward were
positive, supporting left-click bit0 and the positive X direction. However:

- First16 reports were small negative/zero motion with button0; previously
  queued data is a plausible explanation, not established fact.
- Sequence16 contained X22768,Y7359. It is retained as an outlier/transition
  observation, not silently removed from the evidence.
- Sequences17..123 had X25..58,Y9..37; Y was substantial despite the requested
  right-only motion. Hand orientation/lift effects are possible, so this does
  not fully isolate axes or validate the Y sign.

These observations partly validate the descriptor interpretation; they do not
establish button release, other buttons, Y direction, wheel or latency. No USB
pointer output or firmware change. Board V7 window expired/passive,COM11 closed.
User told to stop/release/reconnect cable; reconnection unconfirmed. Next: normal
board reboot,passive startup,then separate battery readiness for right-button
hold and downward-only motion/lift to return. Preserve initial packets/outliers.

## Controlled right-button/downward test (2026-11-15)

After confirmed normal reboot, passive baseline saved as ignored
`build-pairing-analysis/probe-v7-right-down-passive-20261115-000859.txt` passed
with no arming/errors. User then confirmed right-button hold and downward-only
movement toward them/lifting to return, no wheel/other buttons.
Armed35-second capture: ignored
`build-pairing-analysis/probe-v7-right-down-20261115-001018.txt` and summary JSON.
1071 complete RX packets:925 discovery,146 data (145 length11,one length20).
One final discovery line was cut off at capture end; all124 ACKed reports and
125 TX timing lines were complete. No firmware errors; ACKed span1969ms.

Button byte0 was1 for sequences0..15,then2 for all108 reports at16..123.
The earlier left-button state strengthens the previously queued-report hypothesis.
Wheel/consumer fields0 throughout. Sequence16 X6927,Y-18490 is retained as a
large transition observation; accumulated motion/wrap is possible, not established.
The remaining107 reports(seq17..123) had X -9..21(abs total1042),Y -7..86
(abs total4813),with96 positive and11 negative Y samples. This supports right
button bit1 and downward-positive Y; nonzero X and small negative Y remain recorded.
No automatic prefix/outlier dropping was added to receiver code.

Board V7 window expired/passive; COM11 closed. User told to stop/release/reconnect
cable; reconnection unconfirmed. Next: normal reboot/passive baseline, then
stationary wheel-up-only battery test with no buttons, about one notch/second,
to compare wheel field/sign and released-button state. No new firmware.

## Controlled wheel-up test and idle packets (2026-11-15)

Fresh passive baseline: ignored
`build-pairing-analysis/probe-v7-wheel-up-passive-20261115-001422.txt` passed.
After user confirmed stationary/no buttons/slow wheel-up, one35-second capture:
ignored `build-pairing-analysis/probe-v7-wheel-up-20261115-001538.txt` and summary.
1001 complete valid RX:954 discovery,16 length11,one length20,30 length2.
17 ACKs(seq0..16),256ms ACKed RX span,no errors. First16 reports retained prior
button2/downward motion. Seq16 contained button0,wheel+19,X855,Y121,consumer
fields0. This supports released buttons and positive wheel-up byte1; only one
fresh wheel report was seen and accumulated motion remains, so retain the caveats.

Then30 identical length2 packets `11 00` (sequence17,peer0) arrived. V7 deliberately
rejected them. Stock0x18938..0x18950 constructs length2 with sequence bytes; the
queue-empty branch skips report append to0x18976. Stock0x16094..0x1609A branches
past outgoing-sequence increment when queue is empty. Thus repeated idle sequence
can be legitimate and must not be interpreted as fresh motion or ACK failure alone.
No idle ACK has been tested yet. V8 will accept only this additional observed
length2 form, using the existing sequence+1/cleared-flags ACK,256 cap/two-second
deadline and exact CRC/address/channel guards. No idle HID report is generated.
Rename matcher to is_ack_candidate; add native small-buffer/repeat/expiry tests
and incoming-length ACK logging. Board V7 window expired/passive;COM11 closed.
User told to stop/reconnect cable; reconnection unconfirmed.

## V8 preparation: bounded idle ACKs

Matcher renamed is_ack_candidate and now accepts length2 (no tag reads), or the
existing length11/tag41 and length20/tag41+45 shapes. Length checks precede tag
access. CRC/address/channel filters,ACK bytes/timing,256 cap/two-second deadline,
one arm and one discovery per boot are unchanged. No HID.
ACK log format is now `PROBE ACK sent: count=N/256 len=L seq=S next=T`.
Idle length2 never carries a type1 report; a subsequent real report can use the
same sequence, so future report deduplication must not consume sequences on idle.

Seven tests pass, including a true three-byte radio buffer(length+two payload),
wrong identity/channel,truncated/extended idle,repeated idle ACKs,expiry and
budget/timeout guards. Native assertions now exit noninteractively on Windows;
the new idle fixture failed cleanly before implementing the matcher change.
V8 build/UF2 checks pass:flash58,752/RAM15,936 bytes,230 blocks,
payload0x26000..0x345FF. Current probe UF2 SHA256:
`4ad89938b68c166ebb3b73fea46878b763cd97974c3f9424acc227f0e45824f4`.
V7 preserved locally at ignored `build-pairing-analysis/probe-v7-e9e79cce.uf2`.
Receive-only regression and binary separation pass. Radio UF2 hash remains
3b459262... above;flash56,704/RAM15,872,222 blocks.
V8 flashed after user confirmed NICENANO ready/mouse wired,checking bootloader
0.6.0,nRF52840-nicenano,S1406.1.1 and4ad89938... UF2 hash/bounds. Five-second
DTR-enabled COM11 passive baseline passed: V8 banner,256 cap/two-second window,
1 MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/probe-v8-passive-20261115-002350.txt`.
No `p` sent;board V8 passive/unarmed,all budgets unused. COM11 closed;mouse last
wired. Next: controlled wheel/idle capture after battery readiness; do not repeat
flash/baseline or earlier generic tests.

## V8 capture: idle ACK acceptance and fresh wheel reports

After readiness, one35-second controlled stationary/no-buttons/slow wheel-up
capture: ignored `build-pairing-analysis/probe-v8-wheel-up-20261115-002506.txt`
and summary JSON.1069 complete valid RX:928 discovery,18 length11,one length20,
122 length2.118 ACKs(19 reports,99 idle) over1973ms;no errors,all119 TXs complete.
Idle ACK sequence counts17:27,18:69,19:3. Data sequence17 at101965ms and18 at
103155ms each had button0,wheel+1,X0,Y0,consumer fields0. Each followed idle with
the same sequence, confirming report deduplication must ignore idle sequences.
Sequence16 had wheel+19,X1665,Y-528; retained as startup/accumulation observation.
The initial16 reports contained prior wheel-up data.

This supports idle ACK acceptance, resuming report traffic after idle, wheel-up
positive byte1, and released-button state. It does not establish a throughput,
latency, full-recovery or1000Hz result. Board V8 window expired/passive;COM11
closed. User told to stop/reconnect cable; reconnection unconfirmed.
Next: separate bounded composite CDC+HID prototype using the same V8 RF behavior,
real-report deduplication, USB backpressure and button release on stop/error.

## Bounded wireless HID bridge prepared (2026-11-15)

Separate `-App receiver` uses the V8 protocol/timing/256-ACK/two-second window,
with composite CDC+HID (local2FE3:0008,product DIY Mouse RX HID Probe). Only
validated type1 packets forward five buttons,signed16 LE X/Y,and signed8 wheel.
The independent six-byte descriptor omits unresolved consumer fields. No clipping
or arbitrary startup-report dropping. Real-report deduplication ignores idle;
255->0 remains a new sequence. USB enqueue precedes ACK; endpoint busy withholds
ACK without consuming RF budget or report sequence. Fatal errors stop the loop.
End/error attempts neutral button release with bounded waits. USB reset/disconnect
after configuration requires reboot for this prototype; recovery remains future work.

Eight tests pass: UF2/evidence tests plus native actual protocol in both build
modes and actual USB bridge with asynchronous mocks. Cases include duplicate/
idle->data handling,busy/retry/errors/short write,wrap and neutral release/timeout.
SDK's DMA allocation rounds six bytes to eight, so wire size is explicitly six;
native tests now model this padding. The first staged build caught an obsolete
allocation-size assertion; the corrected receiver build passed.

Receiver UF2: `C:\Users\Gabriel\zephyrproject\build-mouse-receiver\zephyr\zephyr.uf2`.
SHA256 `cfc2eb5820c34969f7299ee43b47a28b20bd1421dd5ac70acce816f566441b4b`.
Flash60,288/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF.
Probe/radio/original HID regressions and config/binary separation all pass.
New probe hash `fac516813e95b0b1e049079093b2d3acbed278d59cf701d6d4ef597f6e3816ef`;
new radio hash `6f0f263f5095a064aa8c782d42bb06973eef9c569eaf6c9ab745f0e3689eab8e`.
Original HID hash unchanged `058c1ff58130ab07c14f258cde93df331e5345f99b4b673db0ede46d2f4a5953`.
Preserved hardware-tested V8: ignored `build-pairing-analysis/probe-v8-4ad89938.uf2`.
HID forwarding/descriptor present only in receiver binary; RF arming only in
receiver/probe. No live build/serial session. Board still non-HID V8 expired/passive.

Receiver NOT flashed/tested on hardware. USB enqueue can alter ACK turnaround;
measure it in the first real input test. First test should use an empty workspace,
another pointer,mouse initially OFF/unplugged to reduce stale queued actions,then
power-on only after arming confirmation and movement without clicks/scrolling.
Verify passive enumeration/no input before arm and neutral stop after the window.
No measured latency,throughput,channel recovery or1000Hz claim.

## Receiver v1 flash and passive enumeration (2026-11-15)

User confirmed NICENANO ready/mouse wired. Verified bootloader0.6.0,
nRF52840-nicenano,S1406.1.1,receiver hash cfc2eb58... and application bounds;
copied receiver UF2. Windows PnP reports OK for composite USB,CDC COM12/MI_00,
HID-compliant mouse/MI_02 and HIDClass input on local2FE3:0008.
Five-second DTR-enabled COM12 baseline passed: receiver v1 banner,256-ACK/two-second
limit,1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/receiver-v1-passive-20261115-005017.txt`.
No `p` sent; board receiver v1 passive/unarmed,all budgets unused. COM12 closed.
No pointer output has been observed/tested yet. User asked for separate readiness:
empty desktop,another pointer,mouse OFF/unplugged and left OFF until arming
confirmation. First test is motion only; inspect real cursor response and serial
timing/USB errors before deliberate click/wheel trials. No reflash/baseline repeat.

## Receiver v1 armed test: standalone status blocks progress

User prepared empty desktop,mouse OFF/unplugged,and confirmed readiness. COM12
armed once,then user was told to switch ON and move without clicks/scrolling.
Ignored `build-pairing-analysis/receiver-v1-armed-20261115-005234.txt` and summary:
285 valid RX:254 discovery,one length2 idle `00 00`,30 length11 status packets
`00 00 45 06 01 00 00 00 00 00 00`. One idle ACK(seq0->1); both TXs completed,
no errors/USB busy. No type1 movement reports arrived. User confirmed no wireless
pointer movement. Thus the USB forwarding path remains unverified; the immediate
block is V1's report filter rejecting standalone tag45, previously seen only in
the accepted compound tag41+45 form. Queue-empty idle did not consume sequence0.
Discovery timing58/439/487us(handler/READY/END),idle ACK32/163/207us.

Next: receiver v2/non-HID probe v9 accept length11/tag45, ACK but never forward
to HID or consume movement deduplication. Keep256/two-second guards and response
bytes/timing unchanged. Add native observed-status fixture and USB no-output test,
preserve v1 UF2,rebuild/test before asking for next verified flash. Board v1 window
expired/passive,COM12 closed; user told to stop/reconnect, reconnection unconfirmed.

Receiver v2/source fix now passes all eight tests. The observed standalone status
fixture is accepted by the real matcher/protocol and produces no USB output or
movement deduplication update in the real bridge,including while endpoint busy.
Receiver v1 UF2 preserved as ignored `build-pairing-analysis/receiver-v1-cfc2eb58.uf2`.
Receiver v2 build/UF2:flash60,304/RAM16,064 bytes,236 blocks,0x26000..0x34BFF,
SHA256 `e80eb30c51fa375b39a2af5a6d15de65341765673db0682fc9fe60a95591b6c9`.
Non-HID probe v9:flash58,796/RAM15,936 bytes,230 blocks,0x26000..0x345FF,
SHA256 `a1bc4de092a01bdce4c98eb907dfef85ac57675f0d84a33f17a18c1848c22c39`.
Config/binary separation checks pass;radio/default HID artifacts remain unchanged
from6f0f263f.../058c1ff5... above. No v2 hardware test yet. Board remains receiver
v1 expired/passive. Next: verified v2 flash/passive baseline after NICENANO readiness,
then separate off/unplugged/empty-desktop readiness and one motion-only test.

Receiver v2 subsequently flashed after user confirmed NICENANO ready/mouse wired.
Reverified bootloader0.6.0,nRF52840-nicenano,S1406.1.1 and e80eb30c... UF2 bounds/hash.
Five-second DTR-enabled COM12 baseline passed: v2 banner,256/two-second limits,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors/arming.
Windows composite/CDC/HID statuses all OK. Evidence: ignored
`build-pairing-analysis/receiver-v2-passive-20261115-010250.txt`.
Board v2 fresh/unarmed,all budgets unused;COM12 closed. User asked for separate
off/unplugged/empty-desktop readiness; v2 armed pointer test pending.

## Receiver v2: first end-to-end wireless pointer confirmation

After off/unplugged/empty-desktop readiness,armed once on COM12,confirmed arming,
then asked user to power ON/move without clicks/scrolling. Ignored capture:
`build-pairing-analysis/receiver-v2-armed-20261115-010552.txt` and summary JSON.
415 complete valid RX:271 discovery,34 idle,110 length11.122 ACKs over1971ms:
34 idle,one standalone status tag45,87 unique type1 reports. All123 TXs completed,
no errors/USB busy. Movement reports had buttons0,wheel0,X -117..149,Y -64..96.
ACK timing ranges(handler/READY/END after RX END,us):13..112/144..243/189..288,
now including USB enqueue cost before ACK.

User explicitly confirmed Windows pointer moved in response to the unplugged
Glorious mouse and stopped after the brief window,with no other pointer used.
This establishes bounded end-to-end RF-to-USB pointer control. Click/wheel USB
behavior,continuous recovery,latency/throughput and gaming performance remain
unverified. Board v2 window expired/passive,122 attempts used,COM12 closed.
User told to stop/reconnect cable;reconnection unconfirmed.
Next: longer bounded receiver v3 input test with1024 ACK cap/10-second window;
non-HID probe keeps256/two-second limits. No changes to RF bytes/timing or USB
mapping. Native deadline/limit checks and build validation precede another flash.
Capture45seconds to cover30-second discovery arming plus10-second active window.

Receiver v3 source and final build now pass all eight tests. Shared fixed constants
drive ACK deadline,listen duration and printed limits:receiver1024/10000ms,
non-HID probe256/2000ms. RF bytes/timing,USB descriptor and movement mapping remain
unchanged. A new native regression covers a256-status-only transport wrap:when
status advances beyond the last report,invalidate that old retry marker without
generating input;idle still leaves it unchanged. This avoids suppressing a new
report after a full status-only sequence cycle. Not a hardware wrap measurement.

Final receiver v3 UF2 SHA256:
`4bc43f3543638f337eb7d50d42621cc20be321e79ebdefed2ba8d94d9b73d4da`.
Flash60,332/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF. Do not use the
intermediate a020e8ab... build that preceded the wrap-gap fix.
Updated non-HID probe v9:flash58,808/RAM15,936,230 blocks,0x26000..0x345FF,
SHA256 `8fc95a4738701e29ed84f72e3430ccce89a450c917c7498f2879338402ff4de5`.
Radio regression remains6f0f263f... and original HID remains058c1ff5... above.
Config/binary separation and staged receiver source checks pass. Working v2
preserved locally at ignored `build-pairing-analysis/receiver-v2-e80eb30c.uf2`.
Board still v2 expired/passive;COM12 closed,no live build/serial session. V3 not
flashed. Next: verified v3 flash/passive baseline after NICENANO readiness,then
separate readiness on a harmless scrollable document/page and one45-second
capture for a ten-second deliberate movement/click/wheel test. No gaming claim.

## Receiver v3 flash/passive check and handoff (2026-11-15)

User confirmed NICENANO ready/mouse wired. Rechecked final4bc43f35... UF2 bounds/
hash,bootloader0.6.0,nRF52840-nicenano,S1406.1.1 and copied application UF2.
Five-second DTR-enabled COM12 startup passed:v3 banner,1024 ACKs/10000ms,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors/arming.
Windows composite/CDC/HID statuses all OK. Evidence: ignored
`build-pairing-analysis/receiver-v3-passive-20261115-012451.txt`.
Board v3 passive/unarmed,all budgets unused;COM12 closed,no live build/serial
session. Mouse last wired. User requested next-chat handoff before arming.
Next: separate harmless-document/off-unplugged readiness,then one45-second
capture for the10-second controls trial. Do not rebuild/reflash/repeat baseline.

## Receiver v3 armed controls attempt: no RF received (2026-11-15)

User confirmed readiness with harmless scrollable content,another pointer,and
mouse OFF/unplugged. Sandbox COM12 open failed before sending any command;
authorized elevated device access succeeded. DTR enabled,one `p` sent,and exact
1024/10000ms arming text confirmed before the user power-on/controls cue.
45-second capture completed and COM12 closed. Evidence: ignored
`build-pairing-analysis/receiver-v3-armed-20261115-013158.txt` and `.summary.json`.
One arming,81 SCAN lines,zero valid RX/CRC failures/invalid lengths,zero discovery
responses/ACKs,zero errors/USB busy. There was no RF-to-HID traffic to validate
movement,clicks,wheel or neutral release. User observations/power-on timing pending;
no cause for absent traffic established. Arming window expired; budgets unused,
but one-arm opportunity consumed. No automatic rearm/reboot. A repeat requires
physical normal receiver reboot and fresh readiness,not rebuild/flash/baseline.
User subsequently confirmed powering on late or missing the cue. Controls were
not tested during this attempt; pause for normal receiver reboot/fresh readiness.

Second v3 attempt after fresh readiness: ignored
`build-pairing-analysis/receiver-v3-armed-20261115-013553.txt` and `.summary.json`.
One p/confirmed arming,then requested pop-up power-on cue;45seconds completed,
COM12 closed. Again zero RX/discovery replies/ACKs. User timing/observations
pending; no controls validation or firmware failure conclusion. Pause for user;
do not automatically retry. Use pop-ups for required physical intervention.
User confirmed prompt power-on with USB unplugged and no Windows response.
Next targeted diagnostic: mouse already ON near unchanged/expired receiver before
a15-second COM12/DTR capture,WITHOUT p,to separate RF silence from cue timing.
Pause for physical readiness; no rebuild/flash or automatic armed retry.

## RF-silence diagnostic finds discovery plus stale serial output

User left mouse ON/unplugged/near unchanged expired receiver.15-second COM12/DTR
capture WITHOUT commands completed/closed. Ignored evidence:
`build-pairing-analysis/receiver-v3-rf-silence-20261115-013920.txt` and `.summary.json`.
456 complete packets all match known mouse discovery payload;204 satisfy exact
discovery channel/base/match guards.22 SCAN lines,no ERROR,no arming/TX.
However RX timestamps94389..270012 span175623ms in15s host capture,with161223ms
largest jump: historical serial output is queued.457 RX markers,456 complete lines.
First SCAN counters0,last good4556/bad44/invalid8 are cumulative,not capture totals.
Earlier zero-counter host captures therefore cannot establish concurrent RF silence.
No input validation or firmware fault conclusion. Next inspect serial buffering,
then fresh reboot/readiness with continuous drained capture before single arm.
Installed Zephyr legacy `subsys/usb/device/class/cdc_acm.c` poll_out uses a TX
ring buffer,blocking when full with flow control or discarding bytes otherwise.
No firmware modification justified by these logs. Next normal reboot/mouse-OFF
readiness,continuous serial drain before ONE p,new arming confirmation before
power-on cue,and same reader for45seconds. Save host phase timing too.

## Drained continuous v3 capture: ten-second wireless movement

Fresh reboot/mouse-OFF readiness;one COM12/DTR reader drained2seconds before
ONE p,new arming text confirmed,then power-on pop-up.45seconds after arm saved:
ignored `build-pairing-analysis/receiver-v3-drained-20261115-014330.txt`,
`.host.jsonl` with host UTC/elapsed/phase chunks,and `.summary.json`. Port closed.
1049 complete post-arm RX:403 discovery,35 idle,611 length11.622 ACKs over9974ms:
35 idle,one standalone status,586 unique type1. All623 TXs completed,no ERROR/
USB busy,no duplicate type1 ACKs. Buttons0/wheel0 throughout;X-56..43,Y-32..23.
ACK RX-END-to-handler/READY/END ranges13..115/144..246/188..291us.
User confirms pointer moved but did not try buttons/wheel. Neutral-stop user
observation pending;all-zero buttons cannot validate forced neutral release.
V3 expired/passive;one arm/discovery used,622 ACK attempts used. Next physical
normal reboot/fresh readiness via pop-up,then same continuous/drained capture
prioritizing clicks/wheel immediately instead of movement. No firmware changes.

## Receiver v3 Windows clicks/wheel milestone (2026-11-15)

Prior trial's movement stop at limit without unplugging receiver is now explicitly
user-confirmed. After fresh receiver reboot/mouse-OFF readiness,continuous
COM12/DTR capture drained2s before ONE p,new arming confirmed before power-on
pop-up,then45s captured. Ignored `build-pairing-analysis/receiver-v3-controls-20261115-014826.txt`,
`.host.jsonl` and `.summary.json`;COM12 closed/disposed.
1031 complete post-arm RX:393 discovery,229 idle,409 length11.611 ACKs over9980ms:
225 idle,one status,385 type1 ACKs (383 unique reports,two retries). Three left
press/release cycles and one right press/release;wheel fifteen -1 and eleven +1
unique reports. Last unique report buttons0. All612 TXs completed,no ERROR or
USB busy. ACK RX-END-to-handler/READY/END ranges12..461/143..592/187..637us;
not end-to-end latency or throughput measurements.
User confirms all left/right clicks and wheel UP/DOWN worked in Windows and
stopped with no stuck buttons. This establishes bounded normal controls/release.
Forced release with button still held at expiry remains untested. Next readiness
for plain-text selection/left-held-at-expiry trial with other pointer to check
selection no longer extends after timeout. No firmware changes required.

## Forced left-button timeout release validated (2026-11-15)

Fresh normal receiver reboot/mouse-OFF readiness;continuous COM12/DTR reader
drained2s before ONE p,new arming confirmed before power-on/held-left pop-up,
then45s captured. Ignored `build-pairing-analysis/receiver-v3-held-20261115-015320.txt`,
`.host.jsonl` and `.summary.json`.COM12 closed/disposed.
820 complete post-arm RX:180 discovery,136 idle,504 length11.617 ACKs over9983ms:
136 idle,one status,480 type1 ACKs (478 unique,two retries).428 unique ACKed
reports LEFT held,50 released. Final ACKed type1 seq222/t81531ms has buttons1;
later unACKed seq223/t81796ms also held. No later type1 release in capture.
All618 TXs completed,no ERROR/USB busy. ACK RX-END-to-handler/READY/END ranges
12..308/143..439/187..483us;instrumentation only,not end-to-end latency.
User held LEFT about15s,then moved OTHER pointer without its buttons while
Glorious LEFT remained held:selection stayed fixed. This plus final ACKed held
state/no release error validates forced neutral at normal active-window expiry.
USB disconnect/reset/error release paths remain untested physically. Board v3
expired/passive,one arm/discovery used,617 ACK attempts used. Bounded controls
validation complete;continuous link/recovery and gaming performance remain open.
