# Receiver Quiet Diagnostics Plan

**Goal:** Compare perceived input delay with receiver per-packet USB text removed.

**Architecture:** Receiver v5 keeps v4 RF bytes/timing,USB mapping,recovery and finite budgets. Replace receiver RX/ACK lines with at-most-once-per-second cumulative counters. Retain discovery and first-ACK hardware timing samples per link,link transitions,errors and USB-busy diagnostics. Non-HID probe/radio remain verbose.

**Scope:** `discovery_probe.h`, `main.c`, `test_probe.c`, README/context/research. Reuse existing build/test tooling;no dependency/config change. Keep v4 preserved for comparison. This is a hypothesis test,not a latency fix claim.

1. Add failing native assertions for diagnostics rate limiting and per-link timing sampling.
2. Add receiver-only rate-limit/trace flag;update logging gates and v5 banner. Do not alter ACK payloads,turnaround,channel/address filters or HID semantics.
3. Native tests,receiver/probe/radio builds,UF2 and compiled-image/source separation checks. Verify per-packet format strings absent only from receiver.
4. Update handoff/hashes and push tested software milestone on `feature/receiver-quiet-diagnostics`.
5. Pause via pop-up for physical flash readiness. Test against the same harmless document and compare responsiveness. Keep original60s/8192-ACK/eight-discovery limits and single-arm95s capture.

Counters and limited timing samples cannot measure per-report latency. If delay persists,investigate RF scheduling/ACK timing and queued reports with a separate measurement plan. Do not claim1000Hz or end-to-end latency from aggregate counters.

User cutoff: pause by14:30 Asia/Manila on2026-11-15. Hardware remains v4 expired/passive until separately verified flash.

**Software checkpoint:** all8 native tests pass after intended failing assertions.
Receiver/probe/radio builds,UF2 bounds and compiled-format/config/source checks
pass. Receiver v5 SHA256ab8e3001...,full metadata in context. No per-packet RX/ACK
format strings in receiver;other variants keep their logging. Tested v4 preserved;
default HID unchanged. V5 subsequently flashed and passed5s passive COM12/HID
startup;fresh/unarmed,budgets unused. PAUSED for user's departure before any
armed comparison. Resume only after return/separate input readiness;do not
repeat completed flash/passive check. No latency improvement established yet.
