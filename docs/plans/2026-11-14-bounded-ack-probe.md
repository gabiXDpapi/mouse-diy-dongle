# Bounded ACK Probe Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Test whether the V4 ACK can release several consecutive mouse reports.

**Architecture:** Keep the separate compile-time probe and V4 radio/timing path.
Replace its single data ACK with at most 16 data ACK attempts within the existing
two-second window after discovery; keep one discovery response and one arm per boot.
Any TX failure ends the remaining budget. Keep exact CRC/address/channel/length/tag
filters, response bytes, raw logging, and no HID output. Stay on channel51 to test
the next step of the observed exchange; do not invent channel hopping.

**Tech Stack:** Zephyr4.2, nRF52840, C, native GCC harness, Python unittest, PowerShell.

---

1. Extend `firmware/test_probe.c`: finite count, deadline not extended, wrap and
   peer flags, timeout exhausts remaining budget, no rearm. Run
   `python -m unittest discover -s firmware -p 'test_*.py'` and confirm failure.
2. Modify `firmware/radio_logger/src/discovery_probe.h`: `ack_count`, fixed limit16,
   consume before TX; exhaust on failure. Modify its only runtime caller in
   `src/main.c` to use count and log ACK ordinal; update V5 banner. Keep all packet
   filters and V4 timing code unchanged. Update CMake option description.
3. Run all seven tests, preserve hash-checked V4 UF2 under ignored evidence, and
   build `firmware/build.ps1 -App probe`. Validate hash and application-only bounds.
4. Rebuild `-App radio` to check TX compile-out remains intact. Check cache flags
   and banner/arming strings in both binaries. Do not modify the default HID app.
5. Update `PROJECT_CONTEXT.md`, `README.md`, `research/RADIO_ANALYSIS.md` with
   artifact hashes, limits, test results, board state, and next hardware procedure.
6. Await user bootloader reentry with mouse wired. Then identity/hash-checked V5
   flash and passive baseline; wait for battery readiness before one armed capture.

User subsequently authorized committing and pushing tested milestones; explicitly
stage project source/research/docs, excluding local IDE settings, board-drive copy,
private evidence and binaries. Do not rewrite history.
Execution continues in this chat under the user's instruction to proceed until
physical intervention is needed. No delegation or extra workflow dependency needed.
