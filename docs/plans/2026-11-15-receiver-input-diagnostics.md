# Receiver Input Diagnostics Plan

**Goal:** Explain whether quiet receiver traffic reaches the USB bridge before another responsiveness comparison.

**Architecture:** Receiver v6 retains v5 RF/USB mapping,recovery,60s session,eight-discovery cap and manual arming. Increase the finite shared ACK cap to65536 because v5 exhausted8192 in about9.7s of observed counter progress. Add successful-ACK counters for idle/status/type1 and counters for successful unique HID report submission and suppressed report retries. Include them in existing1s/final-link summaries. No per-packet text.

**Scope:** `discovery_probe.h`, `wireless_hid.h`, `main.c`, actual native tests, README/context/research. No dependencies/config change. Preserve v5 image. Branch `feature/receiver-input-diagnostics`.

1. Failing native assertions for65536 cap,successful ACK classification,HID submission vs retry,USB busy/error/short-write exclusion and counter continuity across new links.
2. Minimum counter increments in existing successful paths;print cumulative values in stats and final neutral-complete link summary. Counts of queued USB reports do not prove Windows delivery.
3. Run eight native tests;build/check receiver/probe/radio,UF2 bounds,mode/binary/source separation and default HID/preserved artifacts.
4. Save metadata/handoff and push software checkpoint. PAUSE BEFORE FLASHING until user returns;no more hardware test before break.

Next after break: verified v6 flash/passive check,separate readiness,single-arm95s capture. Compare ACK types vs HID submissions and user pointer response.65536 remains a cap;it does not guarantee60s if actual ACK cadence is higher. No latency or reliability claim until measured.

**Completed software checkpoint:** all8 native tests pass,including error/short-write/neutral exclusion and cumulative counters. Receiver/probe/radio builds,UF2 bounds and mode/source/binary checks PASS;default HID unchanged. V6 hashc0e18e29...,full metadata in context. PAUSED BEFORE FLASHING;hardware remains expired/passive v5,all live sessions closed.

**Hardware checkpoint after return:** verified v6 flash and5s passive COM12/HID
startup PASSED on2026-11-15 about22:41 Manila. V6 unarmed/budgets unused;port
closed. Pause for separate harmless-document/mouse-OFF-unplugged readiness,
then one95s capture. Do not repeat completed flash/passive check.

**RF checkpoint:** v6 controls/power-cycle recovery/clean stop confirmed by user,
with improved perceived responsiveness.55417 ACKs,14557 unique HID submissions,
51 retries,95 USB-busy events,no errors. User reports intermittent motion stalls;
do not claim reliable/gaming completion. Next focused steady-motion test on
unchanged v6 before selecting a fix;exact state/procedure/evidence in context.
