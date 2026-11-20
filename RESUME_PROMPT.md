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


V23 software milestone397f156 pushed; no live sessions or pending recovery backup.
Normal testedv14 remainsinstalled/userconfirmed. V23 stillNOTflashed. Nextstep
is freshNICENANO readiness, then verifiedv23flash and READ-ONLY multi-querytrial;
temporaryDPIselection mustwaitfor separate readiness afterreadsPASS. EOF-only
protocol-helper tidy doesnotchange testedlogic. No completedbuild/test repeats.


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
