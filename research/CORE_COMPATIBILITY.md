# Glorious CORE compatibility investigation

November 19, 2026. Static evidence and offline tests only so far.

The user chose to investigate official CORE support before the pending four-hour
trial. Normal tested v14 is unchanged. No mouse settings, USB identities or RF
configuration commands have been changed by this investigation.

## Evidence sources

The installed `Glorious Core/resources/app.asar` includes its device table,
`ModelOSeries.js`, `DeviceService.js` and native `hiddevice.cc` source. Selected
files are retained privately under ignored `build-pairing-analysis/core-static/`.
No proprietary source or binary is committed. The existing mouse application dump
and `mouse-disassembly.txt` supply device-side static evidence; no new extraction
was performed. CORE's database files are line-delimited JSON records, not SQLite.

## Detection and feature support

CORE's Series One PRO record explicitly lists wired `258A:2018` and dongle
`258A:2031`, routes both through `ModelOSeries`, and requests a configuration
collection with usage page `FFFF`, usage `0000`. Notifications use `FFFF/0001`,
`FFA0/0001` and consumer `000C/0001` collections.

The record has `LEDSetting: false`. CORE does not expose general lighting for
this model. DPI-stage colour is separate, present in `SetDPIColor2Device`.
This is evidence about the installed application's support, not a new physical
verification of all LEDs on this particular mouse.

CORE initializes devices using `SetProfileDataFromDB`; detection can trigger
profile writes. A firmware that merely pretends success could mislead CORE or
overwrite settings incompletely. Do not connect a partially implemented compatible
device to CORE until command forwarding and explicit failure responses are ready.
Do not forward firmware-updater/bootloader commands.

## USB command format

Offsets below include the Windows report-ID byte. SET_FEATURE is 65 bytes, with
`[0]=0`, target `[3]=2` (mouse), payload size `[4]`, group `[5]`, operation `[6]`,
payload starting `[7]`. The native CORE binding removes report ID on GET_FEATURE;
its JavaScript response offsets are therefore one lower than Windows buffers.
Our helper retains the report ID. Status is `[1]`: `A0` busy, `A1` success,
`A2` failure, `A3` unsupported, `A4` receiver cannot find mouse (CORE comments).

| Operation | Group / operation | Evidence |
| --- | --- | --- |
| Firmware version read | `00 / 81` | CORE `ReadFWVersion` |
| Polling divider write / read | `01 / 00`, `01 / 80` | CORE write, mouse dispatcher read |
| DPI stages write / read | `01 / 01`, `01 / 81` | CORE write, mouse dispatcher read |
| Selected stage write / read | `01 / 02`, `01 / 82` | CORE write, mouse dispatcher read |
| DPI-stage colours write | `02 / 01` | CORE `SetDPIColor2Device` |
| Button binding write | `03 / 00` | CORE `SetKeyFunction` |

Polling divider `1/2/4/8` corresponds to `1000/500/250/125` Hz in CORE.
DPI-stage payload starts with one-based profile and stage count, followed by
big-endian X/Y 16-bit values per stage. Selected-stage payload uses one-based
profile and stage. Button writes include profile, button ID, slot and binding
record; keyboard/consumer output would also need receiver HID forwarding.

Mouse dump `0x151AC` dispatches groups 0..4 to `0x14788`, `0x15290`, `0x156B8`,
`0x14540`, `0x149E8`. Group-1 read paths at `0x15306`, `0x153FA..0x1547E`,
`0x15526` return polling divider, DPI-stage list and active stage. These are
static inferences, not yet hardware-confirmed settings queries.

## Radio transport: promising, not hardware-validated

The existing short ACK has length 2 and carries no command. The stock mouse
accepts longer ACKs in `0x16044..0x16084`, with an independent lower-six-bit
command sequence and a queued payload. `0x13334..0x13410` walks payload records,
decoding low-three-bit type, four-bit length and a high continuation bit.
Type 7 records reassemble a configuration buffer by byte offset. On final data,
`0x133E8..0x13408` copies 64 bytes into the same mailbox used by USB and sets its
pending flag. `0x151AC` dispatches that mailbox. This establishes a static path
from ACK payload to settings handler, not a complete usable transport yet.

Still to validate: framing and offset bounds, sequence startup/wrap/retry,
completion/reply fragmentation, notification behavior, disconnected status and
preserving the motion path during configuration. Never send a speculative write.

## Prepared read-only reference

`research/read_mouse_settings.ps1` reuses the existing 32-bit HidSharp library
and permits only four fixed queries: version, divider, profile-1 stages and
profile-1 active stage. It refuses an ambiguous device or running CORE. It has
bounded busy polling and requires an `A1` reply with matching command fields.
Raw replies must be saved in the ignored analysis directory, not printed publicly.

Run its offline check with 32-bit PowerShell and process-only execution policy:
`powershell.exe -NoProfile -ExecutionPolicy Bypass -File research/read_mouse_settings.ps1 -SelfTest`.
The first invocation without process-only policy was blocked by local execution
policy; no system policy was changed. The offline check passes; no hardware query
has run yet. Hardware results will determine whether reply assumptions need revision.

Next physical step: close CORE, connect the mouse by USB in wired mode and leave
the DIY receiver unchanged. Use a readiness popup before running the helper.
Then implement an allowlisted wireless read-query experiment as described in
`docs/plans/2026-11-19-glorious-core-compatibility.md`.


November 19 WIRED READBACK PASS (no settings changed): four fixed queries returned
A1/matching commands. Firmware 1.0.0.0; polling divider1=1000Hz; profile1 stages
400/800/1600/3200 (X=Y), active stage2=800DPI. Earlier CORE UI saved1600 was not
current device readback. Raw replies ignored: core-wired-settings-reference.json.
Glorious is wired/CORE closed by user readiness; receiver remains normalv14.
Next autonomous work: bounded radio READ-query codec/diagnostic; no settings
writes or identity impersonation. Four-hour trial remains deferred.


## Prepared v20 wireless read-only diagnostic

The separate -App config-read build uses existing quiet normal startup and
stopped-only CDC writer. RF_CONFIG_READ_DIAGNOSTICS is off for other builds.
All pointer forwarding is bypassed in this diagnostic; it is not an everyday
receiver image or a CORE-compatible configuration interface.

It serializes only the fixed version request verified over wired USB, using
eight type7 records with explicit offsets and eight data bytes each. Repeated
command-sequence values resend identical fragments; advancement must be exactly
one modulo63. Fragment attempts cap at512. Independent mouse data-queue ACKs
are retained. Replies use a fixed64-byte buffer, ignore duplicate queue reports
and reject gaps, bounds and command mismatches. Link loss/explicit stop marks
an incomplete query failed. No settings or bootloader commands are supported.

Production config_read.h codec tests cover framing, retransmission, wrap, caps,
reply assembly and failure. The existing probe harness tests actual shared ACK,
no-pointer and x-stop integration in a fourth mode. All12 native checks pass.
The capture parser self-test passes: fixed product check, one reader, x at35s,
d afterward, close by40s; it never sends p or arbitrary command data.

Validated private image: config-read-v20-68f92a56.uf2, SHA256
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632.
191blocks, 0x26000..0x31F00 exclusive, 48,828flash/16,512RAM; product
DIY Mouse Config Read v20, 1ms HID interval. Not flashed/hardware-tested.
Metadata/build log ignored. Two unused-function warnings reflect deliberately
bypassed pointer-forward and ordinary timing-dump functions.

