Continue my DIY Glorious mouse receiver project. Read PROJECT_CONTEXT.md,
especially LATEST HANDOFF / NEXT, plus README.md, research/RADIO_ANALYSIS.md,
and docs/plans/2026-11-15-wireless-hid-prototype.md.

Wireless Windows pointer control was confirmed on receiver v2. Receiver v3 is
already flashed and passed passive startup on COM12: 1,024 ACK attempts maximum,
10-second active window, 30-second discovery arming. It is unarmed with unused
budgets; no `p` has been sent. Mouse was last confirmed wired. No live serial or
build session remains. Do not repeat firmware extraction, builds, flashing or baseline.

Next: ask me to prepare a harmless scrollable document/page, another pointer,
and the mouse OFF/unplugged, left OFF until arming confirmation. After readiness,
arm once on COM12 with DTR, confirm arming, then tell me to turn the mouse ON and
test movement/clicks/wheel. Capture 45 seconds, analyze results, and ask what I observed.

Keep PROJECT_CONTEXT.md current and push tested milestones on feature-based
branches (current: feature/wireless-hid-prototype; no codex prefix). Continue
autonomously until physical intervention is needed. Keep raw captures, device
addresses and firmware binaries local/ignored. Gaming performance and sustained
operation remain unverified.
