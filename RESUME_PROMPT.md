Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read newest LATEST HANDOFF / NEXT in PROJECT_CONTEXT.md, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-20-usb-config-read.md. This message resumes the project;
ask a new break time if the November20 5AM cutoff has passed. Current branch:
feature/glorious-core-compatibility. Keep context/prompt current, commit/push
milestones, pause by popup for physical work; captures/addresses/binaries ignored.
Leave unrelated .vscode/ and INFO_UF2.TXT alone. No completed extraction/builds/
baselines repeated. CORE compatibility precedes four-hour trial; app fallback.


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
