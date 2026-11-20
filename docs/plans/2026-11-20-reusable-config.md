# Reusable configuration and selected-DPI staging

**Goal:** Read actual polling/DPI settings repeatedly through the receiver, then demonstrate a reversible selected-stage change under prototype USB IDs.

**Scope:** Keep the proven vendor descriptor and suppress pointer output. Permit only exact firmware-version, polling, profile-1 DPI-stage and selected-stage reads, plus profile-1 selection of stages1..4. No arbitrary settings, updater, address, binding, lighting or firmware commands. CORE cannot attach to the prototype identity. Preserve exact normal v14 and tested v22 images.

## Shared transaction behavior

Extend `config_read.h` with canonical request validation and per-request storage.
Validate reply target/group/operation and bounded payload size. A completed reply
must also be acknowledged by the mouse advancing its data-queue sequence before
publication; repeated final packets do not release the transaction.

Extend `config_hid.h` to allow the next command only after publication and host
GET consumption. Keep one pending slot, reject overlapping SET, freeze the
USB-owned snapshot, cap fragment attempts and impose a4-second main-loop deadline.
Any failed/ambiguous transaction locks out further SET until reboot, preventing a
late old reply from being accepted as a new response. Reset/detach keeps existing
receiver restart policy. Main owns RF state; callbacks never perform RF work.

## Native checks

Add behavioral checks before implementation: allowlist and padding rejection,
short/long replies, final-packet retransmission and release barrier, two successive
different commands, busy/consumption gate, timeout lockout, independent command
sequence wrap and immutable USB snapshots. Retain normal/probe/v20/v22 regression
coverage. These tests do not establish Windows or real RF reliability.

## Host and recovery

Reuse the existing settings packet construction/parser through one shared
PowerShell protocol file; retain original wired read-only helper behavior.
The staging helper verifies the exact new product and unique Feature interface,
reads baseline version/polling/DPI values and selected stage, and saves a private
backup BEFORE any change. Read-only mode is the first hardware step.

Only with explicit physical test readiness, select an alternate existing DPI
stage, read it back, then restore and verify the original in a finally block.
Mark recovery needed on disk before the write; clear it only after verified
restoration. On transport failure stop: use fresh wired readiness and the trusted
wired configuration interface to restore the saved selection. Do not attempt to
reuse a quarantined receiver or fabricate a successful restore.

## Build and hardware gate

Use distinct v23 product/helper gate, frozen UF2/hash and source metadata. Run
native suite, host protocol/transaction self-checks, build/config/UF2 validation.
Pause for flash readiness, then separate stationary mouse readiness. No timed
power-on cue needed. Restore tested normalv14 after the diagnostic batch, verify
USB identity and ask user for ordinary wireless movement/click confirmation.
Keep context/resume current and push tested milestones.

This is configuration transport qualification; CORE identification/full profile
writes and the four-hour trial remain later work. Selected DPI is a real mouse
setting, not receiver movement scaling. Sensor-DPI calibration is not claimed.


## Software validation checkpoint

V23 built/UF2-validated and frozen ee7f5a7a, notflashed.13 native checks pass;
host transaction/recovery workflow self-checks and originalwired-reader check
pass. Activeprofile read00/85 was added fromCOREsource so selection writes
require actualactiveprofile1. Vendor descriptor preserved fromworkingv22.
Hardware queries/reuse/selection change/recovery remain unqualified. No actual
settings writes yet; normaltestedv14 remains installed and confirmed working.
