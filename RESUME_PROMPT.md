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
- V25 real DPI-value edit3200->3250->3200 passed; original400/800/1600/3200 list
  and selectedstage2=800 restored. No pending recovery or live process/session.
- Wireless and wired40-record configuration backups match. Full-device restore
  and macro bodies are not qualified. Full CORE app compatibility is unfinished.
- Last wired readiness check found no258A:2018; HID layout inspection NOT started.

NEXT: popup and pause for COREclosed, Glorious wirelessswitchOFF, direct working
USB DATA connection with wired movement verified, receiver unchanged. AfterReady
verify device presence and run32-bit SysWOW64 PowerShell -NoProfile
-ExecutionPolicy Bypass -File research/inspect_wired_hid.ps1. This reads Windows
HID report layouts only: no Feature/settings requests, flash or input recording.
Use actual layouts for later keyboard/media/system forwarding and CORE remapping.
Keep normal v14 unchanged during this step. Do not repeat completed extraction,
builds, flashes or baseline tests. Four-hour use trial remains deferred for CORE.

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
