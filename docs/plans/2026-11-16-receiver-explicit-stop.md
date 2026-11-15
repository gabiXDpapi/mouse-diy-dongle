# Receiver Explicit Stop Implementation Plan

**Goal:** Add a one-way host `x` stop before extending the validated v8 session limits.

**Scope:** `discovery_probe.h`, `main.c`, native probe harness, README, research, context and resume prompt. Keep 65,536 ACKs/60 seconds/eight discoveries/one-second link gap, mapping, ACK timing and quiet-active diagnostics. Probe/radio command behavior and default HID remain unchanged; shared-source edits may alter assertion locations and image hashes. No dependency or persistent mouse setting change.

**Design:** Receiver-only cancellation latch rejects discovery and arming. Poll the existing CDC command input at most once per 10ms from both scan entry and the active loop. On cancellation disable RF before the existing neutral-release path, print an explicit disarmed response only after successful release, and require board reboot to arm again. Cancellation before first arm or during rediscovery also latches off. A failed disable/release follows the existing fatal-error path, with no success acknowledgment. Poll cadence is not a physical stop-latency guarantee. Preserve v8 rollback and do not increase limits in this version.

1. Add a failing native active-stop regression using actual `poll_probe_arm`, stop predicate and ACK guard. Change the old unknown-command fixture from `x` to `z`. Run focused harness and verify failure on v8.
2. Implement the cancellation latch, paced polling and radio-before-neutral orchestration. Test stop before arm, during active link and rediscovery; repeated stop and `p` cannot rearm; no ACK/budget/HID forwarding after latch; neutral error cannot report success; paced poll honors pending commands. Keep existing bounds and protocol tests.
3. Run all eight native tests. Build receiver/probe/radio, validate UF2/config/source/banner and control hashes; default HID existing image must remain unchanged. Preserve v8, record v9 hash/footprint and software checks.
4. Update/push checkpoint on `feature/receiver-explicit-stop`. Pause for physical intervention/break. No automatic flash or arming. Later verified flash/passive readiness and a held-left `x` trial are required before any longer-session version.

Assumptions: one user's receiver/mouse, same saved DPI/Windows settings; private captures/addresses/binaries remain ignored. A bounded sustained experiment is the next objective; this version supplies its missing explicit-stop prerequisite. Existing authorization is to execute until physical intervention, so no additional design-confirmation round is needed.

## Software checkpoint

Active-stop regression failed on v8 before repair. All8 checks pass on v9;
receiver/probe/radio builds and UF2 checks pass. Receiver staged sources/config/
banner/disarmed string match. Probe/radio hashes changed after shared edits;
byte identity is not claimed. Default HID existing hash unchanged,not rebuilt.
V9 hash ecc725eb...,61536 flash/16128 RAM,241 UF2 blocks. V8 preserved. No
flash/arming/live capture. Physical flash/passive readiness is next,then held-left
x-stop and motion regression before increasing session limits. User wants a
break after this preparation step.
