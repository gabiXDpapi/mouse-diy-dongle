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


NOVEMBER 20 — V25 FLASHED; PASSIVE USB STARTUP PASS; NO DPI EDIT YET.
Fresh user NICENANO readiness, CORE closed/Glorious wired-OFF. Board and exact
f577b7e0 image verified; Windows product DIY Mouse Config HID v25 healthy, mouse
and vendor Feature collections present. Private core-v25-startup.json. No serial
opened, no query or settings write during flash/startup. No live build/helper.
NEXT separate physical readiness: CORE closed, Glorious ON/USB unplugged/STILL
near unchanged receiver, other pointer available. Explain test temporarily edits
one inactive profile1 stage by50 DPI, saves originals first, then restores full
stage list and original selected800-DPI stage with readback. Pointer disabled.
After Ready run32-bit research/test_receiver_config.ps1 -ExerciseDpiValues once.
Check result and backup NeedsWiredRestore flag. If pending, stop and popup for
wired recovery (mouse wired with switchOFF, receiver unchanged), then run32-bit
helper -RestoreWired with exact pending backup. Do not flash normal before pending
mouse settings recovery is resolved. After successful edit/restore, fresh NICENANO
popup to restore exact normalv14/93eb987a, then USB and user movement/click check.
Normalv14 is currently NOT installed. All40 configuration records previously
matched wireless and wired backups; CORE full support still unfinished.


NOVEMBER20 — V25 REAL DPI-VALUE EDIT AND RESTORE HARDWARE PASS.
Fresh user temporary-write readiness; 32-bit test_receiver_config.ps1
-ExerciseDpiValues completed successfully. Profile1 inactive stage4 changed
3200->3250 X=Y, complete four-stage list read back, then restored400/800/1600/3200
X=Y with full readback and original selectedstage2=800 restored/read back.
Private config-trial-20261120-211429-90d63770.json and its .backup.json:
Success/Changed/Restored true; NeedsWiredRestore false IN BOTH. Schema2 backup
pointer current-config-hid-backup.txt saved before mutation. Exact write echo
payloads checked; no pending mouse recovery. Independent wired recovery code
self-tested and data connection verified, but this trial did not need it.
V25 remains installed, pointer disabled; mouse last ON/unplugged/STILL. COREclosed.
No live serial/capture/build/helper/timer. NEXT fresh physical popup: Glorious
wired or OFF, double-reset ONLY receiver to NICENANO. Restore exact normalv14
build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
verify USB then user wireless movement/click check. Full CORE app/bindings/lighting
still unfinished. This proves configuration value transport/readback, not physical
sensor DPI calibration. User continue until stop; four-hour trial deferred.
