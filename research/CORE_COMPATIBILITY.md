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
