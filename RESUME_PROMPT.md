Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-quiet-active-window.md.
Inspect git status/log first. Branch: feature/receiver-quiet-active-window.
The project was paused for a break on November16,2026;this new continuation
request resumes it. Do not automatically arm or repeat completed hardware work.

Current hardware: receiver v8, EXPIRED/PASSIVE after one arm/discovery and59958
ACK attempts. COM12 closed;no live serial/Raw Input/build sessions. Mouse was
instructed reconnected by USB. Normal receiver reboot is required before another
arm;never assume physical readiness. No firmware changes since v8 validation.
V8 UF2 SHA256:
5df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd
Image:C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
Private copy:build-pairing-analysis/receiver-v8-5df75330.uf2. V7 rollback preserved.

Completed/user-confirmed on v8:
- Motion feels like wired;random circle overshoot gone in first foreground trial.
- Movement,left/right clicks,wheel up/down before/after mouse-only OFF3s/ON.
- Recovery stays within original60s session;clean stops,no stuck buttons.
- Held-left timeout release:selection stayed fixed with other pointer while
  Glorious LEFT remained physically held beyond70s;raw left-up matches stop.
All8 native checks,receiver/probe/radio builds/UF2 checks passed. Probe/radio
control hashes unchanged;default HID existing image unchanged,not rebuilt.
V8 only suppresses periodic/per-busy text during active RF processing,retaining
cumulative busy totals and link/error/final summaries. ACK rules,mapping/timing
and safety limits unchanged. Last held trial hadno ERROR,neutral complete,
2016 raw records/no read errors,5613 HID submissions,23 retries,busy2.

NEXT: design the smallest bounded sustained-session experiment using v8 as stable
baseline. Review current guards/tests first;retain explicit stop/neutral handling
and finite limits. Do not silently enable unbounded operation or increase limits.
No further build/flash/test repeat is needed until a justified new change.
Current limits:65536 ACKs/60s,8 discoveries,1s gap;initial discovery30s,one p/boot.
For later live work:question-popup readiness,one reader,DTR,2s prearm drain,
single p,actual arming confirmation BEFORE mouse power-on,adequate capture time.

Host logger:research/capture_raw_mouse.c;ignored capture_raw_mouse.exe accepts
seconds new.csv [--foreground]. Foreground window must be activated by user;
CSV input_code0 foreground,1 background. Background~125-event/s cadence is
Windows throttling/coalescing confounded;not polling/loss evidence. Raw timestamps
are host arrival,not physical latency/DPI. Wired CORE Profile1 selected1600 saved;
wireless DPI equivalence unverified. Windows accel1/speed9 unchanged;X/Y1:1.
Do not claim final1000Hz gaming/physical latency/daily-use reliability.

Continue until physical intervention;use question pop-ups to pause. Keep
PROJECT_CONTEXT.md AND RESUME_PROMPT.md updated after meaningful steps. Push
tested milestones on feature-based branches. Keep captures/RF addresses/binaries
ignored in build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
