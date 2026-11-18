# Accepted input accounting diagnostic (v17)

Purpose: investigate the A2 device/host report-count difference (27594/27592)
without assuming RF loss or Windows suppression. Preserve tested normalv14.
This candidate adds measurement only; no fast-poll promotion or gaming app.

Scope: new App input uses NORMAL_DONGLE+LATENCY_DIAGNOSTICS+INPUT_CONSERVATION,
50us polling,product DIY Mouse Input v17. Existing normal and frozen v15/v16
images remain separate. New input_totals.h is main-thread-only: reset at session
preparation,record AFTER successful unique USB submission,ignore busy/failure/RF
retry,record successful forced-neutral release separately. Signed64 dx/dy/wheel
sums,five down/up counters,unchanged-content reports and finalbuttons. OneMsample
cap flags an incomplete diagnostic;no effect on RF session or normal operation.

Stopped-only d keeps original LATENCY_V1 frame and appends INPUT_V1/SUM/EDGES/END.
Signed64 sums are fixed16digit two's-complement hex. Same shared DTR and1.5sFIFO
output deadline;no output while active,no allocation/perpacketlogging. Host
controller accepts exactv17 product and requires complete latency AND input
footers before treating dump complete;v15/v16 compatibility retained.

analyze_input_totals.py validates timing frame,matching device report coverage,
uncapped totals,finalneutralstate andfivebutton balance. It includes ALL receiver
Raw Input records (foreground/background) but excludes otherdevices. Raw wheel
signed16 units are compared todevicewheelsteps*120. Relative-only input andfields
are validated. Report counts include acceptedRFinput plus forcedneutral separately.
Motion/wheel/edge differences are explicit;aggregate mismatch yieldsCLIexit2.
Matching aggregate totals do NOTprove event order,timing,individual preservation,
or specific suppression/coalescing. Opposite lost motions could cancel. Preserve
that limitation even if aggregate match succeeds. Historical A2 cause stays
unresolved unless evidence independently establishes it.

Tests: inputreportcount assertion failed before implementation;native tests cover
signedextremes,held/chordedges,neutralrelease,64bit totals,cap/reset. Fullbridge
checks run with/withoutdiagnostics/conservation,includingfailed/shortwrite/retry
andneutral exclusion. Formatter checks signedhex andcomplete appendedframe.
Analyzerfixtures cover omittedunchangedrecord,background inclusion,forcedneutral,
negativewheel,ignoredwireddevice,motion/edgemismatch,bad/partial/cappedinput.
ControllerSelfTest covers partial-footerrefusal andlegacycompleteness.

Build input andnormalcontrol;validateflags,stagedsources,UF2range/hash,preserve
image andpushsoftwarecheckpoint. Then physicalflashreadiness:Gloriouswired/OFF,
uniqueNICENANO/S140+exactimageverify,USBidentitycheck. Imageautoactivates.
Separateinputreadiness:GloriousOFF USBunplugged,otherpointer,sameDPI/surface,
harmless blank capture window. OneCDCreader,100sforegroundraw,noarmcommand;
userONaftercue,activatewindowwithOTHERpointer,prioritizeeachbutton3press/releases,
wheelUP5/DOWN3 (nonzero net),thencontrolledmotionuntilx. Releasebuttons normally;
forcedneutralcountermechanics are native-tested and can be separately checked
later ifneeded. Active d50s silent;x70s;frozendump;analyzeallreceiverrows.
Collectuserfeel/stop separately. Restore exactnormalv14aftertrial viafreshphysical
readiness before today's15:00Maniladeadline (UTC07:00Nov18). Controller8minmargin;
do notstarttrialat/after14:52;prefernoflashafter14:45ifdelayed.

No firmware setting writes,Windows inputsetting changes,driverreplacement or
inputinjection. Privatecaptures/addresses/binaries ignored;context/resumeupdated.
USBframephase instrumentation remains later work to avoid mixing observers.
Ordinary-use user report Nov18:usednormalv14,noissuesnoticed,durationunknown;
not a4h/8h soak qualification.

Reference: Microsoft RAWMOUSE documents signedrelative motion,button transition
flags,120-unitwheel delta and possible coalescing:
https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse


### November 18: v17 hardware accounting check passed

The 100-second foreground trial completed with no Raw Input read errors. Device
and Windows both recorded 9,585 input reports. Aggregate X/Y movement, wheel
(net two steps), and all five buttons matched exactly: each button had three
presses and three releases. The user confirmed smooth operation and a clean
explicit stop. No diagnostic cap, USB sample failure or tracking fault occurred.
The active dump probe stayed silent; both summaries arrived only after stopping.

This validates the new accounting path in one trial. Equal aggregate totals do
not prove event ordering or individual-event preservation, and the earlier A2
two-record difference remains unexplained. The instrumented RF-to-USB callback
mean was 756.490 microseconds, maximum 1,155 microseconds; this excludes sensor,
prior RF retries, Windows/game/display latency and is not a controlled performance
comparison. No faster-polling promotion is justified by this run.

Diagnostic UF2 SHA256:
`aea67abc7c748d90c3fde1cc2931343d3e81fcf5b2492c741de82cebfb81b01d`.
Private captures and image remain ignored. Restore tested normal v14 after the
trial; consult PROJECT_CONTEXT.md for the latest installed-board state.