Build preparation caught a reserved CONFIG_ CMake prefix and obsolete cache
entry. Renamed the flag and regenerated only the disposable diagnostic cache;
clean build passes. The exact normal rollback image is untouched.


## Break handoff

User requested a break before flashing. Both builds finished successfully;
no build/reader/capture remains. Normal isolation control has config-read OFF,
SHA2566877579cdeb4bfd5587894f7e238dcf31eb1c6ddcfce0ddf0ecf3704c5ca82c7,
46,188flash/16,064RAM; not hardware-tested and not a replacement rollback.
Normal tested v14 remains installed. V20 is built and preserved, not flashed.
On resume use separate bootloader and input-readiness popups, then one stationary
wireless version-query capture and exact93eb987a normal restoration. No CORE
connection or settings writes until transport validation. See current context.


NOVEMBER20 V20 FLASH/USB STARTUP VERIFIED - overrides installed-v14 notes below.
User confirmed NICENANO visible, CORE closed, Glorious wired/OFF. Verified unique
D: NICENANO, Board-ID nRF52840-nicenano, UF2bootloader0.6.0 and exact saved v20
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632 before copy.
V20 NOW INSTALLED: DIY Mouse Config Read v20, COM12, healthy USB enumeration.
Private startup record: build-pairing-analysis/config-read-v20-usb-startup.json.
No serial opened, no live capture/build/helper, no settings writes, no mouse RF
trial yet. This diagnostic has NO wireless pointer output. Keep CORE closed.
NEXT pending physical popup: Glorious OFF/USBunplugged, other pointer available,
receiver unchanged. After Ready start capture_config_read.ps1, confirm RUNNING
before cue to turn mouse ON and leave STILL. Fixed read only; x35s/d, close40s.
After trial or failure ask bootloader readiness and restore EXACT TESTED v14
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052, not normal
software-control6877579c. Four-hour trial remains deferred for CORE work.


NOVEMBER20 WIRELESS CONFIG READ PASS - CURRENT INSTALLED V20, STOPPED.
User confirmed OFF/unplugged readiness, then ON/still after capture-running cue.
One-reader research/capture_config_read.ps1 completed; x at35s then frozen d.
Private capture: config-read-v20-20261120-043100-1f1efba5 (.json/.txt).
Success=true, started1, offset64, attempts8 (eight fragments), reply16bytes,
complete1, failed0; firmware1.0.0.0 exactly matches wired reference. No movement
output/settings write/CORE vendor interface. This validates nominal RF config
read roundtrip only, not general settings writes/retry stress/full CORE support.
No live serial/capture/build/helper/timer remains. V20 is stopped by x and still
installed; user last confirmed Glorious ON/unplugged/still. Keep CORE closed.
NEXT PHYSICAL PAUSE: Glorious wired/OFF, double-reset RECEIVER into NICENANO;
verify unique board and restore EXACT TESTED normalv14 frozen93eb987a UF2:
SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Do NOT restore source-built normalcontrol6877579c. Verify USB product normalv14
without serial; then separate user check that wireless automatic control works.
After restoration continue CORE vendor transport work from this successful read
reference. Four-hour trial remains deferred; app compatibility before soak.


NOVEMBER20 EXACT NORMAL V14 RESTORED after successful v20 RF read.
User confirmed NICENANO readiness/mouse wired-OFF. Verified board and exact
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 frozen v14
before copy; USB product DIY Mouse Dongle v14 verified afterward. No serial
opened. Private record: core-v20-normal-restoration.json. Physical wireless
movement/click restoration check PENDING popup; no recording running.
No build/serial/helper/timer/capture remains. No settings writes occurred.
V20 hardware read milestone pushed5bda374: firmware1.0.0.0 equals wired,8fragment
attempts,16replybytes,complete/no failure. V20 is no longer installed.
After restoration check, continue CORE-compatible vendor transport preparation
on feature/glorious-core-compatibility; keep four-hour trial deferred. No need
to repeat version baseline/build/flash. CORE detection/settings writes remain
unimplemented. Ask fresh physical readiness for any future diagnostic flash.


NOVEMBER20 RESTORATION COMPLETE: user confirms automatic wireless movement/click
works normally on exact tested v14 after v20. No capture/serial/build running.
Next software milestone: native USB Feature-report to RF read-only bridge using
local prototype IDs first; CORE cannot connect to this staging identity. Existing
CORE native code matches VID/PID and HID usage, NOT interface number, so CDC may
remain while adding a separate vendor HID collection; no need to remove CDC or
reorder original mouse interface just to test transport. Reject settings/update
commands during this read-only stage; app initialization can write profiles.
No existing user settings changed; four-hour trial remains deferred.


NOVEMBER20 V21 SOFTWARE READY; BREAK BY05:00AM MANILA (no flash at/after04:50).
Normal exact testedv14 remains installed and user-confirmed; no serial/capture/
build/helper/timer running. V20 RF version-read PASS saved5bda374, normalrestore
confirmed817851a. Four-hour trial remains deferred for CORE compatibility.

New -App config-hid v21 is BUILT/UF2-VALIDATED, NOT FLASHED/HARDWARE-TESTED.
Private frozen config-hid-v21-a7631b8d.uf2; SHA256
a7631b8dcae26221b13bf1e1d397f0f5f93dcb795c767a553bb67f9beabf22df.
Product DIY Mouse Config HID v21;194blocks;0x26000..0x32200;49572flash/16768RAM.
Two HID instances, vendor Feature64bytes usageFFFF/0000 plus CDC, local2FE3:0008
identity (CORE WILL NOT DETECT IT). No pointer output. EXACT firmware-version
read only, once/boot. No command sent before host Feature SET. USB callbacks
queue/snapshot; main processes/provides real RF reply, busyA0/failureA2.
Arbitrary settings/bootloader requests and repeat SET rejected. v20 automatic
read path still separate; v21 only host-triggered. No settings writes implemented.
Native13 tests PASS including real RF-codec through USB callback/mailbox and five
shared probe build modes (normal/probe/receiver/v20/v21). Host read_receiver_version.ps1
SelfTest PASS. Source/staged equality, quietflags, bothRFflagsON, fastpollOFF,
localUSBIDs,1ms interval verified. No normal rebuild needed: conditional off-path
native checks passed; hardware-tested rollback remains EXACT93eb987a frozenimage.

NEXT pending physical readiness popup if BEFORE04:50: COREclosed, GloriousWIRED
orOFF; double-reset only receiver into NICENANO; verifyboard+v21image thenflash.
After USBcheck, separate readiness: GloriousON/USBunplugged/STILL nearreceiver,
otherpointer available. No timed power-on cue needed (host triggersquery).
Run 32-bit powershell -NoProfile -ExecutionPolicy Bypass -File
research/read_receiver_version.ps1. It checks exactv21 identity/uniqueFeature,
GETinitialA3, sends ONE fixed SET, pollsGET up to10s, savesrawreplyprivately.
No CDC reader/capture window or p/x command needed for that helper. Restore EXACT
TESTED normalv14 SHA93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
using separate bootloader readiness and brief wirelesscheck afterward.
If cutoff reached or user pauses, leave normalv14 installed and resumev21later;
5AM deadline must not be bypassed. Do not repeat extraction, completedbuilds,
versionbaseline or normalrestoration. See docs/plans/2026-11-20-usb-config-read.md.

