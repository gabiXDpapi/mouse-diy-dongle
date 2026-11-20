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
