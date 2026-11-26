Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

This message explicitly resumes the saved pause. I may be using a different
OpenAI account; use the local project handoff, not assumed chat memory.
Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and the CORE startup
and DPI-value plans in docs/plans/. Inspect git status before edits.

Current state:
- Branch `main`.
- V30 (`-App core-id`, `258A:2031`, `Series One PRO Wireless`, frozen
  `build-pairing-analysis/core-id-v30-ee8a268b.uf2`, SHA256
  `ee8a268b26547d2933138b2ba295c5a9b1863ac3d9f1195cc25a361ee6a67267`) is BUILT,
  natively tested (15/15 PASS), and UF2-validated: adds 4-slot FIFO config queue
  for OpenMouse back-to-back `setDpi` writes + `01/07` LOD + `0..32 ms` debounce
  + flexible read-only getters (updater/bootloader/address/pairing/reset remain blocked).
- Live Raw Input polling on v28 (`258A:2031`) confirmed true 1,000 Hz (median 1.010 ms,
  912 Hz peak, 9,627 foreground events, 0 errors).
- Preserved rollbacks: exact tested normal v14 (`dongle-v14-93eb987a.uf2`), v28
  (`core-id-v28-e24377f1.uf2`), and 40-record wired & wireless backups.

NEXT: flash v30 via fresh NICENANO readiness, verify 7 HID collections on `258A:2031`,
and verify OpenMouse DPI/polling/LOD/debounce editing wirelessly. Do not repeat
completed extraction, builds, flashes, or baseline tests.

Working preferences:
- Continue autonomously until physical intervention or an explicit pause/stop.
- Use a question popup and PAUSE when I need to act, so I notice it.
- Update PROJECT_CONTEXT.md and this prompt at meaningful steps for future chats.
- Commit/push tested milestones; use feature-based branch names.
- Keep private RF addresses/captures/binaries/proprietary source ignored.
- Leave unrelated .vscode/ and INFO_UF2.TXT untouched.

Exact rollback: build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Private DPI trial config-trial-20261120-211429-90d63770.json and backup both have
NeedsWiredRestore=false; current-config-hid-backup.txt points to the saved backup.
Current-core-profile-backup.txt points to complete40-record wired backup.
Keep the WHOLE local workspace, including ignored build-pairing-analysis and.git:
cloning GitHub alone does not restore private evidence/parameters/UF2 images.
External Zephyr toolchain: C:\Users\Gabriel\zephyrproject. On a new computer or
Windows user, verify dependencies/private paths before hardware work.

For the user: sign in with the later account, open this same local project folder,
start a new chat and paste: "Read RESUME_PROMPT.md and resume this project from
its saved handoff. Pause with a question popup whenever you need me to act."