## Requested pause before v21 physical test

PAUSED BY USER November20 before05:00AM Manila. User explicitly selected
"Pause here - test v21 next session". Resume only when user returns.
Normal exact hardware-tested v14 remains installed and user-confirmed working.
V21 is BUILT/VALIDATED BUT NOT FLASHED/HARDWARE-TESTED. Software milestone3a13236
pushed on feature/glorious-core-compatibility. No capture/serial/build/timer/
helper running, no physical intervention requested now, no settings changed.

Next session read docs/plans/2026-11-20-usb-config-read.md and
research/CORE_COMPATIBILITY.md. Ask fresh bootloader readiness for receiver only,
Glorious wired/OFF and CORE closed. Verify unique NICENANO board and saved image:
build-pairing-analysis/config-hid-v21-a7631b8d.uf2; SHA256
a7631b8dcae26221b13bf1e1d397f0f5f93dcb795c767a553bb67f9beabf22df.
Product DIY Mouse Config HID v21; localUSB2FE3:0008; twoHID instances; no pointer
output; one exact version READ perboot. No CORE identity or settings writes.
13native checks, host-helper self-test, staged-source/config/UF2 validation PASS.
After flash/USBcheck, separate readiness: GloriousON/USBunplugged/STILL near
receiver, another pointer available. No timed power-on cue/serial reader needed.
32-bit SysWOW64 powershell -NoProfile -ExecutionPolicy Bypass -File
research/read_receiver_version.ps1 drives one FeatureSET and boundedGET polls.
Compare real RF response to wired1.0.0.0; save observation privately. Restore
EXACT testedv14 dongle-v14-93eb987a.uf2 / SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 via separate
bootloader popup, then brief wireless restoration check. Do not substitute any
normal software-control build. No completed builds/extraction/baselines repeated.

Earlier v20 RFread PASS:8fragment attempts,16replybytes, firmware1.0.0.0 equals
wired; exactv14 restored. Wired settings1000Hz, stages400/800/1600/3200, active800.
Four-hour trial deferred for official CORE compatibility; separate app fallback.
General lighting disabled in installed CORE model record; DPI colours separate.
The05:00AM pause is now honored. Ask new available time on a later resumed session;
old04:50 flash cutoff is not authorization to flash later while project paused.


NOVEMBER20 V21 FLASHED; VENDOR USB INTERFACE FAILED - RESTORE V14 NEXT.
User confirmedNICENANO/COREclosed/mousewired-OFF and continue until toldstop
(no current time cutoff). Verified unique board/bootloader and exacta7631b8d
image hash beforecopy. Product DIY Mouse Config HID v21 enumerates; mouseMI_02
and CDC/COM12 healthy, vendor USB Input Device MI_03 Code10, ProblemStatus
3221225657 (0xC00000B9). No vendor HID child created. Host version helper NOT RUN;
no FeatureSET, RF query, serial session or settings write. No live sessions/build.
Private failure observation: config-hid-v21-usb-startup.json.

Static comparison found v21 vendor descriptor lacks local Usage before Feature;
known working mouse/stockreceiver descriptor includes Usage00 inside collection:
06ffff0900a10109001500250175089540b102c0 (20bytes). Omitted field usage is likely
cause, not yet corrected/hardware-proven. Current test checked descriptor size/
prefix only and missed it. After normalrestoration, add a failing semantic
local-usage/report-length regression, correct descriptor (preserve oldv21image),
use distinctv22product/hash and validate/build before separate physicaltest.
Microsoft primary HID value-capability docs support per-main-item usages;
Windows Code10 alone does not prove the exact cause. No driver replacement.

CURRENT INSTALLED v21 has NO wireless pointer output, vendor interface broken.
NEXT PHYSICAL POPUP: keep Gloriouswired/OFF, COREclosed; double-reset ONLY
receiver intoNICENANO. Restore EXACT tested normalv14 frozen93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052,
verifyUSB product and fresh userwirelesscheck. Do not reuse sourcecontrol image.
Do not retry a v21 query against failedHID or repeat extraction/baselines.
Four-hour trial remainsdeferred, CORE compatibility is thepriority.


NOVEMBER20 NORMAL V14 RESTORED after v21 USB vendor failure.
Fresh userNICENANO readiness; verified unique bootloader/board and exact frozen
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 beforecopy.
USB product DIY Mouse Dongle v14 and normal mouseHID healthy. No serial opened.
Private record core-v21-normal-restoration.json; physicalwirelesscheck PENDING.
No capture/build/helper/timer. V21 is no longer installed. No mouse settings
changed. NEXT popup: unplug GloriousUSB, turnON, try movement/click withoutarm.
Aftercheck continue descriptorfix/regressionsoftware prep for distinctv22;
oldv21vendor Code10 likelymissinglocalUsage but correction not yet hardwareproven.
Current user preference: continue until toldstop. Four-hour trial still deferred.


NOVEMBER20 V22 CORRECTION BUILT/VALIDATED; NOT FLASHED/HARDWARE-TESTED.
Normal exacttestedv14 is installed and user-confirmed working. No livebuild,
serial/capture/helper/timer. No settings writes. Continue until user saysstop.
Preserved failedv21; descriptorcorrected to known20-byte vendorfeature descriptor.
Newsemanticregression failedoldbeforefix and now passes; all13nativechecksPASS,
hosthelper SelfTest and32-bitCIM/Pnp commandavailabilityPASS.
Private frozen config-hid-v22-9cb33d52.uf2; SHA256
9cb33d5224c8eb517ab8a3e00f27c007e58c2ddd6c3ffdc4e7c95f6c73991523.
Product DIY Mouse Config HID v22;194blocks;0x26000..0x32200;49572flash/16768RAM.
Stagedsource/config/flags verified: twoHIDs, local2FE3:0008,1ms, quiet, read+HID
flagsON, fastpollOFF. One fixedversionREAD/boot, no pointeroutput, noCOREidentity,
no settings/updater writes. Build/test doesNOT prove Windowsstartupfix yet.
NEXT physicalpopup: COREclosed,Gloriouswired/OFF,double-resetONLYreceiver to
NICENANO. Verifyboard+exactv22hash,flash,thencheck BOTH HIDcollections healthy
(noCode10) andCDC. Ifhealthy, separate GloriousON/unplugged/STILL readiness,
then32-bit research/read_receiver_version.ps1 (v22gate), oneSET/boundedGET polls,
compareversion1.0.0.0wired. No timedpowercue or serialreader. Ifstartupfails,
noquery; restorev14 andinvestigate. Restoreexacttestedv14afterbatch via fresh
physicalpopup: frozen93eb987a hash93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Do not repeat extraction/completedbuild/baseline or flasholdv21. Four-hourtrial
stilldeferred for CORE; fullappsettings remainunimplemented.


