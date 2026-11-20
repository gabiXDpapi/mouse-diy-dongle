Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-20-reusable-config.md. Branch feature/glorious-core-compatibility.
Keep context/prompt current and commit/push milestones; popup/pause for physical
intervention. Keep captures, addresses, proprietary code and binaries ignored.
Leave .vscode/ and INFO_UF2.TXT untouched. User says continue until toldstop.


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
