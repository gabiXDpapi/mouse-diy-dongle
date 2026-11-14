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

## V6 follow-up from the V5 capture

V5 advanced sequence0..15 but rejected the observed20-byte tag41+tag45 report.
Keep the16-attempt/two-second limits. Add the captured packet as a native matcher
fixture, reject all unsupported lengths and wrong tags/addresses/channels, and
confirm the new valid fixture fails against V5. Then extend only the matcher to
accept length11/tag41 or length20/tag41+tag45; update banner to V6. Preserve the
V5 UF2 before rebuilding. Run seven tests, probe build/UF2 validation and receive-only
build/separation check. Update context/research/README, commit/push this tested
milestone on the same feature branch, then request bootloader reentry. No type5
decoding or extra transmissions.

## V7 follow-up from the V6 capture

V6 acknowledged the compound report and advanced to sequence 16. Raise only the
ACK cap to 256 and derive the V7 banner's limit from the constant. Keep the
two-second deadline and all filters/bytes/timing unchanged. Add expiry-after-partial
budget and explicit fixed-limit assertions to the native tests; preserve V6 UF2.
Run tests, both builds/UF2 checks and binary separation checks, then update/push
the milestone before requesting bootloader reentry. No HID or general parsing.

## V8 follow-up from the stationary wheel test

V7 ACKed17 reports then rejected captured idle payload1100. Stock0x18938..0x18976
confirms the queue-empty two-byte form. Add only length2 to the existing matcher,
checking length before reading tags; rename it is_ack_candidate in its runtime
caller and native tests. Keep bytes/timing,256-attempt/two-second guards. Include
length in ACK logs (new prefix `PROBE ACK sent:`), update V8 banner. Test a real
three-byte radio buffer (length byte + two payload bytes), wrong addresses/channel,
truncated/extended idle and repeated idle ACKs without extending deadline. Preserve
V7 UF2, build both variants/validate bounds/separation, update/push notes and
request bootloader reentry. No HID or general packet parsing.