NOVEMBER20 V22 NOW FLASHED; WINDOWS VENDOR STARTUP PASS.
UserfreshNICENANO/COREclosed/Gloriouswired-OFF readiness; verifiedunique board,
bootloader0.6.0 and exact9cb33d52image hash beforecopy. USBproduct
DIY Mouse Config HID v22; BOTH mouse/vendor HIDcollections healthy, CDC/COM12,
zeroerror devices (v21Code10 gone thisboot). Private config-hid-v22-usb-startup.json.
No serial opened, no FeatureSET/RF query/settings write. No livebuild/helper.
Currentinstalledv22 intentionallyNOwirelesspointeroutput. COREmuststayclosed.
NEXT physicalpopup: GloriousON/USBunplugged/STILL nearreceiver, otherpointer
available; receiverunchanged. No timedcue needed becauseUSBhost triggersquery.
Then32-bit read_receiver_version.ps1 exactv22gate; initialGETA3, ONEfixedSET,
boundedGET polls, compareactual RF version1.0.0.0withwiredreference, saveprivate.
Afterquery/failure restoreexactTESTEDv14 93eb987a viafreshbootloaderpopup and
briefwirelesscheck. Four-hourtrialstilldeferred. Continueuntiluser saysstop.


NOVEMBER20 V22 HOST USB->RF->USB VERSION READ PASS.
UserconfirmedGloriousON/USBunplugged/STILL, COREclosed. 32-bit preparedhelper
finished successfully, one FeatureSET and twoGETs(total includesinitialA3).
Returned firmware1.0.0.0 exactlymatcheswiredreference; Success=true.
Private observation config-hid-v22-20261120-175941-a3145cd2.json. No serial,
no settings write, no livehelper/capture/build/timer. V22 remainsinstalled with
pointeroutputdisabled; one-shotqueryused. This establishes vendorFeature host
roundtrip throughreceiver tostockmouse, not fullCOREdetection/configwrites.
NEXT freshphysicalpopup: Gloriouswired/OFF, COREclosed, double-resetONLYreceiver
intoNICENANO; restore EXACT TESTED normalv14 frozen93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
VerifyUSB thenbriefuserwirelesscheck. Afterrestoration continue multi-command/
allowlist/CORE compatibility implementation. Four-hourtrialstilldeferred.


NOVEMBER20 EXACT NORMAL V14 RESTORED after successful v22 end-to-end read.
FreshNICENANO/mousewired-OFF readiness; board+exact93eb987a frozenimage verified
beforecopy. USBproductDIY Mouse Dongle v14 and normalmousecollection healthy.
No serialopened. Private recordcore-v22-normal-restoration.json; physicalwireless
restorationcheck PENDINGpopup. V22 no longerinstalled. No livecapture/build/helper.
V22successmilestone623acb7 pushed: correctedvendorhealthy,65byteFeature, oneSET/
twoGETs returnedrealRFfirmware1.0.0.0matchingwired. No settingschanged.
Afteruserwirelesscheck continue multi-command/readback/COREtransport software;
four-hourtrialstilldeferred. User sayscontinueuntilstop, no timecutoff.


NOVEMBER20 V23 SOFTWARE READY, NOT FLASHED/HARDWARE-TESTED.
Normal EXACTtestedv14 remainsinstalled and userconfirmedworking afterv22.
No livebuild/serial/capture/helper/timer; NO realmouse settingswrites yet.
Frozen private config-hid-v23-ee7f5a7a.uf2; SHA256
ee7f5a7a220ba7a6c56c40f7a6bf1bc1db9f7ffac058d37a3777094cec148dbf.
Product DIY Mouse Config HID v23;196blocks;0x26000..0x32400;50084flash/16896RAM.
Stagedsources/config/quietflags/twoHIDs/localIDs2FE3:0008/1ms checked; fastpollOFF.
13nativechecksPASS (reusable SETs, reply-release/GET-consumption gates, dynamic
reply/selection echo, wrap, timeoutlockout, normal/probe paths). Hostnewhelper
SelfTestPASS incl busyGETwithoutSETretry, stage/backup bounds and actual trial
workflow faults: restoreattempt after read/writefailure; failedrestore remains
pending. Original wiredreader SelfTestPASS after sharedprotocol extraction.

Canonicalallowlist: version, activeProfile00/85, polling, profile1DPIstages and
selectedstage READs; onlyWRITE profile1 selectedstage1..4. No generalDPIvalues,
lighting/bindings/updater/address writes. COREcannotattach toprototypeIDs.
Main ownsRF;4sdeadline; failedtransactionquarantinesSETuntilreboot; completed
reply mustbe released bymousequeue advance and consumed byhostGET before reuse.
Vendor descriptor isunchanged from hardware-testedv22. Pointeroutputdisabled.
New research/test_receiver_config.ps1 defaultREAD-only. -ExerciseDpi backs up
freshselectedstage BEFORE change, triesalternateexistingstage, readbacks and
restoresoriginal in finally, verifiesrestore. Saved .backup.json and pointer
build-pairing-analysis/current-config-hid-backup.txt markNeedsWiredRestore=true
untilverifiedrestoration; unfinishedrecoveryblocksnewwrites. -RestoreWired uses
freshwiredreadiness, boundedprivatebackuppath andfixedstagecommand only. On
interruptedtrial CHECKthispointer/pendingflag BEFORE anyotherconfigurationwork.

NEXT commit/push thenfreshbootloaderpopup: COREclosed,Gloriouswired/OFF,
double-resetONLYreceiver intoNICENANO. VerifyuniqueBoard+exactv23UF2 thenflash,
checkUSBproduct/BOTHhealthyHIDs/65byteFeature (noqueryyet). Separate mouseON/
USBunplugged/STILL readiness; run32-bit test_receiver_config.ps1 WITHOUTswitches
for FIRST READ-ONLYfivequery hardwarecheck. OnlyifreadsPASS askseparatereadiness
for TEMPORARYselectedDPIchange+restore using -ExerciseDpi. No timedpowercue or
serialreader. Ifwrite/restorationfails, pauseforwiredrecovery usingbackup BEFORE
normalwirelessuse; do not reusequarantinedreceiver. Restore EXACTtestednormalv14
93eb987a hash93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
afterbatch withfreshbootloaderpopup andusercontrolcheck. Four-hourtrial deferred.
Usercontinueuntilstop; no currentdeadline. FullCOREsettings stillunimplemented.


NOVEMBER20 V23 NOW FLASHED; USB STARTUP PASS.
FreshuserNICENANO/COREclosed/Gloriouswired-OFF readiness. Verifiedunique board/
bootloader0.6.0 and exactee7f5a7a imagehash beforecopy. ProductDIY Mouse Config
HID v23; twohealthyHIDcollections andCDC, zeroerrors. Private startuprecord
config-hid-v23-usb-startup.json. NO FeatureSET/query/DPIwrite/serialopened.
No livecapture/build/helper. Installedv23 intentionallyNOwirelesspointeroutput.
NEXT separatephysicalpopup: COREclosed, GloriousON/USBunplugged/STILL nearreceiver,
otherpointeravailable, receiverunchanged. Run32-bit test_receiver_config.ps1
WITHOUT -ExerciseDpi for READ-ONLY fivequeries first. OnlyafterPASS askseparate
readinessfor temporaryselectedDPIchange/readback/originalrestore. No timecue.
Beforeanywrite persistbackupNeedsWiredRestore; onfailure checkprivatepointer
current-config-hid-backup.txt and usefreshwiredrecovery. RestoreEXACTtestedv14
93eb987a afterbatch withseparatebootloaderpopup andwirelesscontrolcheck.
Usercontinueuntilstop; no currentcutoff. Four-hourtrialstilldeferred.


