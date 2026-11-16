V13 INTENDED DESK POSITION PASS / NEXT MILESTONE CHOICE - 2026-11-17 00:52 Manila:
User confirms receiverinintendednormaldeskposition,normalmousegrip/spot,smoothmotion
andcleanstop;distance/obstaclesunknown. Full100sserial/rawexit0,39356receiverrecords
ALLforeground,readerrors0/failed0. Onep,onelink,onex,neutralstop/disarm,noERROR.
Hostmedian1003us,p90=1222us,p99=4893us,max418994us;peak1000events/fullsecond,
23of55interiorbins950..1050events. Manualmotionnotinstrumented;hosttimingisnot
physicalpolling/latencyorinterference/rangecertification. Final58610ACKs,
39360HIDreports,8retries,1471busy. Raw/HIDtotalsdiffer4;donotcallthatRFpacketloss.
Privatebasev13-motion-20261117-004932-493d1a24 summarywithuserreportignored.
exec1572completed;no live serial/raw/UAC/helper/build. Installedv13DISARMEDneutral,
onearmused;normalrebootbeforefuturearm. GloriousinstructedreconnectUSB,cablenot
independentlyverified. No firmware/settings/DPI changes. Deadline02:00AMManila
November17 (UTC2026-11-16T18:00Z). Branchfeature/receiver-foreground-timing.
Completedbaseline:all5buttons/wheelvalidated,two15minrecoverytrialsandforeground
motion/intendedplacementpassed. Preservev13image/guards;no needrepeatthosechecks.
NEXT userpreferencepopup: prioritize convenient REPEATED EXPLICIT Start/Stop,
keepingfiniteboundsandpassiveUSBreconnect (wouldrequirecarefullytestedrearmstate
changefromonearmperboot),or device-sidetiminginstrumentationforperformancework.
No newrearmbehavior/firmwareimplemented or armed. Untilpreferencearrives,do not
silentlyremoveonearmguardoraddautoarm/unboundedmode. Hardwareinterventionwill
stillneedpopupforanyfutureflash/inputtrial. CurrentstatesupersedesLIVEhistory.

V13 DESK-POSITION CAPTURE LIVE - 2026-11-17 00:49 Manila:
Usernormaldeskreceiverposition/freshnormalreboot/GloriousOFF USBunpluggedconfirmed.
Distance/obstaclesnotprovided. Onep/newactualarmconfirmedbeforeONcue. LIVEexec1572;
privatebasev13-motion-20261117-004932-493d1a24. Existing100sforeground/autox60slink.
Useractivateswindowwithotherpointer,motion45-60snormalspot/grip. No firmware/settings
change. Do not reopenserial/rearm/reset. Stopfileexactbase.stop-requestifabort.
NEXT poll1572,collectstalls/overshoot/cleanstoppopupanddistanceifknown,save/push.
Deadline02:00AMManilaNovember17. Earlierdisarmedstatehistorysupersededwhilelive.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md, docs/plans/2026-11-15-wireless-hid-prototype.md and
 docs/plans/2026-11-16-fifteen-minute-session.md. Inspect git status/log first.
Current branch feature/receiver-foreground-timing; use feature-based names.
Keep context and this copy-paste prompt current;push tested milestones.
Private captures/addresses/images/binaries stay ignored in build-pairing-analysis.
Leave unrelated .vscode/ and INFO_UF2.TXT untracked.

