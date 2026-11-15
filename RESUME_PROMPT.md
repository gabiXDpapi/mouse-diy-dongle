FOREGROUND WIRED CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/raw-wired-foreground-20261116-002746.45s exit0,20389
wired-only relative records/errors0.20109 foreground(input_code0;20106 motion,
3 button records),280 background. Foreground median arrival1079us,p90 2947us,
p99 4950us;whole-run max includes movement pauses. Background median8007us.
User confirmed capture window stayed active;subjective smoothness answer pending.
No physical polling/latency/DPI claim. This verifies background cadence confound.
No live logger/serial/build. Firmware remains v7. NEXT pop-up already pending:
user wired observation plus normal receiver reboot/Glorious OFF-unplugged
readiness for matching foreground wireless pair. Do not assume readiness;do not
repeat wired capture. Pair script must pass --foreground and show its window;
user activates with other pointer after confirmed arm then moves Glorious.
One p/DTR,95s serial+100s raw;analyze input_code0 only. Do not reuse old boot.

Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-foreground-raw-input.md.
Inspect git status/log first. Branch: feature/foreground-raw-input.

FOREGROUND RAW INPUT SOFTWARE CHECKPOINT 2026-11-16: branch
feature/foreground-raw-input. User clarified wireless symptom: occasional random
overshoot during circles;stop was clean. Background paired capture completed as
recorded below. Do not infer polling or dropped motion from background cadence.
research/capture_raw_mouse.c now accepts optional final --foreground,shows an
ordinary capture window without forcing focus,and appends input_code to CSV:
0 foreground /1 background from WM_INPUT wParam. Default hidden mode preserved.
Known-device filtering,mouse-only registration,exclusive-create output,1..180s
limit and cleanup unchanged. No keyboard/injection/firmware/settings changes.
Serializer regression failed on old code;new self-check passes both delivery
codes plus prior bounds/filter/signed tests. GNU11 warnings-as-errors build PASS.
Hidden1s lifecycle PASS;visible1s lifecycle verified exit0/records0/errors0 and
DONE marker. Invalid option rejected before creating file. Explicit stdout flush
added after DONE because initial visible smoke omitted buffered completion text.
Final ignored exe SHA256
1e6ed06a5b447461d77d2fa20add18269634bcc8475ea6aeea8a59130c832d16.
Smoke is not movement/performance evidence. No controlled foreground capture yet.
No live capture/serial/build sessions. Board v7 expired/passive after one arm,
one discovery,58479 ACKs;COM12 closed. User told to keep mouse wired.
NEXT PHYSICAL PAUSE: wired readiness,same surface/saved1600 profile/other pointer.
Start45s logger --foreground;after readiness cue user clicks capture window and
keeps it active while doing20-30s circles/straight strokes. Analyze input_code0
only and record activation fraction. This is a new foreground measurement,not
a repeat of background baseline. Later wireless needs reboot/OFF-unplugged
readiness and one actual-confirmed arm. Do not rearm on current used boot.

Firmware remains tested v7 idle-ACK sequence hold. UF2 SHA256:
b6a63b99667a764ed32efb2a3f46bf0721c99f43ecbd9131f81ea08370fde16a.
Bounds:65536 ACKs/60s,8 discoveries,1s gap;initial discovery30s,one p per boot.
No rearm without normal receiver reboot. No live serial/build/raw logger.
Last paired base:build-pairing-analysis/receiver-v7-raw-20261116-001759.
Raw7198 receiver-only relative records/errors0;serial52983 HID submissions,
117 retries,one discovery,58479 ACKs,neutral complete. Background counts do not
prove loss or polling. Wired background baseline3674 records,subjectively smooth.
CORE saved wired Profile1 selected1600 DPI;wireless equivalence unverified.
Windows acceleration1/speed9 unchanged;receiver signed X/Y mapping1:1.
No fixed DPI adjustment or confirmed overshoot cause. Quiet-active firmware
candidate deferred;private patch preserved,no new firmware build/flash.

Keep PROJECT_CONTEXT.md and this resume prompt updated after each meaningful
step. Push tested milestones on feature-based branches. Continue until physical
intervention;use question pop-ups to pause. Confirm actual arming before mouse
power-on;never silently assume readiness. Keep captures/RF addresses/binaries
ignored in build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
Do not repeat completed extraction,builds,flashes or background baselines.
