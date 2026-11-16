V14 NORMAL FLASHED / USB ENUMERATION PASS - 2026-11-17 01:24 Manila:
PhysicalreadyGloriouswired/OFF. VerifieduniqueNICENANOnRF52840-nicenano/S1406.1.1
andnormalUF293eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
beforecopy. USBrootproductreadback DIY Mouse Dongle v14,oneHID+oneCDC,statusOK.
Private dongle-v14-flash-validation.json saved. NO serial reader opened,NO DTR/
armcommand. InstalledhardwareNOWNORMALv14;automaticreceiverlisteningexpected,
actualRFinput NOTyettested. Notunarmed/passive/expired. V13rollbackpreserved.
No live build/serial/raw/helper/UAC. Branchfeature/receiver-plug-and-play,software
3f5934cpushed. Deadline02:00AMManilaNov17 (UTCNov16T18:00Z).
NEXT separateinputreadinessfor18minnormaltrial:harmlessscrollabledoc/otherpointer,
GloriousOFF/USBunplugged,unchangedDPI/settings. Receiverleaveconnected. Raw-only
capture1160s(hidden,mousereadonly,NOCDC/arm). CueONthenallfivebuttons/wheel/motion.
At~1min RELEASEallbuttons thenunplug/replugONLYreceivernormally(no double reset),
mouseON;verifyautomaticcontrolrecoverywithoutserial. Mouse-onlyOFF3s/ONat5/10min.
Checkcontrolsagainat16and18min,thus>15minafterreceiverreplug. At18minmouseOFFand
reconnectUSB;normaldongledoesNOTautostopwhencaptureends. NoheldUSBunplugtrial.
Needstartby01:37tofit1160s+wrapby02:00;otherwisepausewithoutnewtrial. Usemanual18min
timer;oldfive-minuteHTMLtimerunsuitable. Collectusercontrols/automaticstartup/
USBreplug/bothmousecycles/continuedactivityafteroldlimit/nounexpectedstops.
Rawcapturefilehostcountsarenotphysicalpolling/latencycertification;normalmodehas
noserialdiagnosticsbydesign. Do not useboundedv13serialcontrolleronthisimage.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-17-plug-and-play.md.
Original implementation: docs/plans/2026-11-15-wireless-hid-prototype.md.
Inspect git status/log first. Current feature/receiver-plug-and-play.
Keep context and this copy-paste prompt current after each meaningful step;
commit/push tested milestones. Use feature-based branches. Leave unrelated
.vscode/ and INFO_UF2.TXT untracked. Private captures/addresses/binaries ignored.

V14 PLUG-AND-PLAY SOFTWARE READY / FLASH NEXT - 2026-11-17 01:15 Manila:
User explicitly requests normal dongle operation: USB plug automatically activates
until unplug. This supersedes previous finite-only/manual-start and passive USB
reconnect preferences FOR THE NEW NORMAL BUILD. Current branch:
feature/receiver-plug-and-play. Installed hardware STILL v13, disarmed/neutral;
no v14 flash/arming/hardware trial yet. No live build/serial/raw/UAC/helper session.

New App dongle / NORMAL_DONGLE=ON starts after USB configuration without DTR or
host command. No discovery-arm timeout, session duration, ACK or discovery cap.
One-second successful-ACK gap neutralizes then keeps rediscovering. Optional x
stays stopped until explicit p or reboot. USB reset/reconnect can autoactivate
again; USB/radio/HID errors fail closed. Telemetry wrapping does not affect link.
Normal overlay disables UART console, printk and boot banner to avoid CDC output
blocking with no reader. CDC input is optional. RF bytes/timing, mapping and idle
ACK hold are unchanged. Bounded App receiver remains available and now permits
explicit fresh p only after completed neutral stop; no active-session renewal.

Tests: new expiry rearm regression failed on v13; normal-mode regression failed
old 30s guard before implementation. All8 tests PASS, protocol subtests cover
probe/receiver/dongle, no-command startup, offline/fault gates, old limits,
telemetry wrap, gap recovery, x latch, fatal TX and neutral-failure cases. All four
firmware builds PASS. Final staged sources/config match; NORMAL only ON for dongle.
Normal PRINTK/UART_CONSOLE/BOOT_BANNER disabled, HID+REBOOT enabled, interval1ms.
Normal UF2 SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
46124 flash/16064 RAM,181 blocks,0x26000..0x31500 end exclusive. Preserved ignored
build-pairing-analysis/dongle-v14-93eb987a.uf2. Earlier ddd815ea image was superseded
by final USB-reset-during-init fix; DO NOT flash that stale image.
Bounded v14 receiver SHA256
5aadca1a29a17d8fc25f022d4309e24159fd12d7aa25a1bd06882b25b42b3696,
62268 flash/16128 RAM,244 blocks,0x26000..0x35400; preserved separately. Probe
SHA e51374e433090fadbecf3092a3f76e002bef742279d062cd535dac5965d0b26f;
radio SHA014953ed153a2cc50ca0577278be7fbd0154961faedb86233c57230b2c169438.
Default HID hash unchanged. V13 rollback396d7dd8 full hash verified preserved.
Private software metadata v14-software-validation.json ignored. Build15557 and
final normal rebuild25478 finished; no more builds needed for current sources.

NEXT physical flash popup: Glorious wired/OFF, double-reset ONLY receiver to
NICENANO. Verify unique bootloader/board + exact normal image before copy. The new
image AUTOACTIVATES; do not claim passive/unarmed startup. Verify USB enumeration
and product DIY Mouse Dongle v14 without opening CDC. Then separate harmless-doc/
other-pointer readiness, mouse OFF/USB unplugged; raw-only host capture, no serial
reader or arm command. Test automatic controls/powercycle then safe USB replug,
all buttons released first. Longer normal trial must exceed old15min window.
Old capture_receiver_session.ps1 targets bounded v13 and MUST NOT be used for
normal v14; normal has intentionally no serial diagnostics. Save/push every tested
milestone and keep private images/captures ignored. Deadline02:00AM ManilaNov17,
UTC2026-11-16T18:00Z; refresh if later. Abrupt unplug held-state known from both
DIY and wired reference; no physical polling/latency or long-term certification.

Preserved hardware baseline before new flash:
V13 two separate15min runs each passed both mouse-only OFF/ON recoveries and
original deadline clean stop;not one continuous30min run (fb26dad,2b7dabd).
All five buttons/wheel validated (f8a922f):front thumb Windows5,rear Windows4.
Foreground motion (4d9a89f) and intended desk position (f51f6d7) smooth/cleanstop;
desk39356events,median1.003ms,peak1000/fullsecond. Host timing/manual motion only;
distance/obstaclesunknown,not physical polling/loss/latency proof.
Connected x/timeout heldLEFT release previously passed. Abrupt physical unplug
held-state matched wired reference on thisPC/app;wired PnP restart not compared.
V12 startup-neutral did not fix it;v13 restoredv11 bridge. Do not revive that guess.
Use question pop-ups for physical intervention and pause so user notices. Do not
repeat extraction, completed baselines/builds/flashes without a reason. Never
reuse old one-arm/passive rules for the new normal build: user's latest request
explicitly authorizes automatic continuous operation until unplug.