V13 ALL FIVE BUTTONS VALIDATED - 2026-11-17 00:47 Manila:
User confirms requestedmiddle/frontthumb/rearthumborder,cleanstop,no stuckbuttons.
Full100s raw/serial exited0:25819 receiverrecords,allforeground(code0),read_errors0/
failed0. Middle5down/5up,side2=5/5,side1=5/5;oneextraLEFTdown/up. Temporalorder
plususerconfirmation mapsfrontthumb->Windowsbutton5(side2),rearthumb->button4(side1).
Onep,onelink,onex,neutralstop+disarm,noERROR. Final59898ACKs,25821HIDreports,
23retries,919busy. Raw/HID totals differ2;do not equate reportcounts with physical
packetloss. Privatebasev13-motion-20261117-004417-1a641303 summarywithmappingignored.
Togetherwithpriorleft/right/wheeltests,allfivebuttons+wheelup/downnowdeliberately
validated. No firmware/build/flash/Windows/DPI changes. exec13642finished;no live
serial/raw/UAC/helper/build. Installedv13DISARMEDneutral;normalrebootfornewarm.
UserinstructedreconnectUSB. Branchfeature/receiver-foreground-timing;deadline02:00
AMManilaNovember17. Preservev13UF2396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc.
NEXT intendeddesk-placementRFcheck onunchangedv13:receiverinintendeddailyposition,
sameDPI/surface/settings,otherpointer/harmlesscapturewindow. Normalreceiverreboot,
GloriousOFF USBunplugged;onearmactualconfirmbeforeON. Existing100sforeground
controller/autox60s. Usersteadycircles/strokes45-60snormalgripfromnormaldeskspot,
reportstalls/overshoot/cleanstoptogetherwithapproxreceiverdistance/obstacles.
This checksonepracticalplacement,notcertifiedrange/interference/gaminglatency.
No need repeatcompleted wired/extraction/build/flash or longrunbaseline.
CurrentcompletedstatesupersedesearlierLIVEhistory. Save/pushthenphysicalpopup.

Previously passed:
- V13 two separate15min normal-use trials,each withtwo mouse-only OFF3s/ON
  recoveries;allcontrols smooth,originaldeadlinecleanstops (fb26dad,2b7dabd).
  First92400rawrecords,second75552;both935s/allbackground/errors0. Not one30minrun.
- V13 foregroundmotion39210records/allcode0/errors0,usersmoothlikewired,xstopclean
  (4d9a89f). Medianhostgap1007us,peak988events/fullsecond. Hosttimingonly;
  manualmotion/stationaryintervalsunmarked,notphysical1000Hz/latencycertification.
- V13 basedonexacttestedv11bridge/main/HIDharness;v12startupneutralexperiment
  removedbecausenobenefit. Onlyreceivercap983040/900000ms/bannerchangedfromv11.
  Eightnativechecks/builds/UF2config/stagingpassed;probe/radio/defaultHIDcontrols
  matched. V13flashed/passivestartupverified (fa9936c,5e1ca7e).
- Connected x/timeout heldLEFTneutralreleasepreviouslyuserverified. PassiveUSB
  restart/rearmv11passed. AbruptphysicalunplugheldstateMATCHEDwiredreferenceon
  thisPC/app;wiredPnPrestartnotcompared. Donotrevivestartupneutralguesses.

Safety/workflow: continueuntilphysicalintervention;usequestionpopuptopause.
Beforeanynewarm:currentdeadline,freshnormalreceiverrebootifused,harmlessdoc/
otherpointer,GloriousOFF USBunpluggedconfirmed. OneCDCreader/DTR,drainfreshv13
banner,ONEp,actualarmingresponseBEFOREmouseONcue. Neverautorearm/reopenreader.
research/capture_receiver_session.ps1 requirescurrentFinishBeforeUtc and18min
margin;defaultfull935s, -ForegroundMotion full100svisiblewindow+xafter60slink.
Bothmodeshardwarepassed. Stopfile exactprivatejob.stop-request sendsxonetime;
earlyabortmarkedpartial. Rawlogger1..1200s,mouse-only,exclusivecreate.
Currentdeadline02:00AMManilaNov17=2026-11-16T18:00Z;lastarm01:42becauseconservative
margin. Do not reuseexpireddeadlineonlaterresume. Native/CIM/serial/gitmutations
usuallyneedrequire_escalated here. No currentliveprocesses.