NOVEMBER20 V23 FIVE READ-ONLY CONFIG QUERIES PASS; NO SETTINGS WRITES.
Initialhelper invocation hit reservedPowerShellPID variable BEFORE AddType/HIDopen
orquery. Renamed to mouseVendorId/mouseProductId and movedassignments before
SelfTest so runtime setup ischecked; offlineSelfTestPASS, sameauthorizedREAD-only
retryPASS withoutnewflash/reboot. Firmwareunchanged ee7f5a7a.
Private resultconfig-v23-20261120-183450-80b8c5f4.json: Success true, five ordered
transactions Version/Profile/Polling/DpiStages/ActiveDpi; firmware1.0.0.0,
activeprofile1,1000Hz, stages400/800/1600/3200 X=Y, selectedstage2=800DPI.
ExerciseDpi false;NeedsWiredRestore false; no backupneeded/created. No serial,
no livehelper/capture/build. V23 stillinstalled, pointeroutputdisabled.
Reusablehosttransactions/replyrelease gate now nominallyhardwarevalidated.
NEXT separatepopup readiness for TEMPORARY selectedDPIstage change/readback and
verifiedoriginalrestore via test_receiver_config.ps1 -ExerciseDpi. COREclosed,
mouseON/unplugged/STILL, otherpointer; receiverunchanged (no resetneeded).
Hostfreshreads/backup BEFORE write; expectedstage2->3->2 ifunchanged. Onfault,
checkprivatecurrent-config-hid-backup.txt NeedsWiredRestore and pauseforWIRED
recovery BEFORE normaluse. Restoreexacttestedv14 93eb987a afterbatch viafresh
bootloaderpopup thenuserwirelesscheck. No generalDPIvalue/lighting/bindingwrites.


NOVEMBER20 FIRST REAL WIRELESS DPI SELECTION WRITE/RESTORE PASS.
UserexplicitReady for temporaryDPIstagechange, COREclosed/mouseONunpluggedSTILL.
Prepared -ExerciseDpi helper Success true; freshbaseline profile1/1000Hz/stage2,
selectedstage3 readbackconfirmed, originalstage2 readbackrestored.800->1600->800.
Nine successfulUSB/RFtransactions inthistrial; priorfiveREAD-onlysameboot had
passed. DpiTrialChanged/Restored true; NeedsWiredRestore false in resultANDbackup.
Private resultconfig-v23-20261120-183851-c34067f2.json, .backup.json; privatepointer
current-config-hid-backup.txt pointsbackup. CHECKflag oninterruptedresume but
thistrialhasNO PENDING RECOVERY. DPIstagevalues/lighting/bindings notmodified.
No serial/livehelper/capture/build/timer. V23 remainsinstalled, pointerdisabled;
mouse lastON/unplugged/still. Selectedstagewrite isrealmouseconfiguration,
not receivergain; no sensorDPIcalibration orfullCOREappsupport claim.
NEXT restore EXACTtestednormalv14 viafreshbootloaderpopup (COREclosed,mousewired/
OFF,double-resetONLYreceiverNICENANO). Image93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052. VerifyUSB,
thenuserwirelessmovement/clickcheck. Afterrestoration continueCOREcompatibility
scope; actualDPIvalue writes/fullprofile/bindings/lighting stillnotimplemented.
Four-hourtrialstilldeferred; usercontinueuntilstop.


NOVEMBER20 EXACT NORMAL V14 RESTORED after v23 DPI-stage trial.
FreshNICENANO/mousewired-OFF readiness; unique board/exact93eb987a hashverified
beforecopy. USBproductDIY Mouse Dongle v14 and onehealthymousecollection verified.
No serialopened. Private core-v23-normal-restoration.json; wirelessusercheck
PENDINGpopup. V23 nolongerinstalled; currentmouseoriginal800DPI restored byradio
readback beforeflash. Private DPIbackupNeedsWiredRestore=false; NO RECOVERYneeded.
No livecapture/build/helper/timer. Hardwaremilestone4ddc36c pushed: real800->1600
->800 selectedstagewrite/restore PASS. FullCOREidentity/settingsUI notyetdone.
Afterbriefwirelesscheck continueCOREtransport scope, no repeatedbaseline/extraction.
Four-hourtrialstilldeferred; usercontinueuntilstop/no cutoff.


NOVEMBER20 OFFLINE CORE STARTUP INVENTORY COMPLETE; WIRED BACKUP NEXT.
Normal EXACTtestedv14 isinstalled/userconfirmedworking; original800DPI restored,
no pendingselectedDPIrecovery. No livehelper/serial/build/capture/timer.
New research/trace_core_startup.js reuses existingprivateModelOSeries source and
factorySupportDevice record with mockHID/DB, NO realapp/nativeaddon/hardware.
InlineassertsPASS:42SET/29GET for threeprofile factoryinitialization+receiverFWread.
Private tracecore-startup-trace-1795172092145.json; counts documented in
 docs/plans/2026-11-20-core-startup-coverage.md. User profiles may differ.
COREwrites24bindings,3DPIlists,3DPIcolors,3LOD,3poll,3debounce,profile/selectedstage
onconnect. Need backup/commandcoverage/inputforwarding beforeofficialPID.
Installedpackage1.1.36, envmissingBuiltType defaults0; traceuses that codepath.

Prepared research/backup_mouse_profiles.ps1 READ-ONLY wired helper:40queries,
3profiles,24slot0 bindingrecords plusDPIstages/selection/colors/debounce andglobal
version/activeprofile/poll/LOD. Getter shapes checked againstexistingmouse dump;
NO brightnessgetter/address/bootloader/macrobodies. SelfTestPASS with40request
coverage, getter-onlyopcodes, echoes/profiles/slots/lengths, malformed rejection.
No firmwarebuild/flash needed. Backup savesprivateopaque rawreplies; onlycomplete
successupdates current-core-profile-backup.txt. Fullrestoretool notyetimplemented.
NEXT physicalpopup: closeCORE, connectGloriousUSB inWIREDmode; leaveDIYreceiver
unchanged. Then32-bit backup_mouse_profiles.ps1, validate40responses andsave
resultsprivately. No settingswrites; normalreceiverremainsv14. Afterbackupcontinue
COREallowlist/keyboard-consumer-forwarding implementation; do not launchCOREyet.
Usercontinueuntilstop; nocutoff. Four-hourtrialstilldeferred.


PAUSED BY USER November20: selected "Pause here - keep normal wireless operation"
on wiredprofile-backup readiness popup. Resume ONLYwhenuser returns.
Normal EXACTtestedv14 (93eb987a) remainsinstalled/userconfirmedworking, original
800DPI restored; NO pending selected-DPI recovery. No serial/capture/build/helper/
timer running. No wiredprofilebackup executed; no furtherconfiguration changes.
Latestsoftwaremilestonec630442 pushed: offlineCOREstartup inventory and READ-only
backuphelper. V23 real800->1600->800 selection/restore PASS milestone4ddc36c.

NEXT SESSION: read docs/plans/2026-11-20-core-startup-coverage.md and
research/CORE_COMPATIBILITY.md, asknewphysicalreadiness for COREclosed + Glorious
USBconnectedinWIREDmode; leaveDIYreceiverunchanged. Run32-bit SysWOW64
powershell -NoProfile -ExecutionPolicy Bypass -File research/backup_mouse_profiles.ps1.
40read-onlyqueries coverglobals plus3profilesDPIstagevalues/selection/colors/
debounce and24slot0bindings. Rawbackupprivate; onlySuccessupdates
build-pairing-analysis/current-core-profile-backup.txt. No RFaddress/macrobodies/
settingswrites/flash. Validateall40responses beforeclaimingcompletebackup.
Fullprofileautomatedrestoretoolnotyetimplemented; don'tenableCOREidentityyet.

