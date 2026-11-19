Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (newest LATEST HANDOFF / NEXT), README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-19-glorious-core-compatibility.md.

This message resumes the project after my requested break. Keep context and this
copy/paste prompt current, commit/push milestones on feature branches, pause
with question popups for physical intervention. Keep captures, RF addresses,
proprietary sources and binaries ignored. Leave .vscode/ and INFO_UF2.TXT alone.


Saved break state - November 19. This prompt explicitly resumes work.
No physical intervention requested now. No serial/capture/build/timer/helper is
running. Normal TESTED v14 remains installed (93eb987a frozen image), unchanged.
Mouse last confirmed WIRED, Glorious CORE closed. No settings changed.

Latest completed steps: wired READ-only reference succeeded: firmware1.0.0.0,
1000Hz setting, profile1 stages400/800/1600/3200, active stage2=800DPI.
Private observation: build-pairing-analysis/core-wired-settings-reference.json.
Built/UF2-validated v20 READ-only RF diagnostic; native12 checks pass including
actual ACK/no-HID/x-stop integration, plus capture-parser self-test. It sends
only one fixed firmware-version query; no pointer output, settings writes,
CORE-compatible vendor interface or mouse bootloader/updater commands.
Private frozen: config-read-v20-68f92a56.uf2; SHA256
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632.
Product DIY Mouse Config Read v20;191blocks;0x26000..0x31F00;48828flash/16512RAM.
NOT FLASHED/HARDWARE-TESTED. Source/config metadata and logs ignored.
Normal isolation rebuild also PASS, experimental flagOFF; normalcontrol SHA256
6877579cdeb4bfd5587894f7e238dcf31eb1c6ddcfce0ddf0ecf3704c5ca82c7,
46188flash/16064RAM. That control is NOT hardware-tested and NOT the rollback.

On resume: read research/CORE_COMPATIBILITY.md and November19 CORE plan. Ask
popup for v20 flash readiness: Glorious wired/OFF, receiver double-reset into
NICENANO. Verify board and exact preserved v20 hash before copy; check passive
USB startup. Then separate mouse-OFF/unplugged readiness and capture-running
cue for research/capture_config_read.ps1 (one reader, no p, x35s/d frozen summary,
closes by40s). Mouse should stay STILL; diagnostic has no wireless pointer output.
Compare radio version reply with wired reference; no speculative config writes.
Restore EXACT TESTED v14 (93eb987a) after trial or failure, using separate physical
readiness. Keep CORE CLOSED throughout v20; it does not support CORE detection.
Do not repeat completed extraction/build/baseline. Working branch:
feature/glorious-core-compatibility. Four-hour trial remains deferred; user
prioritizes official CORE compatibility before soak. Separate app fallback.
Deadline target before November21 is not guaranteed. General lighting disabled
in installed CORE's model record; DPI indicator colours separate.


RESUMED November 20 by user; supersedes November19 PAUSED status.
Re-read handoff/CORE findings/plan. Git7899ecf on feature/glorious-core-compatibility;
only unrelated .vscode/ and INFO_UF2.TXT untracked. Rechecked preserved v20 SHA256
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632 and exact
hardware-tested v14 rollback SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Both match. No rebuild, flash, settings change or serial session. No background
capture/build/helper/timer. Last known installed normalv14; current mouse/cable
state needs fresh readiness. NEXT pending popup: keep CORE closed, mouse wired/OFF,
double-reset only receiver into NICENANO. After Ready verify board and copy exact
v20, passive USB check, then separate stationary wireless read-query readiness.
V20 has NO wireless pointer output and only a fixed firmware-version READ.
Restore testedv14 after this diagnostic batch. Four-hour trial remains deferred.


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
