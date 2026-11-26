Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

This message explicitly resumes the saved pause. I may be using a different
OpenAI account; use the local project handoff, not assumed chat memory.
Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and the CORE startup
and DPI-value plans in docs/plans/. Inspect git status before edits.

Current state:
- Branch feature/glorious-core-compatibility.
- V27 (`DIY Mouse CORE Proto v27`, `core-proto-v27-cc5f4b87.uf2`, local `2FE3:0008`,
  3 HID interfaces / 7 collections) is currently installed and HARDWARE-VERIFIED.
- V28 (`-App core-id`, `258A:2031`, `Series One PRO Wireless`, reusing exact v27
  C source) is BUILT, all 15 native unit tests PASS, and UF2-validated/frozen at
  `build-pairing-analysis/core-id-v28-e24377f1.uf2` (SHA256
  `e24377f1a7f56113da977d65bf668790a52a5828a7920409c771845ce0ade18f`), NOT YET FLASHED.
- Exact tested normal v14 (`93eb987a`) stays preserved as rollback; 40-record wired
  and wireless backups match; no pending settings recovery.

NEXT: popup and pause for CORE closed, Glorious wired or wireless OFF, replug and
double-reset ONLY the DIY receiver into NICENANO. Verify board ID + exact v28 hash,
flash v28, verify `258A:2031` enumerates all 7 HID collections with zero Code 10
errors, then switch mouse ON (unplugged) and launch Glorious CORE for first live
app detection, battery readout, and settings testing. Do not repeat completed
extraction, builds, flashes, or baseline tests. Four-hour trial remains deferred.

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
