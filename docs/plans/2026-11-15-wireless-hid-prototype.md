# Bounded Wireless HID Prototype Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Forward the experimentally supported mouse reports to Windows during
the existing explicitly armed, two-second diagnostic window.

**Architecture:** Reuse the V8 radio/probe source and the original HID app's
Zephyr USB APIs. Add a separate `receiver` build with CDC+HID and private generated
radio parameters. Preserve the default HID demo and the radio/probe variants.

**Tech Stack:** Zephyr4.2 USB legacy HID/CDC, nRF52840, C/native GCC tests,
Python unittest and PowerShell build script. No new dependencies.

---

## Understanding and assumptions

- One user's known mouse and nice!nano; local USB test IDs only.
- Keep V8 RF bytes/timing, one arm/discovery,256 ACK attempts and two-second window.
- USB report: six bytes, buttons5/padding3, signed16 LE X/Y, signed8 wheel.
- Preserve full motion range; no scaling, arbitrary clipping or startup-prefix drop.
- Only unique validated type1 reports are forwarded. Idle never consumes sequence;
  a duplicate data report is ACKed without replay. Sequence255->0 is new data.
- USB busy means no RF ACK and no deduplication advance; a retry can be forwarded.
- Submit USB report before ACK so data is owned by USB before releasing mouse queue.
  This can change software turnaround; hardware timing must be checked.
- Stop/error sends neutral buttons with bounded waits. USB failure cannot guarantee
  host delivery; stop RF/output and report the failure. USB reset/disconnect after
  configuration latches output off until board reboot for this prototype.
- No continuous receiver,1000Hz claim, horizontal/unknown consumer output, channel
  recovery or persistent mouse writes. Those are later work, not hidden promises.

## Decisions

1. Separate compile-time receiver variant rather than modifying the passive/probe
   behavior: keeps hardware comparisons and recovery artifacts available.
2. Six-byte independent descriptor rather than eight-byte vendor report copying:
   expose supported mouse controls and omit unresolved consumer fields.
3. USB enqueue before ACK rather than silently dropping on endpoint busy: preserves
   report delivery semantics; test and measure the turnaround cost.
4. Keep current two-second limits rather than add sustained behavior at the same
   time as USB integration. First establish wireless pointer/click/wheel output.

## Implementation steps

1. Add `firmware/test_wireless_hid.c` and Python runner using mocked USB/semantics.
   Cover no output before configuration, valid byte mapping/extremes, duplicates,
   idle->same-sequence data, USB busy/error/short write, retry, wrap, neutral release,
   release timeout and reset/disconnect latch. Confirm failure before helper exists.
2. Add `radio_logger/src/wireless_hid.h` with the actual descriptor/USB bridge.
   Reuse asynchronous completion semaphore and static USB buffer from the HID demo.
   Tests exercise this header directly; no separate mock implementation of mapping.
3. Extend the actual `send_report_ack` path to gate on the HID bridge only in the
   receiver build, before consuming RF budget. Native protocol tests run both modes
   and prove USB busy/error causes no ACK/TX/budget consumption.
4. Integrate helper initialization/status callback, EAGAIN retry, neutralization on
   listen exit/error, and receiver-specific banner in `radio_logger/src/main.c`.
5. Add `receiver.conf`, CMake `WIRELESS_HID` gate, and build script `-App receiver`.
   Distinct local product/PID2FE3:0008,1ms HID endpoint; no throughput promise.
6. Run native tests; build receiver,probe,radio and default HID. Validate all UF2
   bounds and binary/config separation. Preserve V8 before rebuilding probe.
7. Update README/context/research with hashes, states, limitations and hardware
   procedure; commit/push `feature/wireless-hid-prototype`.
8. Stop for user bootloader reentry. Then verified flash/passive baseline, explicit
   input readiness on an empty workspace, mouse power-cycle to clear old queued
   actions, and one armed test. No automatic physical flash/readiness assumption.

Execution continues in this chat under existing authorization to work until
physical intervention is required. No extra design-confirmation round is needed.

## Receiver v2 follow-up

Fresh power-on exposed a standalone length11/tag45 status packet; v1 rejected
it and no type1 report reached USB. Add only that form to the ACK matcher and
ignore it in the USB bridge without consuming movement sequence. Add actual
status fixtures to matcher and bridge tests,including no USB output while an
endpoint is busy. Preserve v1 UF2,update receiver/probe banners to v2/v9,run eight
tests and rebuild those two variants. Confirm radio/default HID remain separate
and unchanged. Push this fix before requesting NICENANO reentry. Timing,limits,
descriptor and report mapping remain unchanged.

## Receiver v3 follow-up

V2 established user-confirmed wireless pointer control. Give the next interactive
test enough time for clicks/wheel:receiver-only fixed1024 ACK cap/10000ms active
window. Keep non-HID probe256/2000ms and discovery arm30000ms. Define shared
fixed constants in discovery_probe.h,drive actual ACK/listen deadlines and log
limits from them,and update receiver banner v3. Native protocol assertions cover
both modes' limits and actual deadline;all existing guards remain. Preserve v2,
build receiver/probe/radio as affected,check default HID unchanged and separation,
then push before requesting NICENANO. No RF bytes/timing or USB mapping changes.
Future capture45seconds covers the maximum30s arming plus10s data window.
The larger packet budget also requires a status-only full-wrap regression: when
status advances transport beyond the last delivered report,invalidate that old
retry marker without generating input. Idle still leaves the marker unchanged.
This prevents suppressing a genuinely new report after a256-status sequence gap.

## Hardware validation checkpoint (2026-11-15)

Receiver v3 now has user-confirmed bounded Windows movement,left/right clicks,
wheel up/down and stop with no stuck buttons. Controls capture611 ACKs over9980ms,
383 unique type1 reports,no firmware errors or USB busy. Use one continuous
COM12/DTR reader with2s pre-arm drain,ONE p,new arming confirmation before user
power-on,and45s post-arm capture;save host phase timing to distinguish queued
serial output. Private evidence/details and exact board state are in context.
Forced neutral while physically holding a button at expiry remains a separate
hardware check. Continuous operation/recovery and gaming performance remain open.
