# DPI value edit and recovery implementation plan

**Goal:** Qualify actual profile-1 DPI value editing over the receiver and verified restoration, before CORE startup writes.

**Architecture:** Reuse the v24 Feature/RF transport and shared host protocol. Permit only profile1, exactly four DPI stages, values50..19000 in50-DPI steps, canonical padding. Save original stages and selection before the temporary mutation; independent wired recovery uses the same exact saved packet. Keep prototype USB identity and pointer output disabled.

**Tech stack:** Existing C/Zephyr firmware, native assert tests, PowerShell/HidSharp, private JSON evidence.

1. Add failing allowlist and response tests to firmware/test_config_read.c for group1/op1, size18, profile1/count4; reject bad dimensions, padding, profile, count and mismatched echoes.
2. Implement minimum validator and full setter echo check in firmware/radio_logger/src/config_read.h. Keep old normal path and other write scope unchanged.
3. Reuse research/mouse_settings_protocol.ps1 for packet generation and transaction parsing. Extend research/test_receiver_config.ps1 minimally for a value trial and matching wired recovery; retain v23 selection trial behavior. Mock changed-readback and failed-restore paths, and mark restored only after full original readback.
4. Candidate product v25: firmware/radio_logger/config-hid.conf. Run existing13 native tests plus host self-tests, build once, verify staged source/config/UF2 and freeze privately. Update context and push software milestone.
5. Pause for receiver bootloader readiness; flash and verify USB. Separate readiness before any write. Change only an inactive stage by50 DPI, verify, then restore all original stage values and selection. Save pending-recovery marker before SET; on transport failure pause for wired recovery using saved original values. Never retry an ambiguous mutation blindly.
6. Restore exact tested normal v14 and ask for movement/click check. Record hardware qualification separately. Full CORE, arbitrary profiles, bindings and lighting remain outside this milestone.

Installed CORE source SetDPIStages2Device supplies big-endian X/Y stage values to group1/op1. Existing mouse dump dispatch15290, setter1530a..153f8 stores values then applies sensor setting; all allowlisted fields need strict bounds. Independent backup is already saved (three profiles,40 records); preserve opaque binding data privately. No copied vendor source is committed.

Software validation complete:13 native tests, shared read-only helper tests and extended host fault checks pass. V25 saved as config-hid-v25-f577b7e0.uf2; not flashed. Wired recovery connection is present and40 read-only records match RF backup. Actual value mutation/recovery remains pending fresh physical readiness.
