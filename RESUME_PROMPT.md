Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-quiet-active-window.md.
Inspect git status/log first. Branch:feature/receiver-quiet-active-window.

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
