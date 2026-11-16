# Plug-and-play receiver

User now requests automatic operation from USB plug until unplug. This supersedes
the earlier finite-only/manual-start choice for the normal build. Keep the
diagnostic receiver variant bounded and preserve the validated v13 UF2.

The new `dongle` app reuses receiver radio/HID logic with `NORMAL_DONGLE` enabled.
It waits for USB configuration, initializes radio and automatically starts once,
without CDC DTR, a terminal or a host arm command. It has no discovery-arm timeout,
session duration, ACK-attempt or rediscovery limit. Link gaps neutralize buttons
and resume discovery. Counters remain unsigned telemetry and may wrap without
affecting the link. USB reset restarts normal startup; errors fail closed.

Normal config disables printk, boot banner and UART console so absent serial
readers cannot stall radio processing. CDC command input remains optional: `x`
neutralizes and stays stopped; a later explicit `p` or physical reboot restarts.
The bounded receiver also gains explicit rearming after a completed neutral stop.
This is the same shared session preparation, guarded against USB failure.

Tests exercise the actual common guards in probe, bounded receiver and normal
dongle modes: no serial command needed, USB-not-ready refusal, old time/counter
limits exceeded, telemetry wrap, repeated mouse discovery, x latch and TX errors.
Bridge checks cover new-session dedup/counter reset without clearing USB faults.
Keep mapping, RF/ACK timing, idle-sequence hold and quiet active behavior intact.

Build dongle/receiver/probe/radio; validate UF2 bounds, config and staged sources;
preserve v13 and new image, push software checkpoint. Before flashing, physical
readiness must keep Glorious wired/OFF because this image automatically activates.
After flash, verify board USB identity and product/image provenance. Then test
with NO CDC reader and NO arm command: user turns wireless mouse on in harmless
workspace, checks controls/power cycles, and stops by mouse OFF or receiver unplug.
Do not claim an unarmed passive startup for this build. Check a normal receiver
USB replug under safe readiness to prove startup independence. Capture mouse-only
host events separately; absence of serial diagnostics is intentional.

Hardware result is pending. Until qualified, retain v13 rollback and report the
known abrupt-unplug held-state behavior observed with both DIY and wired mice.
No generic Windows driver replacement, persistent mouse writes or input injection.
Current work deadline remains 02:00 AM Manila November 17, 2026.