Offlineactualvendor-source factorytrace42SET/29GET (mockHID/DB, no realpackets)
showed CORE initializesall3profiles automatically. Actualsaveduserprofilesmay
vary. Private tracecore-startup-trace-1795172092145.json. Backuphelpspreserve
mouseconfiguration beforeallowlistingfullstartup commands/keyboard-consumer
forwarding/officialidentity. GeneralRGBdisabledinCOREmodel;DPIcoloursseparate.
Four-hourtrialremainsdeferred for COREappcompatibility. Fullappsettings/realDPI
valueediting/bindingsstillunfinished. No promisesofcompletionbeforeNov21.

Exact testednormalrollback: build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Don'treflash/rebuild/re-extractcompletedmilestones. Preserveignoredcaptures,
addresses, proprietarysource/binaries; leave .vscode/ and INFO_UF2.TXT untouched.
Keepcontext/resumeprompt current; pushmilestones onfeaturebranches. Currentbranch
feature/glorious-core-compatibility. Popup/pause forphysicalintervention.


NOVEMBER20 WIRED BACKUP ATTEMPT STOPPED BEFORE ANY QUERY: NO WIRED MOUSE PRESENT.
UserconfirmedCOREclosed/wiredreadiness, but backup_mouse_profiles.ps1 couldnot
find one wiredconfiguration interface. Present-onlyWindowsPNP showsNO VID258A
devices;32-bitHidSharptarget258A:2018 count0. No streamopened, no FeatureSET,
no settingswrite, no backupcreated. Private readinessfailure observation saved
core-profile-backup-readiness-failure.json. No livehelper/serial/capture/build.
Normalreceiverv14 remainsunchanged; lastknown800DPIrestored/no pendingrecovery.
NEXT physicalpopup: reconnectGloriousUSBdatacable atbothends directlyPC, tryknown
workingdatacable/portifneeded, keepCOREclosed andreceiverunchanged. SelectReady
oncewiredconnectionestablished; recheckpresentUSBbefore40read-onlybackup.
Do not alterfirmware orretryquerieswithoutthewireddevice. Usercontinueuntilstop.


NOVEMBER 20 — V24 WIRELESS PROFILE-BACKUP CANDIDATE READY; NOT FLASHED.
Normal exact tested v14 remains installed and working. User reports active mouse
use; Windows still sees no Glorious wired USB device after reconnection. No wired
stream or query was opened. Keep CORE closed. Continue until user says stop.

V24 expands canonical GET coverage for the 40-query profile backup. The existing
profile-1 selected-stage setter remains restricted, but this backup operation
sends GETTERS ONLY and changes no settings. No new write trials while wired
recovery is unavailable. No pointer output in this diagnostic.
Build finished successfully: all 13 tests PASS, staged source hashes match,
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v24.
197 UF2 blocks, 0x26000..0x32500 exclusive; 50188 flash / 16896 RAM.
Preserved private image build-pairing-analysis/config-hid-v24-875a85b9.uf2
SHA256 875a85b941cd15930f13b19e1a1eabacacc00c2133dcb2f8abe0db33de4d1a15.
Both backup helper self-test modes passed. No flash, settings write, serial,
capture, helper, build or timer remains running. Hardware getter coverage for
all profiles is still unqualified. Full automated profile restore is not ready.

NEXT: fresh popup/pause for CORE closed, Glorious wired or OFF, double-reset ONLY
receiver into NICENANO. Verify board and exact preserved image before flashing.
Check passive USB startup, then separate readiness for Glorious ON / USB unplugged
and STILL near receiver. Use 32-bit SysWOW64 PowerShell to run
research/backup_mouse_profiles.ps1 -Wireless. Validate all 40 responses; only full
success updates private current-core-profile-backup.txt. On failure preserve
partial evidence, do not retry blindly or change settings. After this batch,
popup/pause to restore EXACT tested normal v14 image dongle-v14-93eb987a.uf2,
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052,
verify USB identity and ask for wireless movement/click check. No pending DPI
recovery: original 800-DPI selection previously verified restored.
CORE identity/full settings/key forwarding remain unfinished; four-hour trial
is deferred. No app launch or stock identity until required coverage qualifies.


NOVEMBER 20 — V24 FLASHED; PASSIVE USB STARTUP PASS; BACKUP NOT STARTED.
Fresh user readiness confirmed CORE closed, Glorious wired/OFF, NICENANO visible.
Verified board and exact frozen v24 hash875a85b9 before flash. Windows reports
DIY Mouse Config HID v24, healthy composite/mouse/vendor HID/CDC COM12, no errors.
Private core-v24-startup.json saved. No serial opened or configuration query sent.
Installed firmware is now V24, NOT normal v14; wireless pointer output disabled.
NEXT popup/pause: keep CORE closed, Glorious ON/USB unplugged/STILL near receiver,
other pointer available; leave receiver connected unchanged. After Ready run
32-bit SysWOW64 PowerShell research/backup_mouse_profiles.ps1 -Wireless once.
Validate 40 read-only responses. No settings changes or new write trials. After
batch restore exact tested normal v14 through fresh NICENANO readiness and user
wireless check. Continue until user stops; no live build/capture/serial/helper.


NOVEMBER 20 — V24 WIRELESS PROFILE BACKUP HARDWARE PASS; RESTORE V14 NEXT.
Fresh stationary mouse readiness; 32-bit backup_mouse_profiles.ps1 -Wireless
completed all 40 read-only USB/RF queries successfully. Private result
mouse-profile-backup-20261120-205523-d2d97068.json; private pointer
build-pairing-analysis/current-core-profile-backup.txt updated only on success.
Raw request/reply lengths, getter opcodes, status/header/profile/slot/counts
checked. All three profiles have four stages: 400/800/1600/3200 X=Y (big endian).
Selected stages: profile1=2 (800), profiles2/3=3 (1600). All 24 slot0 binding
records preserved; DPI colours, debounce, global polling/LOD/version included.
No settings writes occurred. Full-device restoration and macro-body backup are
NOT qualified. CORE must remain closed; do not enable stock identity yet.
V24 remains installed with NO pointer forwarding. Mouse last ON/unplugged/still.
No live serial/capture/build/helper/timer. No pending DPI recovery.
NEXT PHYSICAL POPUP: Glorious wired or OFF, CORE closed, double-reset ONLY
receiver into NICENANO. Restore EXACT tested normal v14 image
build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
verify healthy USB product DIY Mouse Dongle v14, then ask wireless movement/click
check. After restoration continue CORE command coverage/input forwarding work;
no repeated extraction/build/baseline. Continue until user says stop.


