Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md,docs/plans/2026-11-16-receiver-quiet-active-window.md
and docs/plans/2026-11-16-foreground-raw-input.md. Inspect git status/log first.

V8 QUIET-ACTIVE SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-quiet-active-window. Receiver v8 BUILT/VALIDATED,NOT FLASHED.
Change only active diagnostics:skip periodic stats while linked,replace per-busy
text with cumulative EAGAIN counter in final summary. Keep v7 idle ACK rule,
motion mapping,ACK timing,first timing samples,link/error prints and all limits.
Regression failed on v7;all8 native tests PASS with v8. Receiver/probe/radio
builds and UF2 checks PASS. Probe/radio hashes unchanged;default HID image hash
unchanged,not rebuilt. Receiver staged source/config/v8 banner verified.
V8 UF2: C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd.
61256 flash/16128 RAM;240 UF2 blocks,0x26000..0x34FFF. Private preserved image
build-pairing-analysis/receiver-v8-5df75330.uf2;metadata receiver-v8-build-validation.json.
V7 rollback receiver-v7-b6a63b99.uf2 hash verified. V8 is an unproven A/B logging
overhead experiment,not an established overshoot fix;less active-time telemetry.
Board STILL v7 expired/passive after foreground trial:one arm/discovery,59129
ACKs used. COM12 closed,no live raw/serial/build. Glorious asked to reconnect wired.
NEXT PHYSICAL PAUSE via popup: keep Glorious wired/OFF,double-reset only RECEIVER
into NICENANO. Verify bootloader identity and exact v8 hash/UF2 before copy,then
passive startup only,no arming. Separate later motion readiness for paired
foreground raw100s/serial95s,one p/DTR,actual arming confirmation BEFORE mouse ON.
Raw logger accepts --foreground;user must activate window with other pointer;
analyze input_code0. No repeat v7 baseline/build/extraction. DPI/Windows unchanged.

Latest foreground v7 pair:private base receiver-v7-foreground-20261116-003012.
Raw39180 receiver-only relative records,39076 foreground,0 read errors.
Foreground median1025us,p99 4044us. Serial59129 ACKs,39709 HID submissions,
102 retries,171 busy messages,one discovery,neutral complete. User confirms
random overshoot persists during circles;clean stop. Wired foreground baseline
raw-wired-foreground-20261116-002746:20109 foreground records,median1079us,
user-confirmed smooth. Wireless occasional max axis31/14 vs wired3/2;some big
steps near busy log chunks,not causal proof. Physical strokes not mechanically
matched. Background125-events/s samples are confounded by Windows background
throttling/coalescing;do not claim polling rate,RF loss or DPI from counts.
CORE wired Profile1 selected1600 saved unchanged;wireless equivalence unverified.
Windows acceleration1/speed9 unchanged. Signed X/Y mapping1:1. No fixed gain hack.

V7/8 limits:65536 ACKs/60s,8 discoveries,1s gap;initial discovery30s,one p per boot.
No rearm on used boot. Confirm actual arming before telling mouse power-on.
Continue until physical intervention;use question pop-ups to pause. Keep this
resume prompt and PROJECT_CONTEXT.md current after each meaningful step. Push
tested milestones on feature-based branches. All private captures/RF addresses/
binaries ignored in build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
