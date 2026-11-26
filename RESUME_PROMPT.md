Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

This message explicitly resumes the saved pause. I may be using a different
OpenAI account; use the local project handoff, not assumed chat memory.
Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and the CORE startup
and DPI-value plans in docs/plans/. Inspect git status before edits.

Current state:
- Merged into `main` (from `feature/glorious-core-compatibility`).
- V28 (`-App core-id`, `258A:2031`, `Series One PRO Wireless`, frozen
  `build-pairing-analysis/core-id-v28-e24377f1.uf2`, SHA256
  `e24377f1a7f56113da977d65bf668790a52a5828a7920409c771845ce0ade18f`) is currently
  installed and HARDWARE-VERIFIED:
  - 12/12 PnP devices OK, zero Code 10 errors; all 7 HID collections match wired layout.
  - User confirmed smooth automatic wireless control, live Glorious CORE wireless
    detection of `SERIES ONE PRO WIRELESS` (battery %, firmware version, settings),
    and OpenMouse (WebHID) wireless detection (Connected, 100%, 1,000 Hz).
  - User also reported prior real gaming use felt as expected.
  - Updater/bootloader/address/pairing/reset commands remain strictly blocked.
- Preserved rollbacks: exact tested normal v14 (`dongle-v14-93eb987a.uf2`) and
  prototype v27 (`core-proto-v27-cc5f4b87.uf2`), plus 40-record wired & wireless backups.

NEXT: optional CORE/OpenMouse keybinding remap test (live keyboard/media auxiliary
HID verification) or longer endurance/sleep-resume soak on v28. Do not repeat
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