NOVEMBER 20 — EXACT NORMAL V14 RESTORED AFTER V24 BACKUP.
Fresh NICENANO readiness, board identity and preserved 93eb987a image hash
verified before copy. Runtime product DIY Mouse Dongle v14 and healthy Windows
mouse/CDC USB devices verified. No serial opened. Private evidence:
build-pairing-analysis/core-v24-normal-restoration.json. User wireless movement/
click check PENDING next popup. V24 no longer installed; backup passed all40 reads
and remains private (milestone06e36c8 pushed). No settings changes, no pending
DPI recovery. Current original profile1 selected stage2 = 800 DPI.
No live build/capture/serial/helper/timer. Continue until user says stop.
After user control confirmation, resume CORE compatibility software work:
qualify bounded startup setters and required keyboard/consumer forwarding before
stock identity/app launch. Read docs/plans/2026-11-20-core-startup-coverage.md.
Wired USB still unresolved; do not assume wired recovery is available or start
new configuration write trials without a qualified restoration path.


NOVEMBER 20 — NORMAL V14 RESTORATION USER-CONFIRMED; RECOVERY CONNECTION NEXT.
User confirmed automatic wireless movement/click after restoring exact normal
v14 (93eb987a). Private core-v24-normal-restoration.json now records confirmation.
No live serial/capture/build/helper/timer; no pending settings recovery. Backup
milestone 06e36c8 is pushed, all 40 configuration reads passed, no settings changed.

Review confirms current normal firmware forwards mouse input only. Before CORE
button/key remapping, auxiliary keyboard/consumer/system outputs still need
implementation and neutral/busy/retry qualification. Do not claim CORE support.
Also establish a usable independent recovery connection before new profile write
trials: previous USB cable attempts yielded no present wired Glorious device.
NEXT physical popup: CORE closed; leave DIY receiver unchanged, switch Glorious
wireless power OFF, connect it directly using a known USB DATA cable. Check
whether wired movement works with wireless switched OFF. This distinguishes
actual wired data from continuing receiver input. Recheck present 258A:2018 and
65-byte Feature interface before sending any read-only verification. No flashing,
settings writes, CORE launch or change to Windows/DPI settings for this check.
If wired data remains unavailable, document the blocker and continue safe
auxiliary software work; do not assume wired restoration is available.
User continue until stop. Keep private backup/binaries ignored and context current.


NOVEMBER 20 — WIRED RECOVERY CONNECTION RESTORED AND READ-VERIFIED.
User confirms wired movement with wireless switch OFF. Windows sees one healthy
258A:2018 mouse with mouse/keyboard/consumer/system/vendor collections. CORE
closed; normal receiver v14 untouched. 32-bit wired backup helper completed40
read-only queries; private mouse-profile-backup-20261120-210214-9dfacd49.json.
All40 raw response records match the earlier wireless backup exactly. Current
private backup pointer now references wired copy. No settings writes occurred.
Independent wired data/Feature access is now available; actual DPI-value restore
still must be qualified. No pending recovery or live helper/serial/capture/build.
NEXT software milestone: prepare v25 narrowly scoped profile1 four-stage DPI
value edit/restore, with backup before any mutation and separate physical flash
and temporary-write readiness. CORE remains closed; no full app identity yet.
User continue until stop. Normal v14 installed and user-confirmed working.


NOVEMBER 20 — V25 DPI-VALUE CANDIDATE VALIDATED; NOT FLASHED.
Normal exact tested v14 remains installed/user-confirmed working. Glorious last
wired with wireless switch OFF; CORE closed. Wired 258A:2018 healthy, all40 wired
backup reads passed and match wireless backup exactly. No pending recovery.
No live build/serial/capture/helper/timer. User continue until stop.

Prepared v25: same Feature/RF transport, no pointer output, prototype identity.
Adds only profile1 four-stage DPI-value setter group1/op1,18-byte payload,
big-endian X/Y50..19000 in50-DPI steps, zero padding. Exact returned setter
payload must match. Existing selected-stage writer remains restricted1..4.
Native failing test reproduced old rejection before implementation; all13 native
tests PASS after change. Shared packet parser and host helper self-tests PASS,
including ambiguous change/readback/restore/selection failures. No settings writes
occurred in preparation; hardware setter/restore not qualified yet.

research/test_receiver_config.ps1 -ExerciseDpiValues requires exact v25 USB
product. Saves schema2 original values/selection with NeedsWiredRestore=true
BEFORE mutation. Changes one INACTIVE stage by50 DPI, reads all values, then
finally restores original list, reads it back, restores selection and verifies.
Only full readback clears pending flag. On failure pause for fresh wired recovery:
32-bit test_receiver_config.ps1 -RestoreWired -BackupFile saved.backup.json;
checks saved values, mouse version and active profile1 before value recovery.
Do not blindly retry an ambiguous RF write. CORE stays closed.

Build log config-hid-v25-build.log; staged code/config hashes match current.
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v25,2HID,1ms.
50300 flash /16896 RAM;197 blocks,0x26000..0x32500 exclusive.
Preserved private image build-pairing-analysis/config-hid-v25-f577b7e0.uf2
SHA256 f577b7e095a9297da3d2fb34218fe5ba4646d1b36f2c7882041e9461f68b2426.
NEXT fresh popup CORE closed, Glorious wired/OFF, double-reset ONLY receiver to
NICENANO; verify board/exact image before flash. Check USB startup, then separate
stationary wireless mouse readiness that explains temporary inactive-stage edit
and verified restoration. Run32-bit test_receiver_config.ps1 -ExerciseDpiValues
ONLY after readiness. On failure preserve private pending backup and pause for
wired recovery before restoring normal receiver. After batch restore EXACT
normal v14 via fresh bootloader readiness, verify USB and user movement/click.
Normal rollback build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Full CORE identity/keyboard-consumer forwarding/lighting/binding writes remain
unfinished. Four-hour trial deferred. No new baseline/extraction/build needed.


NOVEMBER 20 — V25 FLASHED; PASSIVE USB STARTUP PASS; NO DPI EDIT YET.
Fresh user NICENANO readiness, CORE closed/Glorious wired-OFF. Board and exact
f577b7e0 image verified; Windows product DIY Mouse Config HID v25 healthy, mouse
and vendor Feature collections present. Private core-v25-startup.json. No serial
opened, no query or settings write during flash/startup. No live build/helper.
NEXT separate physical readiness: CORE closed, Glorious ON/USB unplugged/STILL
near unchanged receiver, other pointer available. Explain test temporarily edits
one inactive profile1 stage by50 DPI, saves originals first, then restores full
stage list and original selected800-DPI stage with readback. Pointer disabled.
After Ready run32-bit research/test_receiver_config.ps1 -ExerciseDpiValues once.
Check result and backup NeedsWiredRestore flag. If pending, stop and popup for
wired recovery (mouse wired with switchOFF, receiver unchanged), then run32-bit
helper -RestoreWired with exact pending backup. Do not flash normal before pending
mouse settings recovery is resolved. After successful edit/restore, fresh NICENANO
popup to restore exact normalv14/93eb987a, then USB and user movement/click check.
Normalv14 is currently NOT installed. All40 configuration records previously
matched wireless and wired backups; CORE full support still unfinished.


