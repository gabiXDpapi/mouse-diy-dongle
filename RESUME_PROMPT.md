Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-timed-host-motion.md.

WIRED SMALL-CIRCLE REFERENCE USER-CONFIRMED / LARGER-MOTION CHECK NEXT - 2026-11-18 19:06 Manila:
UserwiredcontinuousSMOOTH, mostlySMALL/CONTROLLEDcircles. Privatewired-timed-motion-
20261118-190223-853da817.summary.json updated. Fixed20s12469wiredmoveevents/
623.45Hz,328..879/s,nozeroseconds,median1024us,p95=3031,p99=5063. All18103raw
rowswiredonly, noerrors/focusloss. Firstwireless553.9Hz/3initialzeroseconds with
usercontinuoussmoothreport stillunexplained. Lowmeanmovementeventsalsoonwired;
notRFfaultproof. NEXT scopedlargersteadycircle WIREDtrial (alreadywired): target
activateswindowitself,keepmouseinhand, comfortable~3–5cmcirclesifroom/sensoronpad,
no pauses/lifts/clicks/wheel,DPIunchanged. Same5/30/5window/fixed20sanalyzer. Then
matchmotionwirelessly tocheckwhetheroriginalgap/ratepersists. No firmwareflash/
settingschange/newcode/build. Normalv14receiverunchanged, no livecapture/serial.
Useruntilsaystop; physicalreadinesspopupbeforecapture. Save/pushwiredreference.

Currentbranchfeature/timed-host-polling-measurement. Software8de3dac/wiredsupport
24c8bc5 pushed; wiredreferencecheckpointfollows. Useruntilsaystop,nofixeddeadline.
Installednormalv14exact93eb autoactive,GloriouscurrentlyWIRED; no activeprocesses.
Normalimagebuild-pairing-analysis/dongle-v14-93eb987a.uf2
SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
No restorationneeded; no newflashes/builds forcurrentmotioncheck.

Controllerresearch/capture_timed_motion.ps1 -Source wired (orreceiver default).
Noarm/serial/x/injection. Visible180shardlimit waitsfocus->5scountdown/30sMOVE/
5sSTOPautoclose, normalinputcontinues. AnalyzeclosedcompleteCSV central20s only;
18analysischecks/controllergates/syntaxPASS. Samevalidatedtimedbinary06f74472;
source/headerhashesprivatevalidationJSON; originalrawbinary09d2unchanged.
Firstwirelessprivatev14-timed-motion-20261118-184803-b718bd7e; wiredsmallreference
wired-timed-motion-20261118-190223-853da817. Preserveuserevidenceandfixedepochs.

LiveUSBcheck7e23c76 fullspeedHID2/IN0x83/max16/bInterval1/activepipematch confirms
configurationtarget1k, notactualfreshreport/sensor/buscadence. PhaseABBA0a7c14e
showedrepeatable~18usRFstagegain buttotalgamingbenefitunproven; normalkept50usCPU
wait(separatefromUSB1ms). No moreABBArepeatsneeded. No companionapp/DPIwrites.
Keepcontext/resumecurrenteachstep;pushmilestones;pausewithquestionpopupsforphysical
intervention. Privatecaptures/addresses/binariesignored; unrelatedfilesalone.
Longsoak/repeatedsuspend/hibernate/physicalend-to-endlatency/sustainedfresh1kstill
unqualified. OneS3/5mininactivityrecoverypassed. No user-settingchanges.
