Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md and
 docs/plans/2026-11-20-core-startup-coverage.md.
Keep context and this prompt current; commit/push milestones on feature branches.
Continue until I say stop; popup and pause for physical intervention.


NOVEMBER 20 — V24 WIRELESS PROFILE-BACKUP CANDIDATE READY; NOT FLASHED.
Normal exact tested v14 remains installed and working. User reports active mouse
use; Windows still sees no Glorious wired USB device after reconnection. No wired
stream or query was opened. Keep CORE closed. Continue until user says stop.

V24 expands canonical GET coverage for the 40-query profile backup. The existing
profile-1 selected-stage setter remains restricted, but this backup operation
sends GETTERS ONLY and changes no settings. No new write trials while wired
recovery is unavailable. No pointer output in this diagnostic.
Build finished successfully: all 13 tests PASS, staged source hashes match,
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v24.
197 UF2 blocks, 0x26000..0x32500 exclusive; 50188 flash / 16896 RAM.
Preserved private image build-pairing-analysis/config-hid-v24-875a85b9.uf2
SHA256 875a85b941cd15930f13b19e1a1eabacacc00c2133dcb2f8abe0db33de4d1a15.
Both backup helper self-test modes passed. No flash, settings write, serial,
capture, helper, build or timer remains running. Hardware getter coverage for
all profiles is still unqualified. Full automated profile restore is not ready.

NEXT: fresh popup/pause for CORE closed, Glorious wired or OFF, double-reset ONLY
receiver into NICENANO. Verify board and exact preserved image before flashing.
Check passive USB startup, then separate readiness for Glorious ON / USB unplugged
and STILL near receiver. Use 32-bit SysWOW64 PowerShell to run
research/backup_mouse_profiles.ps1 -Wireless. Validate all 40 responses; only full
success updates private current-core-profile-backup.txt. On failure preserve
partial evidence, do not retry blindly or change settings. After this batch,
popup/pause to restore EXACT tested normal v14 image dongle-v14-93eb987a.uf2,
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052,
verify USB identity and ask for wireless movement/click check. No pending DPI
recovery: original 800-DPI selection previously verified restored.
CORE identity/full settings/key forwarding remain unfinished; four-hour trial
is deferred. No app launch or stock identity until required coverage qualifies.

Branch: feature/glorious-core-compatibility. Keep private captures, addresses,
binaries and proprietary source ignored. Leave .vscode/ and INFO_UF2.TXT alone.
Do not repeat extraction/builds/baselines. V24 is built and frozen, NOT flashed.
