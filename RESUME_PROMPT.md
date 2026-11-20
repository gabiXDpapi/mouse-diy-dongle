Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-20-usb-config-read.md. Branch feature/glorious-core-compatibility.
Keep context/prompt current and push tested milestones. Popup/pause for physical
intervention. Keep captures, addresses, proprietary source/binaries ignored;
leave .vscode/ and INFO_UF2.TXT untouched. User says continue until toldstop.


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
