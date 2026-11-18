Continue the DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-18-usb-frame-phase-implementation.md.

V18 QUALIFIED / NORMAL RESTORATION CONFIRMED - 2026-11-18 17:52 Manila:
User confirmed restored normal wireless works. Installed normalv14exact93eb987a,
autoactive/no serial/arm/limit; no livecapture/reader/build/helper. Privateafterv18
restoreJSON updated. V18calibrated1/invalid0, all21818foregroundreports+inputtotals
match, all5buttons3down/up, user smooth/cleanstopped. Perphase table savedresearch.
Withinthisrun RFcallbackphasebinsmean1.058ms(n28 early) down0.207ms(late), global
0.566ms/max1.150ms; RFsubmit~36..59us. ObservationalUSBphaseassociation, notgain.
Wholemixedactivityhostgapsmedian1010us/p95=2147us/p99=5797us include deliberate
controlpauses/zeromotion; NOTsustainedpolling orRFloss qualification. Busy309/52184
candidates,retries34; no inferredend-to-endloss. HistoricalA2causeunresolved.
NEXT isolatedfastpollcomparison usingSAMEphaseinstrumentation asv18: addphase-fast
v19 product/config (10us vs50us), nootherC/runtimechange. Build/test/verify/preserve
andpush BEFOREphysicalreadiness. V18A1 preserved; compareB1/B2/A2onlyifappropriate,
phase-aware/repeatedboots/manualmotionlimits; noautomaticpromotion. Restoretested
normalaftereachdiagnostic. Userwindowuntilsaystop, oldcutoffsrevoked. Companionapp
andDPIwritesdeferred. Keepprivatecaptures/addresses/binariesignored.

Current branch feature/usb-frame-phase-diagnostics; software681183d pushed,
hardware/result/restoration milestone follows. No further restoration needed NOW.
Normal image build-pairing-analysis/dongle-v14-93eb987a.uf2
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Qualified phase baseline build-pairing-analysis/phase-v18-05eaf02a.uf2
SHA256 05eaf02aebbbb49163a2a8f9b95c1e5914ae06afd73a4793eba43ab48e2549cd.
Private capture v18-latency-20261118-174506-f45a9f45.summary.json.
Normalcontrolcb8bbb60 is software-only; do NOT substitute it for tested93eb image.

Keep PROJECT_CONTEXT and this copy-paste prompt current after meaningful steps.
Commit/push tested milestones on feature branches. Pause using question popups
for physical interventions; separate flash and input readiness. Capture100s,
noarm/d50silent/x70/frozen3frames; operationalnow+30minfinishguard during user
open-endedwindow. Restoreexactnormalv14 andverifyUSB/useroperationafterdiagnostic.
Privatecaptures/addresses/binariesignored; unrelated.vscode/INFO_UF2.TXT untouched.

Gaming/longreliability remains unfinished. Exactaggregatecontent match does not
prove individualeventpreservation/order or historicalA2two-recordcause. First
WindowsS3 and5mininactivity recoverypassed, but no4h/8h/multiday soak, repeated
suspend/hibernate/deepsleep ortruephysicalsensor-to-screenlatency qualification.
Earlier v15/v16ABBA RFreactionfasterbuttotaladvantagenotconsistent; no normal
fastpollpromotion. Companion app/settingswrites deferreduntilgamingoptimization.
