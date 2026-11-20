Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (newest LATEST HANDOFF / NEXT), README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-20-usb-config-read.md.

This message explicitly resumes the saved pause. Keep context and this copy/paste
prompt current. Commit/push tested milestones on feature branches. Use popup
pauses for physical intervention. Keep captures, RF addresses, proprietary source
and binaries ignored; leave .vscode/ and INFO_UF2.TXT untouched.


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


RESUMED November20 ~17:34Manila by explicit user request. Supersedes saved pause.
Rechecked saved v21 a7631b8d and exact tested normalv14 93eb987a image hashes;
both match. Git10b0f8b, feature/glorious-core-compatibility; only unrelated
.vscode/ and INFO_UF2.TXT untracked. No rebuild/extraction/flash/settings change.
No serial/capture/build/helper/timer running. Last known installed normalv14,
user-confirmed working. V21 remains NOT FLASHED/HARDWARE-TESTED.
Earlier05:00AM break/04:50 flash cutoff expired; ask new break time, do not assume
an ongoing deadline. NEXT popup: COREclosed, Gloriouswired/OFF, double-resetONLY
receiver intoNICENANO. Verifyidentity/hash and flash exactv21, passive USBcheck;
separate GloriousON/unplugged/STILL readiness before hostFeature query. V21 has
NO pointer output and one fixed version READ/boot; no CORE detection/settings
writes. Afterquery restore exacttestedv14 with physicalreadiness and controlcheck.
Four-hour trial remains deferred for CORE compatibility.


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
