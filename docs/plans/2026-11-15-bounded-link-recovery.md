# Bounded Link Recovery Implementation Plan

> Execute in this chat under the user's existing authorization to continue until physical intervention is required.

**Goal:** Exercise sustained input and mouse power-cycle recovery in one explicitly armed, finite receiver session.

**Architecture:** Reuse the existing discovery/ACK and USB bridge. Receiver v4 permits up to eight discovery attempts and 8192 total ACK attempts within 60 seconds of the first successful discovery. A one-second gap since the last successful ACK disables radio, sends neutral buttons, and resumes discovery scanning without extending the session or replenishing budgets. Non-HID probe behavior stays unchanged.

**Tech Stack:** Existing Zephyr4.2/nRF52840 C, native GCC harnesses, Python unittest, PowerShell build/UF2 checks. No dependencies.

## Scope and assumptions

- One known mouse/board; manually arm once per boot, initial discovery window30s.
- RF identity/CRC/channel/shape filters and response bytes/timing stay unchanged.
- Success means a longer bounded session and reconnect after mouse power cycle, not an unbounded receiver or1000Hz claim.
- One-second silence is a conservative experimental release threshold, not a measured protocol timeout. Any valid successfully ACKed idle/status/report refreshes it; malformed/wrong identity/USB busy do not.
- USB reset/disconnect/error remains fatal and requires reboot. Neutral delivery cannot be guaranteed after USB failure.
- Raw logs, device address and binary remain ignored. Preserve tested v3 UF2 before building.

## Decisions

1. Bounded recovery rather than unbounded auto-start: keep deliberate physical trials and finite RF activity.
2. Reuse observed discovery rather than guessing hopping: only rediscover the already known mouse on known discovery channels.
3. Release before leaving an active link; reset report deduplication only after a new successful discovery. This accepts a restarted sequence while preserving retry suppression inside a link.
4. Keep logging for this measurement stage. IRQ/DMA optimization and quiet operation need separate evidence/work.

## Implementation and validation

1. Preserve current receiver artifact locally and create `feature/bounded-link-recovery` from the tested HID milestone.
2. Extend `firmware/test_probe.c` with failing assertions for receiver60s/8192/eight-discovery limits, shared deadline across reconnects, silence boundary, neutral failure, idle refresh, no budget replenishment, and exhaustion. Keep existing probe-mode assertions256/2s. Extend `firmware/test_wireless_hid.c` to prove the same sequence is new after explicit new-link reset.
3. In `discovery_probe.h`, add receiver-only session deadline/discovery count/last-ACK state. `probe_discovery_allowed()` checks both finite budgets and initial-arm/session deadline. Consume discovery attempts before TX; first success sets the sole session deadline. Add link-stop and neutral-before-recovery helpers. Preserve fail-closed TX behavior.
4. In `wireless_hid.h`, add a minimal new-link sequence reset; call it only after discovery succeeds. In `main.c`, check receiver silence/budgets/USB status during listening, disable radio before neutral release/recovery, and use the shared session deadline. Keep non-HID modes behaviorally unchanged. Print v4 limits/recovery policy and link transitions.
5. Run `python -m unittest discover -s firmware -p 'test_*.py'` with native GCC available; confirm intended failing tests before implementation, then all pass. Build/check receiver/probe/radio through `firmware/build.ps1`; check default HID unchanged, private staged header, config/binary separation and UF2 bounds. Review callers and cleanup paths.
6. Update README/context/research with limits, hashes, source/build state and exact physical next step; commit/push tested software checkpoint.
7. PAUSE via question pop-up for NICENANO bootloader entry with mouse wired/OFF before verified flash. Then passive enumeration, separate harmless-document readiness, one95s host capture covering30s initial arm plus60s session and5s cleanup margin. Test sustained controls and mouse OFF/ON recovery. Never silently reuse a consumed arm.

## Deferred

Unbounded operation, USB reconnection recovery, inferred channel hopping, performance optimization, extra controls, measured latency/throughput and gaming polling rate.

## Execution checkpoint

Feature branch created;hardware-tested v3 preserved and SHA2564bc43f35... verified.
New native tests failed on missing implementation,then all8 tests passed after
the receiver-only changes. Final guard cases also cover discovery TX failure,
ACK exhaustion and exact session deadline. Receiver/probe/radio builds,UF2
bounds and mode/config/binary/staged-source review PASS. Default HID artifact
unchanged. Receiver v4 hash2162a852...;full metadata and exact next steps in
PROJECT_CONTEXT.md. Physical flash and5s passive COM12/HID startup subsequently
passed;v4 fresh/unarmed,budgets unused. RF recovery remains pending,separate
input readiness required. Do not repeat the completed flash/passive check.
User requests project pause by14:30 Asia/Manila on2026-11-15. No physical session
will begin near that cutoff;save exact state and close live sessions before pause.