NOVEMBER20 — V25 REAL DPI-VALUE EDIT AND RESTORE HARDWARE PASS.
Fresh user temporary-write readiness; 32-bit test_receiver_config.ps1
-ExerciseDpiValues completed successfully. Profile1 inactive stage4 changed
3200->3250 X=Y, complete four-stage list read back, then restored400/800/1600/3200
X=Y with full readback and original selectedstage2=800 restored/read back.
Private config-trial-20261120-211429-90d63770.json and its .backup.json:
Success/Changed/Restored true; NeedsWiredRestore false IN BOTH. Schema2 backup
pointer current-config-hid-backup.txt saved before mutation. Exact write echo
payloads checked; no pending mouse recovery. Independent wired recovery code
self-tested and data connection verified, but this trial did not need it.
V25 remains installed, pointer disabled; mouse last ON/unplugged/STILL. COREclosed.
No live serial/capture/build/helper/timer. NEXT fresh physical popup: Glorious
wired or OFF, double-reset ONLY receiver to NICENANO. Restore exact normalv14
build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
verify USB then user wireless movement/click check. Full CORE app/bindings/lighting
still unfinished. This proves configuration value transport/readback, not physical
sensor DPI calibration. User continue until stop; four-hour trial deferred.


NOVEMBER20 — EXACT NORMAL V14 RESTORED AFTER V25 DPI-VALUE PASS.
Fresh bootloader readiness; pending DPI backup flag checked false before flash.
Board/exact93eb987a image hash verified. Runtime product DIY Mouse Dongle v14,
healthy mouse/CDC devices verified, no serial opened. Private evidence
core-v25-normal-restoration.json; user wireless check NEXT/PENDING.
Original profile1 list400/800/1600/3200 and selectedstage2=800 were restored and
read back before flash; no pending recovery. Hardware DPI edit milestone3e1089e
pushed. No live helper/build/capture/serial/timer. CORE closed.
NEXT brief user wireless movement/click confirmation. Then continue CORE startup
coverage: remaining setters, input forwarding and backup-aware recovery. Current
normal v14 forwards mouse reports only; full CORE support not claimed. Independent
wired USB data/Feature connection now works and all40 wired settings match radio
backup. No need repeat completed builds/flash/extraction/baselines. User untilstop.


NOVEMBER20 — V25 NORMAL RESTORATION USER-CONFIRMED; WIRED HID LAYOUT NEXT.
User confirmed automatic normalv14 wireless movement/click after v25; private
core-v25-normal-restoration.json updated. Original400/800/1600/3200 stage list
and selected800 verified restored; NO pending recovery. Exactnormalv14 remains
installed, no live build/helper/capture/serial/timer. CORE stays closed.

Prepared research/inspect_wired_hid.ps1 read-only Windows/HidSharp descriptor
inspection for258A:2018. Synthetic descriptor self-test covers reportID/length,
keyboard usage and field width/count/offset. No streams opened, no FeatureSET,
settings changes, flashing or input recording. Output private; no unique device
paths printed. Reports are library Windows descriptor views, not raw USB capture;
usage lists bounded16 entries with total count, payload offsets exclude reportID.
NEXT fresh popup: switchGlorious wirelessOFF and connect directly via working
USBdata cable, COREclosed; leaveDIYreceiverunchanged. AfterReady verify own
wired collections and run32-bit research/inspect_wired_hid.ps1. Preserve partial
failure if Windows disallows descriptors; no blind firmware/driver changes.
Use actual keyboard/consumer/system field layout to prepare auxiliary USB input
forwarding, including busy retries and neutral release, before CORE keybindings.
No new configuration write is part of this step. Full CORE remains unfinished.


PAUSED BY USER — NOVEMBER20, AFTER V25 NORMAL RESTORATION.
User selected "Pause here — keep normal wireless operation" at read-only wired
HID-layout readiness. STOP project work until user explicitly returns.
Exact tested normalv14 remains installed and user-confirmed working. Original
400/800/1600/3200 DPI values and selectedstage2=800 restored/readback verified;
no pending settings recovery. No live serial/capture/build/helper/timer.
Last pushed software checkpoint5b051a3; DPI-value hardware milestone3e1089e.
Prepared inspect_wired_hid.ps1 self-test PASS; no live layout inspection started.
NEXT SESSION: read newest handoff and CORE_COMPATIBILITY.md. Ask fresh readiness
for COREclosed, Glorious wirelessswitchOFF and working wiredUSBdata connection,
receiver unchanged. Then32-bit research/inspect_wired_hid.ps1 read-only descriptor
inspection. No Feature requests/settingswrites/flash/inputrecording. Keep normal
v14 installed. This supports future keyboard/media/system forwarding for CORE
key remapping. Full CORE app compatibility remains unfinished; four-hour trial
deferred. Preserve private backups/captures/addresses/binaries ignored. Keep
context and resume prompt current; push milestones on feature branches.


RESUMED NOVEMBER 21 by user: continue where we left off.
Repository checkpoint aded995 on feature/glorious-core-compatibility; only
unrelated .vscode/ and INFO_UF2.TXT untracked. Saved pause is revoked by explicit
resume. No hardware action, serial session, configuration query, build or capture
started during resume review. Last confirmed installed firmware is exact normal
v14 with original 400/800/1600/3200 DPI values and selected800 restored; no pending
settings recovery. V25 real DPI-value edit/restore passed (3e1089e), normal v14
restoration was user-confirmed. Full CORE app support remains unfinished.
NEXT physical readiness popup: CORE closed; Glorious wireless switch OFF and
connected directly by the working USB DATA cable, wired movement verified;
DIY receiver unchanged. Then run prepared32-bit research/inspect_wired_hid.ps1
read-only Windows HID layout inspection. No settings requests, flashing, streams
or input recording. Keep report output private; distinguish library Windows
views from raw USB capture. If unsupported, preserve partial evidence and report
limitation before choosing fallback. Use layouts for later auxiliary keyboard,
media and system report forwarding; do not enable CORE identity yet.
User preference continues until user says stop; popup/pause for intervention.
Keep context/resume prompt current, push tested milestones on feature branches.


PAUSED BY USER NOVEMBER21 — FUTURE RESUME MAY USE A DIFFERENT OPENAI ACCOUNT.
User requested stop until a later date. No hardware work after readiness check:
Windows wired258A:2018 root count0; inspect_wired_hid.ps1 NOT run. No settings
request, flash, capture, build or live serial/helper/timer. Normal exact tested
v14 remains last user-confirmed working. V25 original400/800/1600/3200 DPI list
and selectedstage2=800 restored/readback verified. Private current-config-hid
backup checked: NeedsWiredRestore=false. No pending recovery.
Keep project PAUSED until user explicitly resumes. Later account should open the
SAME local workspace, read RESUME_PROMPT.md and this newest handoff, and continue
from files rather than assuming access to this account's chat history.
NEXT fresh physical popup: COREclosed, Glorious wirelessswitchOFF, connected via
working USB DATA cable with wired movement confirmed; leaveDIYreceiverunchanged.
Then run32-bit research/inspect_wired_hid.ps1 read-only Windows descriptor view.
No settings requests/flash/inputrecording. This supports keyboard/media/system
forwarding for CORE remapping; full CORE app support remains unfinished.
Retain the WHOLE local folder, including ignored build-pairing-analysis and .git.
GitHub alone omits private RF parameters, backups, source evidence and rollback
UF2s needed by this hardware project. Do not publish those ignored files.
Same Windows user/computer also retains external Zephyr SDK/toolchain under
C:\Users\Gabriel\zephyrproject. A different computer requires environment and
private-path verification before any hardware work. Branch remains
feature/glorious-core-compatibility. All milestones should be committed/pushed,
context/resume prompt updated for meaningful steps, and physical intervention
requested with popup/pause. User prefers autonomous progress until intervention
or an explicit stop. Four-hour reliability trial deferred for CORE compatibility.
