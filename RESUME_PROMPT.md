Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

This message explicitly resumes the saved pause. I may be using a different
OpenAI account; use the local project handoff, not assumed chat memory.
Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and the CORE startup
and DPI-value plans in docs/plans/. Inspect git status before edits.

Current state:
- Branch feature/glorious-core-compatibility.
- Exact tested normal v14 installed and user-confirmed automatic wireless control.
- V25 real DPI-value edit 3200->3250->3200 passed; original 400/800/1600/3200 list
  and selected stage 2=800 restored. No pending recovery or live process/session.
- Wireless and wired 40-record configuration backups match. Full-device restore
  and macro bodies are not qualified. Full CORE app compatibility is unfinished.
- Wired HID layout inspection PASSED (7 collections; private
  wired-hid-layout-20261126-183213-b93b7b4b.json).
- V26 auxiliary USB HID forwarding (`-App aux-hid`, `DIY Mouse Aux HID v26`,
  local `2FE3:0008`, 2 HID instances) is BUILT, all 14 native unit tests PASS,
  and UF2-validated/frozen privately at
  `build-pairing-analysis/aux-hid-v26-e9f3c1cc.uf2`
  (SHA256 `e9f3c1cc1beca9aaed1e26ad42b243ced04ab4c4758eef8e41a62ad646d7b4f7`),
  NOT YET FLASHED.

NEXT: popup and pause for CORE closed, Glorious wired or wireless OFF, double-reset
ONLY the DIY receiver into NICENANO. Verify board ID and exact frozen v26 hash,
flash v26, verify Windows enumeration (Mouse + 5 Auxiliary HID collections healthy,
zero Code 10), then verify automatic wireless pointer/click control on v26. Restore
exact tested normal v14 (`93eb987a`) via fresh bootloader readiness after testing
if needed. Do not repeat completed extraction, builds, flashes, wired layout
inspection, or baseline tests. Four-hour use trial remains deferred for CORE.

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
