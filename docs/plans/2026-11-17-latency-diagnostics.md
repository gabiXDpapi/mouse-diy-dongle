# Quiet receiver timing diagnostic (v15)

Separate opt-in App latency reuses automatic normal startup;working normalv14
remains installed until physical flash readiness. No latency optimization yet.
Existing TIMER2/PPI captures RADIO END/READY;free CC3 captures CPU timestamps in
short IRQ-locked sections. Instrumentation introduces observer overhead.

Six fixed-memory histograms: candidate_age (accepted ACK-candidate RF END to
bridge entry),rf_submit (unique RF END to USB API submission START),submit_usb
(submission start to HID IN-completion callback),rf_usb (RF END to that callback),
ack_ready andack_end (RF END to hardware TX READY/END for successful data ACK).
USB completion is a driver callback,not an external bus analyzer timestamp.
These exclude sensor/debounce,earlier mouse queue/retry waits,OS/game anddisplay.

Nine bins with inclusive upper bounds50,100,250,500,1000,2000,4000,8000us,then
overflow. Each histogram caps at1Msamples;cap/ownership/failure flags invalidate
qualification. Counts/max32bit,sum64bit. Unsigned timestamp differences cover
clock wrap for these short transfers. One pending USB sample;early/synchronous
callback is retained until submission success is known. Neutral reports excluded.
Afterx,collectionstops,pendingcompletiondrains,anddmayemit frozen summaries.

No printk/UART console/boot banner in timing mode. d whileactive emits nothing.
PoststopFIFOoutputchecksDTRand1.5swhole-frame deadline. No perpackettext/heap.
NormaldonglecontrolhasLATENCY_DIAGNOSTICS OFF. Actual RF/USB behavior and quiet
startup remain shared;timingbuildonlyaddsmeasurementhooks andpoststopoutput.

Validation: collector assertion failed before implementation;9nativechecks pass,
including bridge with/without diagnostics. Core covers quantization/cap,clockwrap,
async/early/duplicate callbacks,failed/shortwrites,stopdrain and ownershipfault.
Writer covers active/pending refusal,partial writes,DTRloss anddeadline.2parser
checks plus corruption cases enforce histogram/count/time-component integrity.

Build: firmware/build.ps1 -App latency. Final image d751601753a9be286dc806ecee902f0a3d4a1170b9891cc56680f37519dd06b7,
49304flash/16448RAM,193blocks,0x26000..0x32100exclusive. Preservedprivate timing-v15-d7516017.uf2.
Normalcontrolbuildpassed(flagsOFF);testednormalrollback93eb987a ispreserved.

Physical workflow:
1. Glorious wired/OFF;uniqueNICENANO identity/hash/UF2 checks,flashdiagnostic.
2. VerifyUSBproduct DIY Mouse Timing v15 withoutCDCcommands. Itautoactivates.
3. Separate readiness:harmlessdocument/otherpointer,sameDPI/surface,mouseOFF/USB
   unplugged. capture_latency_session.ps1 -FinishBeforeUtc2026-11-16T19:00:00Z.
   One CDC reader+100sforeground Raw Input;noarmcommand. User ON aftercue and
   activatescapturewindow,steadycircles/strokes~45s. d at50spoststartmustsilent;
   x at70s,thenup to3poststopdrequests. Completeframeimpliesneutralstop.
4. Analyze serial via research/analyze_latency.py. Requireall6histograms,count/
   sum/component reconciliation,failed/capped/trackingflags0. Mean/maxexactfor
   collectedsamples;quantilesarebucketUPPERbounds. Keep rawbackground/foreground
   separate;manualmotionnotgroundtruth. Collectuserfeel/cleanstoppopup.
5. Restorepreservednormalv14viafreshphysicalNICENANOreadinessbefore03:00break,
   thenverifyproductandordinaryuse. Do notleavedisarmeddiagnosticbyaccident.

Controllerdeadlinegate8minbeforefinish;one x,quietactive probe,earlyraw-close/
summaryerrorscaptured,noautorearm. Selftest/syntaxpassed;liverunpending.
Any aborted trial remains aborted even if a valid partial-epoch summary exists.
Companionapp/DPI/lighting work deferred until after gaming optimization.
