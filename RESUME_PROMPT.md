Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md newest LATEST HANDOFF / NEXT, README.md,
research/CORE_COMPATIBILITY.md, research/RADIO_ANALYSIS.md,
docs/plans/2026-11-20-dpi-values.md and CORE startup coverage plan.
Continue until I say stop. Keep context/prompt current; push tested milestones on
feature/glorious-core-compatibility. Popup/pause for physical intervention.


NOVEMBER 20 — V25 DPI-VALUE CANDIDATE VALIDATED; NOT FLASHED.
Normal exact tested v14 remains installed/user-confirmed working. Glorious last
wired with wireless switch OFF; CORE closed. Wired 258A:2018 healthy, all40 wired
backup reads passed and match wireless backup exactly. No pending recovery.
No live build/serial/capture/helper/timer. User continue until stop.

Prepared v25: same Feature/RF transport, no pointer output, prototype identity.
Adds only profile1 four-stage DPI-value setter group1/op1,18-byte payload,
big-endian X/Y50..19000 in50-DPI steps, zero padding. Exact returned setter
payload must match. Existing selected-stage writer remains restricted1..4.
Native failing test reproduced old rejection before implementation; all13 native
tests PASS after change. Shared packet parser and host helper self-tests PASS,
including ambiguous change/readback/restore/selection failures. No settings writes
occurred in preparation; hardware setter/restore not qualified yet.

research/test_receiver_config.ps1 -ExerciseDpiValues requires exact v25 USB
product. Saves schema2 original values/selection with NeedsWiredRestore=true
BEFORE mutation. Changes one INACTIVE stage by50 DPI, reads all values, then
finally restores original list, reads it back, restores selection and verifies.
Only full readback clears pending flag. On failure pause for fresh wired recovery:
32-bit test_receiver_config.ps1 -RestoreWired -BackupFile saved.backup.json;
checks saved values, mouse version and active profile1 before value recovery.
Do not blindly retry an ambiguous RF write. CORE stays closed.

Build log config-hid-v25-build.log; staged code/config hashes match current.
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v25,2HID,1ms.
50300 flash /16896 RAM;197 blocks,0x26000..0x32500 exclusive.
Preserved private image build-pairing-analysis/config-hid-v25-f577b7e0.uf2
SHA256 f577b7e095a9297da3d2fb34218fe5ba4646d1b36f2c7882041e9461f68b2426.
NEXT fresh popup CORE closed, Glorious wired/OFF, double-reset ONLY receiver to
NICENANO; verify board/exact image before flash. Check USB startup, then separate
stationary wireless mouse readiness that explains temporary inactive-stage edit
and verified restoration. Run32-bit test_receiver_config.ps1 -ExerciseDpiValues
ONLY after readiness. On failure preserve private pending backup and pause for
wired recovery before restoring normal receiver. After batch restore EXACT
normal v14 via fresh bootloader readiness, verify USB and user movement/click.
Normal rollback build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Full CORE identity/keyboard-consumer forwarding/lighting/binding writes remain
unfinished. Four-hour trial deferred. No new baseline/extraction/build needed.

Keep captures, addresses, binaries and proprietary source private/ignored.
Leave unrelated .vscode/ and INFO_UF2.TXT untouched.
