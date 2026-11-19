# Glorious CORE compatibility implementation plan

**Goal:** Use the installed Glorious CORE to configure the mouse through the DIY receiver before the four-hour trial.

**Architecture:** Preserve tested normal v14. Recover CORE's USB configuration protocol and the mouse's RF command/reply framing before exposing a compatible receiver interface. Keep configuration work out of the motion hot path.

**Tech stack:** Existing Zephyr C receiver, Windows HID Feature reports, installed CORE, PowerShell/HidSharp for a read-only reference.

## Scope and decisions

- User explicitly chose CORE compatibility before the pending four-hour trial; separate app is a fallback.
- Genuine sensor DPI and mouse-side bindings, not movement scaling.
- Installed CORE identifies Series One PRO with `LEDSetting: false`. General lighting is not exposed for this model; DPI-stage colour has its own protocol.
- Retain automatic normal operation, 1 ms USB mouse interval and neutralization. Do not promote the experimental 10 us RF wait.
- Keep proprietary source extracts, addresses and hardware observations ignored.
- Do not enter the mouse bootloader, run an updater, or fabricate successful command replies.

## 1. Read-only protocol reference

Create `research/read_mouse_settings.ps1`, reusing the bundled 32-bit HidSharp transport pattern in `research/read_mouse_address.ps1`.
Allow only fixed version, polling, DPI-stage and selected-stage queries. Validate reply status, identity fields, bounds and report length. Include an offline self-test for packet construction, known replies and malformed replies.
Run self-test without hardware. Pause for wired readiness and CORE closure before actual queries; save raw replies privately.

## 2. Trace radio transport

Inspect the existing mouse application dump and disassembly, not another firmware extraction.
Document command fragmentation, sequence acknowledgement, duplicate handling, command completion and reply queue framing. Distinguish static inference from hardware evidence.
First receiver diagnostic must send an allowlisted read query only. Test fragment boundary and retry behavior offline before building/flashing it. Preserve exact v14 rollback.

## 3. CORE-compatible transport

Only after read-query round-trip validation: add compatible vendor Feature transport, status polling and required input notification collections. CORE detects both USB IDs and HID usages; matching a product name is insufficient.
CORE initialization can write profiles from its database, so do not expose the receiver to the running app until command validation, unsupported-command failure and bounded queues work.
Review keyboard/consumer report forwarding for mouse-side keybinds, including release on error, stop and reconnect. Standard mouse forwarding alone cannot implement keyboard bindings.
Test the actual shared firmware paths and normal mode regressions. Then build a separate candidate; ask physical readiness before flashing.

## 4. Hardware qualification and handoff

Validate detection, readback, reversible DPI change/restore and one reversible binding change/restore using harmless documents. Confirm normal motion and 1 ms interface remain intact. No update/bootloader commands are forwarded.
Restore exact v14 if the diagnostic fails. Record findings and limitations in `research/CORE_COMPATIBILITY.md`, update context/resume after each milestone, commit and push this feature branch. The four-hour trial stays pending until configuration work is resolved.

Completion before November 21 is a target, not a guarantee; RF settings compatibility and fresh sensor 1 kHz are not established by a USB descriptor.
