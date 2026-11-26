Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

This message explicitly resumes the saved pause. I may be using a different
OpenAI account; use the local project handoff, not assumed chat memory.
Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and the CORE startup
and DPI-value plans in docs/plans/. Inspect git status before edits.

Current state:
- Branch feature/glorious-core-compatibility.
- V26 (`DIY Mouse Aux HID v26`, `aux-hid-v26-e9f3c1cc.uf2`, local `2FE3:0008`)
  is currently installed and hardware-verified for clean Windows enumeration
  (10/10 PnP OK, zero Code 10, all 6 HID collections) and smooth automatic
  wireless control. Exact tested normal v14 (`93eb987a`) stays preserved as rollback.
- V27 combined CORE proto (`-App core-proto`, `DIY Mouse CORE Proto v27`,
  local `2FE3:0008`, 3 HID instances: Mouse + Vendor Config Feature + 5 Auxiliary
  collections, full CORE startup allowlist + battery + local version `00/00/81` +
  offline `0xA4`) is BUILT, all 15 native unit tests PASS, and UF2-validated/frozen
  at `build-pairing-analysis/core-proto-v27-cc5f4b87.uf2` (SHA256
  `cc5f4b8751a450eb858e5b5f5e12a0d2a21c4b5d06a7527ee9dc0e7c4159c765`), NOT YET FLASHED.
- V25 real DPI-value edit 3200->3250->3200 passed; original 400/800/1600/3200 list
  and selected stage 2=800 restored. No pending recovery or live process/session.
- Wireless and wired 40-record configuration backups match. Wired HID layout
  inspection PASSED (7 collections). Full CORE app compatibility still pending
  v27 hardware qualification before enabling stock USB ID `258A:2031`.

NEXT: popup and pause for CORE closed, Glorious wired or wireless OFF, double-reset
ONLY the DIY receiver into NICENANO. Verify board ID and exact frozen v27 hash,
flash v27, verify Windows enumerates all 7 HID collections with zero Code 10 errors,
then run read-only hardware checks (local version `00/00/81`, offline `0xA4`,
wireless battery `00/83` + version `00/81` + live pointer control). Do not repeat
completed extraction, builds, flashes, wired layout inspection, or baseline tests.
Four-hour use trial remains deferred for CORE.

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
