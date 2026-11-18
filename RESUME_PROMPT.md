Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md, docs/plans/2026-11-17-gaming-qualification.md and
docs/plans/2026-11-18-usb-frame-timing-review.md.

NORMAL V14 WINDOWS S3 RECOVERY PASS / SESSION CHECKPOINT - 2026-11-18 14:54 Manila:
User-operated Windows sleep/resume with ALL buttons released completed. User
reported ALL controls recovered automatically/smoothly, no issues/reconnect/reset/
mousepowercycle. Power-Troubleshooter1 TargetState/EffectiveState4=S3, SleepTime
06:47:25.3535Z/WakeTime06:50:30.6608Z (~185.307s); KernelPower42/107 also present.
Do not interpret event header107 time as actualwake or SleepDuration3189 as total
sleepseconds. Receiver unique normalv14 USB identity OK afterwake withoutserial.
Passive300s raw session4802 finished1776receiverrecords,read_errors0/failed0;
observed movement,left15/right2/middle2 balancedpairs,wheel. Thumbedges0: all5
postresume success is USER-REPORTED, not independently captured. Background counts
are not polling/latency qualification. Private v14-sleep-20261118-144655.summary.json.
Installed tested normalv14 93eb987a automatic untilunplug; user can keepusing it.
NO live serial/capture/build/helper. No diagnostic timing image remains installed.
Milestones: v17acceptedcontent/count9585 exactmatch and five-minute inactivity
recovery passed earlier today; normalrestoration confirmed. Historical A2two-record
cause unresolved; no fastpollpromotion/end-to-endlatency/sustained1kHz claim.
NEXT on resume: refreshworkwindow (today's15:00Manila cutoff expires afterthisday),
implement isolated USBframehardwaretimestamp candidate per
 docs/plans/2026-11-18-usb-frame-timing-review.md on feature-basedbranch. SOFTWARE
REVIEW ONLY sofar; TIMER2all4CCoccupied; queuedSOF/INcallbacksinclude dispatchdelay.
Preserve exactnormalv14; no repeatedbaselines/builds/flashes withoutclearquestion.
Long4h/8h/multiday soak,repeated sleep/hibernate,mouse deep-sleep,broaderRF checks
remain. Companionapp/DPI/lightingwritesdeferreduntilgamingoptimization. Captures,
addresses,binariesignored; .vscode/ andINFO_UF2.TXT unrelated/untouched.

Current branch: feature/input-conservation-diagnostics.
Exact hardware-tested normal image (installed and user-confirmed):
build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
Product DIY Mouse Dongle v14. Automatic plug-and-play; no DTR/arm/time limit.
No additional restoration is needed. Do not confuse the newly built normal
control dd74b533 with the hardware-tested 93eb987a image.

Today's completed work: v17 software9ecc2bf, hardware/restoration4b99ed2,
restorationconfirmation/timingreview d8fc777, inactivity7710247; final S3 result
commit follows. Private v17-latency-20261118-142055-af9a2982, idle
v14-idle-20261118-143124 and sleep v14-sleep-20261118-144655 summaries saved.
V17 device9585=host9585; aggregate X/Y/wheel and each5buttons3down/3up match.
Aggregate equality does not prove individual-event preservation/order or explain
historical A2device27594/host27592. V15/v16 ABBA had faster RF reaction but no
consistent total-latency gain; no fast-poll promotion. True sensor/click-to-screen
latency and sustained fresh1000Hz remain unqualified.

Keep PROJECT_CONTEXT.md and this copy-paste handoff current after each meaningful
step. Commit/push tested milestones on feature branches. Pause with question
popups for physical intervention; keep mouse wired/OFF before diagnostic flash,
separate readiness before input trials, and restore preserved normal afterward.
Keep captures/addresses/binaries ignored; leave unrelated files untouched.
