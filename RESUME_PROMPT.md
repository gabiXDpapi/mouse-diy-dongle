Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-quiet-active-window.md.
Inspect git status/log first. Branch:feature/receiver-quiet-active-window.

V8 HELD-LEFT CAPTURE COMPLETE / BREAK PAUSE 2026-11-16: final trial before
requested01:10 AM Manila break. No further trial planned;resume only when user
returns. Private base build-pairing-analysis/receiver-v8-held-20261116-005751.
100s hidden raw exit0,2016 receiver-only records,0 read errors. Button events:
left-down21.635s/up21.690s,then held-down22.116s and up80.928s. Host serial
chunks show link start20.954s and neutral-complete stop80.947s (about60s).
One arm/discovery,59958 ACKs(54322 idle,0 status,5636 type1),5613 HID submissions,
23 retries,busy2,no ERROR. Forced release observed at stop;user confirms trial
performed,but physical-held/selection-fixed observation is PENDING in popup.
Do not mark physical held-left PASS until that response. User asked release
LEFT/reconnect USB. Board v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used;
COM12 closed,no live serial/raw/build. No new firmware changes.
NEXT on resume: collect pending held-left observation,then record final bounded
v8 validation. If PASS,next scope is sustained-session design/testing,not an
unbounded/daily-use claim. No automatic reset/rearm/flash or limit increase.
Keep successful v8 motion(wired-like,no overshoot),controls and mouse-only
OFF/ON recovery milestones;all already user-confirmed. Saved v8 image unchanged.

V8 CONTROLS/RECOVERY HARDWARE PASS 2026-11-16: user confirms movement,
left/right clicks and wheel UP/DOWN worked before/after ONLY mouse OFF/ON,
receiver untouched;clean stop,no stuck buttons. Private base
build-pairing-analysis/receiver-v8-controls-20261116-005207.100s hidden raw
exit0,4982 receiver-only background records,0 read errors,4865 motion.
Left down/up32/32,right4/4,wheel63(41 negative120,22 positive120). Both wheel
directions captured in later segment;before-recovery wheel supported by user
report,not independently present in raw events. No cadence conclusions from
background capture. Serial one arm,two discoveries/links;second discovery had
30371ms remaining in original session. Final53698 ACKs(21225 idle,2 status,
32471 type1),32437 HID submissions,34 retries,busy396,no ERROR. Both link stops
neutral complete. V8 motion previously user-confirmed wired-like/no overshoot.
Board v8 EXPIRED/PASSIVE,one arm/two discoveries,53698 ACKs used. COM12 closed,
no live raw/serial/build. User told to reconnect mouse USB. No new firmware changes.
NEXT PHYSICAL PAUSE: v8 held-left timeout regression on harmless selectable
plain text and other pointer. Normal receiver reboot(no double-reset),Glorious
OFF/unplugged. One confirmed arm;hold LEFT over text beyond60s window,while
still held use other pointer without clicks to confirm selection fixed,then
release. Hidden raw/serial capture can verify release event at stop. Do not
increase limits/build/flash/rearm on current used boot. This checks forced
release on unchanged v8 before considering sustained sessions.

V8 FIRST MOTION HARDWARE PASS 2026-11-16: user reports "Overshoot gone /
like wired;stopped cleanly" with capture window active. One successful trial;
not calibrated physical latency/DPI or final gaming-performance validation.
Private base build-pairing-analysis/receiver-v8-foreground-20261116-004654:
100s raw exit0,50748 receiver-only relative foreground records,50746 motion,
2 button records,0 read errors,no background records. Median arrival1001us,
p90 1087us,p99 2926us;whole-run max includes pauses. Max axis6/5 vs v7's31/14,
hand movements not mechanically matched. Serial one arm/discovery,59468 ACKs
(8706 idle,1 status,50761 type1),50757 HID submissions,4 retries,busy895,
no ERROR,neutral complete. Active-link stats/busy TEXT counts both0 as intended.
Busy total retained;895 vs v7's171 messages is not a like-for-like loss metric.
Board v8 NOW EXPIRED/PASSIVE,one arm/discovery used,59468 ACKs consumed.
COM12 closed,no live raw/serial/build. User instructed reconnect Glorious wired.
V8 image SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
no further build/flash/tuning needed before regression check. V7 preserved.
NEXT PHYSICAL PAUSE: v8 controls/recovery regression. User normal receiver
unplug/replug(no double-reset),harmless scrollable document/other pointer,
Glorious OFF/USB unplugged,same DPI/settings. Capture100s foreground+95s serial,
one p/DTR with actual arm confirmed before mouse ON. User tries movement,
left/right-click,wheel up/down,then MOUSE OFF3s/ON after about15s,tries controls
again,and observes clean60s stop. Keep receiver connected. New boot/readiness
must be explicitly confirmed before clearing used capture checkpoint;do not
reuse current arm or flash-validation false state automatically. No flash.
Analyze only foreground input_code0;activation with other pointer. Preserve
successful wired/v7/v8 captures;do not repeat baseline/extraction/build.

V8 differs from v7 only in diagnostic overhead:periodic stats suppressed while
linked,per-busy USB text replaced by cumulative counter. Idle ACK sequence hold,
RF/USB mapping/timing/guards/limits unchanged. Native regression failed on v7,
then8 checks passed;receiver/probe/radio builds+UF2 checks passed,probe/radio
hashes unchanged. Default HID image unchanged,not rebuilt. Verified flash/passive
startup completed before first arm. Current image exists at
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and ignored
build-pairing-analysis/receiver-v8-5df75330.uf2. V7 rollback preserved.
Limits:65536 ACKs/60s,8 discoveries,1s gap;initial arm30s,once per boot.
Use one serial reader,drain2s before p,keep open95s postarm,save raw serial and
host UTC/elapsed/phase JSONL. Foreground raw logger100s includes UTC start;
no input injection/keyboard collection. CLI: capture_raw_mouse.exe seconds new.csv
--foreground. Click capture window and keep active;CSV input_code0 foreground,
1 background. Background raw rate is Windows-throttling/coalescing confounded.
No actual physical polling/latency/DPI claims. Saved CORE wired Profile1=1600;
wireless equivalence unverified. Windows accel1/speed9 unchanged;X/Y mapping1:1.

Continue until physical intervention;use question pop-ups to pause. Keep this
resume prompt and PROJECT_CONTEXT.md current after each meaningful step. Push
tested milestones on feature-based branches. Keep captures/addresses/binaries
ignored under build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
