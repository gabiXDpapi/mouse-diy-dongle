# Glorious Series One PRO Centauri — Custom Wireless Receiver Project

## Project Goal

Develop a custom 2.4 GHz USB wireless receiver for a Glorious Series One PRO Centauri gaming mouse whose original dongle is missing.

The replacement receiver will use a nice!nano V2-compatible nRF52840 development board.

The intended final result is a reliable gaming mouse receiver with approximately 1000 Hz polling, if the proprietary wireless protocol can be recovered and implemented.

## Hardware

**Mouse**

- Model: Glorious Series One PRO Centauri
- Original wireless receiver: Missing
- Mouse MCU: Nordic nRF52840, identified in published teardown material
- Wireless connection: Proprietary 2.4 GHz
- Original receiver USB ID reported in third-party device databases: `258A:2031`
- Mouse's wired USB ID confirmed by user Device Manager screenshot on 2026-11-14: `258A:2018`.
- Observed wired mouse interface Hardware Id: `HID\VID_258A&PID_2018&REV_0101&MI_00`.

**Development board**

- Type: nice!nano V2-compatible nRF52840 clone
- Purchased from Shopee for approximately ₱200
- USB-C interface
- Board received and physically inspected
- No pin headers soldered

**Verified bootloader information**

The board successfully enumerates on Windows as a USB drive named `NICENANO`.

The `INFO_UF2.TXT` file contains:

```text
UF2 Bootloader 0.6.0
Model: nice!nano
Board-ID: nRF52840-nicenano
SoftDevice: S140 version 6.1.1
Date: Jun 19 2021
```

This verifies a working nice!nano-compatible UF2 bootloader. The exact physical MCU has not been independently verified through a debug probe.

## Current Progress

- [x] Researched Series One PRO hardware
- [x] Selected an nRF52840 development platform
- [x] Purchased and received the nice!nano-compatible board
- [x] Connected the board to Windows
- [x] Verified USB bootloader enumeration
- [x] Inspected `INFO_UF2.TXT`
- [x] Establish firmware development environment
- [x] Implement USB HID mouse proof of concept
- [x] Confirm the HID firmware runs on physical hardware
- [x] Investigate pairing utility and retrieve the mouse RF address
- [x] Recover tested discovery, idle/status ACKs and basic movement report format
- [x] Implement bounded RF discovery and ACK sessions
- [x] Confirm wireless movement-to-USB HID on physical hardware
- [x] Verify wireless left/right clicks, wheel up/down and normal button release in Windows
- [x] Verify forced left-button release while held at active-window expiry
- [ ] Complete sustained link/reconnect behavior and remaining protocol recovery
- [ ] Test and optimize gaming performance

## Development Plan

### Milestone 1 — Development Environment

Set up a C/C++ firmware development environment suitable for the nice!nano-compatible nRF52840.

Prefer a workflow that preserves the existing UF2 bootloader and supports firmware flashing over USB.

### Milestone 2 — USB HID Prototype

Create firmware that enumerates as a USB HID mouse.

Initially generate simulated mouse reports to verify USB communication.

### Milestone 3 — Radio Initialization

Configure the nRF52840's proprietary 2.4 GHz radio and establish controlled radio experiments.

### Milestone 4 — Protocol Investigation

Investigate Glorious' proprietary wireless protocol using publicly available firmware, Glorious pairing software, USB/HID captures, and RF observations where appropriate.

Determine the packet format, RF address, channel selection, bitrate, acknowledgements, pairing information, and any authentication.

### Milestone 5 — Receiver Implementation

Implement the recovered radio and pairing behavior on the nice!nano.

Translate the mouse's movement, clicks, and scrolling into standard USB HID reports.

### Milestone 6 — Gaming Performance

Measure polling consistency, latency, packet loss, and reliability.

Target 1000 Hz USB HID reporting when technically achievable.

## Important Technical Constraints

- The exact Series One PRO RF protocol is not yet known.
- Compatibility with the Glorious mouse is **not guaranteed**.
- The nRF52840 supports proprietary 2.4 GHz radio and native USB, but this does not establish protocol compatibility.
- The nice!nano alone may be insufficient for discovering an undocumented radio protocol; additional RF capture hardware might be needed.
- Do not erase or overwrite the bootloader unnecessarily.
- Do not flash binaries intended for other nRF52840 boards without checking their memory layout and bootloader compatibility.
- Do not assume an original Glorious receiver firmware image can run on this development board.
- Prefer an independently written receiver implementation rather than distributing proprietary firmware.

## Earlier Research / Hardware History

Current source, board state and next action are in **LATEST HANDOFF / NEXT** below.
The following notes preserve the earlier milestones and their historical next steps.

The USB HID prototype source is in `firmware/`; see `README.md` for
build, UF2 validation, flashing, and Windows verification. Zephyr v4.2.0 and
SDK 0.17.2 are installed. Blinky execution, power-cycle restart, and double-reset
bootloader re-entry were confirmed by the user. Wireless reception was not yet
implemented at that earlier HID-only stage.

HID hardware results confirmed on 2026-11-14 (Asia/Manila):
- Device Manager screenshot shows HID-compliant mouse with `VID_2FE3&PID_0007`.
- User confirmed right movement followed by left movement, then LED off.
- User confirmed the NICENANO bootloader drive returns.
- No polling-rate or wireless performance measurements have been made.

After recording the wired mouse Hardware Id, the next step was inspecting the
pairing utility, public firmware information and USB/RF protocol evidence.

Official software research: Glorious lists Series One Pro (Forge) as supported by
Legacy CORE v1: https://glorious.ladesk.com/116109-Legacy-CORE-v1-Product-Support-List
Official download page: https://www.gloriousgaming.com/pages/legacy-software
CORE recognition confirmed by user screenshot on 2026-11-14:
- Device name: SERIES ONE PRO WIRELESS.
- Displayed firmware: v1.0.0.0.
- Displayed battery: 100%.
- CORE application version is not shown in the screenshot.
Standalone pairing utility v3.0.0.7 confirmed by user screenshot on 2026-11-14:
- SERIES ONE PRO WIRELESS selected; Mouse: Connected.
- Dongle: Disconnected; Pair button disabled. Original receiver remains missing.
- This confirms wired recognition, not successful pairing or recovered RF commands.
- CORE's embedded pairing dropdown showed only Model O 2 Wireless and Model I 2 Wireless.
- Standalone utility was downloaded via the official software page:
  https://downloads.gloriousgamingservices.com/download/GloriousPairingUtility.zip
- Local executable located at:
  `C:\Users\Gabriel\Downloads\GloriousPairingUtility\GloriousPairingUtilityV3.0.0.7.exe`

Read-only inspection of standalone utility on 2026-11-14:
- Downloaded EXE SHA256: `4A1BAFB07E4A72448970DFFDB5C321839079A393645217119D34588E8EB475CB`.
- 7-Zip identifies a self-extracting archive containing Config.ini and GloriousPairingUtility.exe.
- Embedded Config.ini explicitly maps Series One PRO Wireless to mouse `258A:2018`
  and dongle `258A:2031`, corroborating the previously reported receiver ID.
- Config also lists Series One PRO Wireless under the golden dongle `258A:2032`;
  this is software configuration evidence, not a tested replacement recommendation.
- Read Config.ini with 7-Zip stdout extraction; did not run embedded code or send USB commands.

Further investigation completed on 2026-11-14:
- Decompiled the embedded .NET utility with ILSpyCmd for static inspection.
- Identified its 65-byte HID Feature-report address read and receiver address-write paths.
- Independently queried the user's wired mouse via MI_02: address query succeeded
  with status 0xA1. Exact address/reply saved locally in ignored
  `build-pairing-analysis/mouse-address-observation.json`.
- No address-write, pairing, or firmware update command was sent during address discovery.
- Official firmware manifest downloaded; linked Series One PRO ZIP returns HTTP 403.
- Inspected public reverse-engineering repository commit
  `68baf22302820bb0b927093da99493248a086bc7` from
  https://github.com/0zLloen/mouse-firmware-re (third-party claims, not all verified here).
- Added `research/read_mouse_address.ps1` and `research/read_mouse_firmware.ps1`.
  Both scripts' self-tests passed.
- User then confirmed readiness for the application read. The firmware reader
  successfully entered mouse bootloader 258A:2103 (version B0-01-00-00), read
  93,344 bytes from 0x6000..0x1CC9F, and restored application USB ID 258A:2018.
- Snapshot: ignored `build-pairing-analysis/mouse-app-6000-1cc9f.bin`;
  SHA256 `965c8f8cc17e912d0ede5f0ed82b4c12a15d21fae931aeaf59921c94794e1738`.
- User confirmed normal movement and clicks after the read. Do NOT repeat the
  read merely to resume work. No erase/program commands were sent.
- Static analysis found radio setup and a 16-channel table; details and remaining
  hypotheses are in `research/RADIO_ANALYSIS.md`.
- A separate receive-only diagnostic app is in `firmware/radio_logger/`.
  `firmware/build.ps1 -App radio` stages/builds it outside OneDrive in
  `C:\Users\Gabriel\zephyrproject\build-mouse-radio`.
  Default `firmware/build.ps1` still builds the original HID app.
- Radio parameters are generated from the hash-checked snapshot and local USB
  observation; device-specific values are kept out of tracked firmware source.

Radio logger build completed successfully on 2026-11-14:
- Five Python tests pass (three UF2 checks and two radio-evidence tests).
- UF2: 222 blocks / 113,664 bytes; payload 0x26000..0x33DFF.
- SHA256: `9daaa154dd51db6a6d77d2784ba38c400b6f79a2e3196a07f471cf2bede565fd`.
- Flash 56,688 bytes; RAM 15,872 bytes.
- File: `C:\Users\Gabriel\zephyrproject\build-mouse-radio\zephyr\zephyr.uf2`.
- Uses shared HFXO on/off manager (as USB does), with bounded startup/disable waits.
- The first build failed on an unavailable assertion macro; corrected to Zephyr's
  __ASSERT with CONFIG_ASSERT=y. The successful build includes the clock correction.

Radio logger flashed on 2026-11-14 after verifying NICENANO board ID, S140,
and the UF2 hash. Windows enumerated COM11 (2FE3:0001, MI_00).
An eight-second DTR-enabled serial baseline produced the expected banner,
register settings, and 12 SCAN lines with no errors and all counters zero.
Mouse remained wired. Evidence: ignored
`build-pairing-analysis/radio-baseline-20261114-121111.txt`.

First wireless capture completed after user confirmed battery-powered readiness.
Saved `build-pairing-analysis/radio-wireless-20261114-121312.txt` and summary JSON.
595 CRC-valid packets, all payload 0x20 plus the four USB-read address bytes,
received with the mouse BASE1 mapping, logical address 1. No movement payloads.
User was told they can reconnect the mouse cable after capture.
Static tracing identified the matching state-4 packet construction and separate
discovery channels 3,49,78, omitted by v1. Logged adjacent frequencies may reflect
adjacent-channel reception; exact transmit frequencies need further validation.
State-6 discovery response processing is documented in research/RADIO_ANALYSIS.md.

Logger v2 prepends those three discovery channels (19 total). Six tests, build,
and UF2 bounds checks pass. Flash 56,704 / RAM 15,872 bytes; 222 blocks.
Radio v2 UF2 SHA256 used for the hardware captures:
`d5122f0ad3b6061c6db82be77b0c661d04c1c333b964595d3e3af83c34417882`.
V2 flashed after user confirmed NICENANO; bootloader identity and UF2 hash were
verified. COM11 returned. Five-second baseline confirmed the v2 banner, 19
channels, and six SCAN lines with all counters zero and no errors. Evidence:
`build-pairing-analysis/radio-v2-baseline-20261114-121813.txt`.
V2 wireless capture completed: 855 CRC-valid packets, all identical discovery
payloads on mouse address 1. Channels 3/49/78 now confirmed along with adjacent
frequencies. Evidence: `build-pairing-analysis/radio-v2-wireless-20261114-122244.txt`
and its summary JSON. User was told to reconnect mouse cable afterward.

Prepared separate `firmware/build.ps1 -App probe` experiment. It uses the shared
radio source with DISCOVERY_PROBE=ON; `-App radio` compiles that code out.
Probe starts passive; serial ASCII `p` arms it once for 30 seconds. Only a
CRC-valid exact discovery packet matching this mouse on 3/49/78 can trigger one
0 dBm response per boot. Candidate payload `00 F4 01` (group 0, interval 500)
and 250 us software turnaround are inferred/experimental, not verified receiver
behavior. It then listens on channel 51 for two seconds and resumes passive
scanning; no normal ACKs, HID movement, or persistent configuration writes.
Transmit and radio-disable waits are bounded. Expired/unarmed and second sends
are rejected. Seven tests pass, including native C packet/arming/timeout guards.
Final probe build passes: flash 57,564 bytes, RAM 15,936 bytes, 225 UF2 blocks,
payload 0x26000..0x340FF. File:
`C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`
SHA256: `6e66aba83badd6014ca26d1dd8a10e374f3df4904983b7e681fe79a96f47d69b`.
Do not use the earlier b1a569... probe build, which lacked the final guards.
Receive-only build regression check passed with
DISCOVERY_PROBE=OFF; probe uses ON. Binary banner checks confirm separation.
Rebuilt receive-only artifact SHA256 (same path, supersedes its prior artifact):
`b12e5867f7b4f36a68827e5905a5b0cbce4f824ea468abd43b61045266197559`.
Probe flashed after user confirmed NICENANO; bootloader identity and final probe
hash were verified. COM11 returned. Five-second passive baseline passed: expected
probe banner, 19 channels, six zero-counter SCAN lines, no assertions/errors.
No arming command was sent. Evidence:
`build-pairing-analysis/probe-passive-baseline-20261114-181929.txt`.
Probe v1 armed once after readiness and captured 35 seconds:
`build-pairing-analysis/probe-armed-20261114-182039.txt` plus summary JSON.
One ARMED/one SENT, no errors; 990 valid packets. 31 new eleven-byte packets
on channel 51 (`00 00 41 00 00 01 00 00 00 00 00`) at t=93111..93352 ms,
then return to discovery (959 discovery packets total). Discovery reply accepted
is supported by this transition. Board remains probe v1, budget consumed/passive.
User was told to reconnect mouse cable.

Probe v2 source now adds one data ACK after discovery, two transmissions maximum
per boot. Accepts only exact eleven-byte/type-1 CRC-valid report on channel 51
and mouse address within two seconds of discovery success. ACK length=2,
payload sequence+1 (8-bit wrap), peer_sequence & 0x3f; derived from stock
0x16088..0x160b8 queue release and length<3 bypass of incoming command branch.
No continuous ACK loop or HID output. Seven tests pass with extended native
checks for ACK expiry, malformed reports, sequence wrap, timeout, and no retry.
Probe v2 build and UF2 validation pass: 57,960 bytes flash, 15,936 bytes RAM,
227 blocks, payload 0x26000..0x342FF. Current probe UF2 SHA256:
`be6109e58a548ed6104f257d4bf35d950ab11ad45db1079b40287935a644c3f1`.
Same build-mouse-probe UF2 path; old 6e66... hash is v1.
Probe v2 flashed after verifying NICENANO identity and be6109... hash. COM11
returned; five-second passive baseline confirmed v2 banner, 19 channels and
six zero-counter SCAN lines with no errors. No `p` sent. Evidence:
`build-pairing-analysis/probe-v2-passive-20261114-182615.txt`.
V2 armed once and captured 35 seconds after readiness. Evidence:
`build-pairing-analysis/probe-v2-armed-20261114-182719.txt` plus summary JSON.
One discovery SENT, one ACK SENT seq0->1, no errors. 974 valid packets, including
31 unchanged eleven-byte reports `00 00 41 00 00 01 00 FF FF 00 00` on channel51
and 943 discovery packets. NO sequence advance observed: ACK acceptance remains
unverified. User was told to reconnect mouse cable. V2 budgets consumed/passive.

V3 source removes the extra 250 us delay from data ACK only (discovery retains
250 us) and moves transition logging before enabling channel51 RX to avoid stale
queued packets during USB output. Same two-TX maximum and guards. Seven tests
pass, including delay separation. V3 build and UF2 checks pass: flash57,936,
RAM15,936, 227blocks, range0x26000..0x342FF. Current probe UF2 SHA256:
`1df362581ae875af62762513bb6c0022058de179b263c0844586bd496c99966f`.
V3 flashed after verifying bootloader identity and 1df362... hash. COM11 returned.
Five-second passive baseline passed: v3 banner, 19 channels, six zero-counter
SCAN lines, no errors. No arming command sent. Evidence:
`build-pairing-analysis/probe-v3-passive-20261114-183203.txt`.
V3 armed and captured after readiness:
`build-pairing-analysis/probe-v3-armed-20261114-183305.txt` and summary JSON.
One discovery SENT, one ACK SENT seq0->1, no errors. 975 valid packets: 31
unchanged eleven-byte reports `00 00 41 00 00 01 00 FF FF 00 00` on channel51,
944 discovery packets. NO sequence advance. Board still runs V3 with both TX
budgets consumed; it is now passive until reboot. User was told to reconnect
the mouse cable, but actual physical reconnection has not been confirmed.

V4 adds hardware timing instrumentation instead of another ACK timing guess.
New `firmware/radio_logger/src/probe_timing.h` uses disabled/unclaimed TIMER2 at
1 MHz and two dynamically allocated nrfx PPI channels: RADIO.END -> CC0 and
RADIO.READY -> CC1. Before TX it saves RX END and samples software handling age;
after TX it logs RX-END-to-handler, TX-READY and TX-END microseconds plus MODECNF0.
Initialization checks timer progress and fails closed if allocation/setup fails.
Compile assertions require TIMER2 disabled in DT and CONFIG_NRFX_TIMER2 off.
Existing USB PPI allocation is respected through nrfx_ppi_channel_alloc.
V4 retains V3's discovery delay250us, immediate data ACK, two-TX total limit,
explicit serial-p arming, filters, expiry and no retries. No HID output.

V4 build, all seven tests, and UF2 bounds validation pass. Native C tests cover
protocol guards/timeout/delay selection; timing helpers are stubbed there, so
actual TIMER/PPI capture still requires a hardware check. Current probe UF2:
`C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`
SHA256 `2bef74865e0f59502cdb1d6c7117708ddd5efd6c14ca693c8d3e0dfbed8309e3`.
Flash58,680 bytes, RAM15,936 bytes, 230 UF2 blocks, payload0x26000..0x345FF.
This supersedes the V3 artifact hash above. V4 subsequently flashed; see below.

LATEST HANDOFF / NEXT:
PAUSED FOR BREAK / V8 BOUNDED VALIDATION COMPLETE 2026-11-16:
User requested break around01:10 AM Manila;paused early after final trial.
Resume ONLY when user returns;no automatic hardware action. Branch
feature/receiver-quiet-active-window. User now confirms held LEFT beyond70s,
other pointer did not extend selection:forced timeout release HARDWARE PASS.
Raw up at80.928s matches neutral-complete stop~80.947s after link~20.954s.
V8 motion(no overshoot/wired-like),movement/clicks/wheel,mouse-only OFF/ON
recovery and held-left timeout are all user-confirmed with clean stops.
Last private base receiver-v8-held-20261116-005751:2016 receiver-only raw records,
0 read errors;one arm/discovery,59958 ACKs,5613 HID submissions,23 retries,busy2,
no ERROR,neutral complete. No further build/flash needed for these completed checks.
Board v8 EXPIRED/PASSIVE;current boot one arm/discovery,59958 ACKs used. COM12
closed;no live serial/raw/build. User instructed release buttons/reconnect USB.
V8 UF2 SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and ignored
receiver-v8-5df75330.uf2;v7 rollback preserved. Source/tests/software milestones pushed.
NEXT WHEN RESUMED: design a bounded sustained-session experiment using v8 as
stable baseline,including explicit stop/neutral handling and finite safety limits.
Review existing guards/tests and decide smallest change before building. Do not
silently enable continuous/unbounded operation,increase limits or arm. New live
trial requires explicit physical readiness and normal reboot of used receiver.
No repeated extraction,baseline,flashing or completed v8 tests without new reason.
No physical1000Hz/end-to-end latency/DPI equivalence/daily-use reliability claim.

V8 HELD-LEFT CAPTURE COMPLETE / BREAK PAUSE 2026-11-16: final trial before
requested01:10 AM Manila break. No further trial planned;resume only when user
returns. Private base build-pairing-analysis/receiver-v8-held-20261116-005751.
100s hidden raw exit0,2016 receiver-only records,0 read errors. Button events:
left-down21.635s/up21.690s,then held-down22.116s and up80.928s. Host serial
chunks show link start20.954s and neutral-complete stop80.947s (about60s).
One arm/discovery,59958 ACKs(54322 idle,0 status,5636 type1),5613 HID submissions,
23 retries,busy2,no ERROR. Forced release observed at stop;user confirms trial
performed,but physical-held/selection-fixed observation is PENDING in popup.
Do not mark physical held-left PASS until that response. User asked release
LEFT/reconnect USB. Board v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used;
COM12 closed,no live serial/raw/build. No new firmware changes.
NEXT on resume: collect pending held-left observation,then record final bounded
v8 validation. If PASS,next scope is sustained-session design/testing,not an
unbounded/daily-use claim. No automatic reset/rearm/flash or limit increase.
Keep successful v8 motion(wired-like,no overshoot),controls and mouse-only
OFF/ON recovery milestones;all already user-confirmed. Saved v8 image unchanged.

SESSION PAUSE REQUEST 2026-11-16: user asks break around01:10 AM Manila
and resume later. Pause by01:10;do not start new armed trial after01:07.
Current clock at request00:55. Save/close all sessions/update handoff before
pause. Remaining unchanged-v8 held-left check may fit if user promptly ready;
otherwise leave pending. Do not automatically resume after the break.

V8 CONTROLS/RECOVERY HARDWARE PASS 2026-11-16: user confirms movement,
left/right clicks and wheel UP/DOWN worked before/after ONLY mouse OFF/ON,
receiver untouched;clean stop,no stuck buttons. Private base
build-pairing-analysis/receiver-v8-controls-20261116-005207.100s hidden raw
exit0,4982 receiver-only background records,0 read errors,4865 motion.
Left down/up32/32,right4/4,wheel63(41 negative120,22 positive120). Both wheel
directions captured in later segment;before-recovery wheel supported by user
report,not independently present in raw events. No cadence conclusions from
background capture. Serial one arm,two discoveries/links;second discovery had
30371ms remaining in original session. Final53698 ACKs(21225 idle,2 status,
32471 type1),32437 HID submissions,34 retries,busy396,no ERROR. Both link stops
neutral complete. V8 motion previously user-confirmed wired-like/no overshoot.
Board v8 EXPIRED/PASSIVE,one arm/two discoveries,53698 ACKs used. COM12 closed,
no live raw/serial/build. User told to reconnect mouse USB. No new firmware changes.
NEXT PHYSICAL PAUSE: v8 held-left timeout regression on harmless selectable
plain text and other pointer. Normal receiver reboot(no double-reset),Glorious
OFF/unplugged. One confirmed arm;hold LEFT over text beyond60s window,while
still held use other pointer without clicks to confirm selection fixed,then
release. Hidden raw/serial capture can verify release event at stop. Do not
increase limits/build/flash/rearm on current used boot. This checks forced
release on unchanged v8 before considering sustained sessions.

V8 FIRST MOTION HARDWARE PASS 2026-11-16: user reports "Overshoot gone /
like wired;stopped cleanly" with capture window active. One successful trial;
not calibrated physical latency/DPI or final gaming-performance validation.
Private base build-pairing-analysis/receiver-v8-foreground-20261116-004654:
100s raw exit0,50748 receiver-only relative foreground records,50746 motion,
2 button records,0 read errors,no background records. Median arrival1001us,
p90 1087us,p99 2926us;whole-run max includes pauses. Max axis6/5 vs v7's31/14,
hand movements not mechanically matched. Serial one arm/discovery,59468 ACKs
(8706 idle,1 status,50761 type1),50757 HID submissions,4 retries,busy895,
no ERROR,neutral complete. Active-link stats/busy TEXT counts both0 as intended.
Busy total retained;895 vs v7's171 messages is not a like-for-like loss metric.
Board v8 NOW EXPIRED/PASSIVE,one arm/discovery used,59468 ACKs consumed.
COM12 closed,no live raw/serial/build. User instructed reconnect Glorious wired.
V8 image SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
no further build/flash/tuning needed before regression check. V7 preserved.
NEXT PHYSICAL PAUSE: v8 controls/recovery regression. User normal receiver
unplug/replug(no double-reset),harmless scrollable document/other pointer,
Glorious OFF/USB unplugged,same DPI/settings. Capture100s foreground+95s serial,
one p/DTR with actual arm confirmed before mouse ON. User tries movement,
left/right-click,wheel up/down,then MOUSE OFF3s/ON after about15s,tries controls
again,and observes clean60s stop. Keep receiver connected. New boot/readiness
must be explicitly confirmed before clearing used capture checkpoint;do not
reuse current arm or flash-validation false state automatically. No flash.
Analyze only foreground input_code0;activation with other pointer. Preserve
successful wired/v7/v8 captures;do not repeat baseline/extraction/build.

V8 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: v8 NOW FLASHED on receiver.
User confirmed NICENANO visible with Glorious wired/OFF. Verified D: NICENANO,
bootloader0.6.0/nRF52840-nicenano/S1406.1.1,and exact v8 UF2 hash
5df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd;
UF2 validation PASS240 blocks0x26000..0x34FFF. Copy completed;COM12 reenumerated
with expected receiver USB ID.6.5s passive DTR capture privately saved as
build-pairing-analysis/receiver-v8-passive-20261116-004355.txt. Verified v8 quiet
banner,timing init,zero packets/CRC/invalid length,no ERROR/arming/link.
Initial check incorrectly demanded ACKs=0 stats,which passive scan does not emit;
saved log/source reviewed and validation corrected without repeat capture/flash.
No p sent since flash,so UNARMED,unused65536 ACK/60s/eight-discovery budgets.
COM12 closed,no live serial/raw/build. Metadata receiver-v8-flash-validation.json.
NEXT PHYSICAL PAUSE: same harmless document/surface/saved DPI/other pointer;
Glorious OFF and USB unplugged;leave receiver connected without reset. After
readiness,paired100s foreground Raw Input/95s serial drain2s,one p/DTR,actual
v8 arming confirmation before mouse ON. User activates capture window using
other pointer,then30s circles/straight strokes,no clicks/wheel/DPI changes.
Capture script must be updated for v8/previously consumed startup banner:
verify flash checkpoint plus passive scans,no preexisting arm/link/errors,then
confirm actual new arm. Do not require a second startup banner or repeat baseline.
Compare input_code0 motion and user overshoot with preserved v7/wired captures.
Hardware benefit of v8 remains untested;all firmware/host software checks passed.

V8 QUIET-ACTIVE SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-quiet-active-window. Receiver v8 BUILT/VALIDATED,NOT FLASHED.
Change only active diagnostics:skip periodic stats while linked,replace per-busy
text with cumulative EAGAIN counter in final summary. Keep v7 idle ACK rule,
motion mapping,ACK timing,first timing samples,link/error prints and all limits.
Regression failed on v7;all8 native tests PASS with v8. Receiver/probe/radio
builds and UF2 checks PASS. Probe/radio hashes unchanged;default HID image hash
unchanged,not rebuilt. Receiver staged source/config/v8 banner verified.
V8 UF2: C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd.
61256 flash/16128 RAM;240 UF2 blocks,0x26000..0x34FFF. Private preserved image
build-pairing-analysis/receiver-v8-5df75330.uf2;metadata receiver-v8-build-validation.json.
V7 rollback receiver-v7-b6a63b99.uf2 hash verified. V8 is an unproven A/B logging
overhead experiment,not an established overshoot fix;less active-time telemetry.
Board STILL v7 expired/passive after foreground trial:one arm/discovery,59129
ACKs used. COM12 closed,no live raw/serial/build. Glorious asked to reconnect wired.
NEXT PHYSICAL PAUSE via popup: keep Glorious wired/OFF,double-reset only RECEIVER
into NICENANO. Verify bootloader identity and exact v8 hash/UF2 before copy,then
passive startup only,no arming. Separate later motion readiness for paired
foreground raw100s/serial95s,one p/DTR,actual arming confirmation BEFORE mouse ON.
Raw logger accepts --foreground;user must activate window with other pointer;
analyze input_code0. No repeat v7 baseline/build/extraction. DPI/Windows unchanged.

FOREGROUND WIRELESS COMPARISON COMPLETE 2026-11-16: user kept window active;
random overshoot persists;clean stop confirmed. Private base
build-pairing-analysis/receiver-v7-foreground-20261116-003012 (csv/txt/host.jsonl/
raw.stdout.txt/summary.json).100s raw exit0,39180 receiver-only relative records,
39076 foreground(39075 motion,1 button),104 background,0 read errors. Foreground
median gap1025us,p90 2054us,p99 4044us;whole-run max273019us includes pauses.
One arm/discovery,59129 ACKs(19317 idle,1 status,39811 type1),39709 HID
submissions,102 retries,171 HID-busy messages,no ERROR;neutral complete.
Wired foreground was user-confirmed smooth. Wireless max axis31/14 vs wired3/2;
median motion magnitude1 in both,p99 wireless2.236 vs wired2. Occasional larger
wireless steps occur near busy serial chunks;host chunks buffered,hand motion
not identical,so correlation is not proof of logging cause or fixed DPI error.
No live raw/serial/build. Board v7 expired/passive,59129 ACKs used. User told
reconnect wired. NEXT SOFTWARE: resume preserved quiet-active-window A/B candidate
(v8),keeping all protocol/timing/mapping/limits. Test actual suppression and busy
counters,build/validate,preserve v7,then pause for physical flash readiness.
Do not repeat foreground/background v7 baselines or guess motion scaling.

FOREGROUND WIRED CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/raw-wired-foreground-20261116-002746.45s exit0,20389
wired-only relative records/errors0.20109 foreground(input_code0;20106 motion,
3 button records),280 background. Foreground median arrival1079us,p90 2947us,
p99 4950us;whole-run max includes movement pauses. Background median8007us.
User confirmed capture window stayed active;user confirmed wired smooth/consistent.
No physical polling/latency/DPI claim. This verifies background cadence confound.
No live logger/serial/build. Firmware remains v7. NEXT pop-up already pending:
user wired observation plus normal receiver reboot/Glorious OFF-unplugged
readiness for matching foreground wireless pair. Do not assume readiness;do not
repeat wired capture. Pair script must pass --foreground and show its window;
user activates with other pointer after confirmed arm then moves Glorious.
One p/DTR,95s serial+100s raw;analyze input_code0 only. Do not reuse old boot.

FOREGROUND RAW INPUT SOFTWARE CHECKPOINT 2026-11-16: branch
feature/foreground-raw-input. User clarified wireless symptom: occasional random
overshoot during circles;stop was clean. Background paired capture completed as
recorded below. Do not infer polling or dropped motion from background cadence.
research/capture_raw_mouse.c now accepts optional final --foreground,shows an
ordinary capture window without forcing focus,and appends input_code to CSV:
0 foreground /1 background from WM_INPUT wParam. Default hidden mode preserved.
Known-device filtering,mouse-only registration,exclusive-create output,1..180s
limit and cleanup unchanged. No keyboard/injection/firmware/settings changes.
Serializer regression failed on old code;new self-check passes both delivery
codes plus prior bounds/filter/signed tests. GNU11 warnings-as-errors build PASS.
Hidden1s lifecycle PASS;visible1s lifecycle verified exit0/records0/errors0 and
DONE marker. Invalid option rejected before creating file. Explicit stdout flush
added after DONE because initial visible smoke omitted buffered completion text.
Final ignored exe SHA256
1e6ed06a5b447461d77d2fa20add18269634bcc8475ea6aeea8a59130c832d16.
Smoke is not movement/performance evidence. No controlled foreground capture yet.
No live capture/serial/build sessions. Board v7 expired/passive after one arm,
one discovery,58479 ACKs;COM12 closed. User told to keep mouse wired.
NEXT PHYSICAL PAUSE: wired readiness,same surface/saved1600 profile/other pointer.
Start45s logger --foreground;after readiness cue user clicks capture window and
keeps it active while doing20-30s circles/straight strokes. Analyze input_code0
only and record activation fraction. This is a new foreground measurement,not
a repeat of background baseline. Later wireless needs reboot/OFF-unplugged
readiness and one actual-confirmed arm. Do not rearm on current used boot.

PAIRED BACKGROUND RAW CAPTURE COMPLETE 2026-11-16: private base
build-pairing-analysis/receiver-v7-raw-20261116-001759. User performed movement
and reports speed still changed or paused;later clarified random overshoot during circles;stop clean.
100s raw logger exit0,7198 receiver-only relative motion records,0 buttons,
0 read errors;median8000us,p99 16936us,max65459us. One serial arm/discovery,
58479 ACKs(5378 idle,1 status,53100 type1),52983 HID submissions,117 retries,
no ERROR. Firmware final neutral complete. Board now v7 expired/passive;COM12
and raw logger closed. No reset/rearm/build/flash. User asked to reconnect wired.
MEASUREMENT LIMITATION: logger uses background INPUTSINK/message-only HWND.
Microsoft documents background raw listeners may be throttled/coalesced:
https://blogs.windows.com/windowsdeveloper/2023/05/26/delivering-delightful-performance-for-more-than-one-billion-users-worldwide/
Thus125 events/s and raw-vs-HID counts cannot establish polling rate or loss.
NEXT SOFTWARE: minimal foreground capture mode and foreground/background event
labels;self-check/build/smoke,then physical foreground wired/wireless comparison.
Retain background captures;do not repeat them or infer DPI scaling.

WIRED RAW INPUT BASELINE COMPLETE 2026-11-16: user performed movement and
confirmed smooth/consistent wired control. Private capture raw-wired-20261116-001333.csv,
.stdout.txt and .summary.json in build-pairing-analysis. 45s run exited0:
3674 wired-only records,0 read errors/failed0;all mouse_flags0(relative),3669
motion records and5 button-event records. X range-73..72,Y-33..26. Median
whole-run arrival gap8004us;steady seconds21..35 each125 records/s. Idle/user
pauses affect whole-run tails;not a physical latency,polling or DPI measurement.
No live logger/serial/build session. Receiver remains v7 expired/passive.
NEXT: physical pop-up for normal receiver reboot(no double-reset),Glorious
OFF/USB unplugged,same surface/profile/document and other pointer. Then paired
raw/serial wireless capture with one p/DTR and confirmed arming before mouse ON.
Do not repeat wired baseline. Keep private captures ignored.

RAW INPUT SOFTWARE CHECKPOINT2026-11-16: branch feature/raw-mouse-input-diagnostics.
User requests ready-to-copy continuation prompt;maintain RESUME_PROMPT.md after
meaningful steps along with this handoff. Last RF/DPI result pushed3f7676e.
New research/capture_raw_mouse.c built to ignored
build-pairing-analysis/capture_raw_mouse.exe SHA256
41a46ce524380cc1f50c3542889729008ff5cc1e556fdd85bc59411021c45a39.
Build: gcc -std=gnu11 -Wall -Wextra -Werror -O2 research/capture_raw_mouse.c
-o build-pairing-analysis/capture_raw_mouse.exe -luser32 -lshell32.
Self-test PASS(known/unknown IDs,bounds/type,signed extremes/flags,no registration).
Timed1s hidden/message-only smoke PASS,0 records/read-errors/failures;exclusive
create test rejects existing file(WinError80) without changing hash. Final smoke
ignored raw-input-smoke-20261116-000158.csv;prior overwrite test235919.
Metadata ignored build-pairing-analysis/raw-input-software-validation.json.
No controlled Raw Input movement capture yet. Logger mouse-only INPUTSINK,no
NOLEGACY/keyboard registration/injection/settings changes;filters wired258A:2018
and receiver2FE3:0008,omits full device paths. UTC start printed to stdout;CSV
QPC elapsed time_us/source/dx/dy/button_flags/button_data/mouse_flags.1..180s
bounded duration;normal stop unregisters/destroys/closes. Save stdout with CSV
to align host timing later. Arrival timing is not physical latency or sensor DPI.
Legacy MinGW needs GNU C extensions;documented Vista constants supplied locally.
Firmware source unchanged,UF2 remains v7 b6a63b99... . Board v7 expired/passive,
one arm/discovery/58365 ACK attempts used;COM12 closed. No live raw/build/serial.
CORE wired Profile1=1600 saved;Windows accel1/speed9 unchanged. Wireless speed
still varies within steady motion;do not guess gain/DPI or claim calibration fixed.
NEXT PHYSICAL PAUSE via pop-up: connect Glorious WIRED USB,same surface/profile,
other pointer available;no receiver reset/arm/flash required. After readiness,
run logger45s to unique private raw-wired-...csv and stdout transcript;after
RAW_INPUT_READY cue ask20-30s circles/straight strokes,no DPI/buttons/wheel.
Inspect source filtering,relative flags,signed deltas and host gaps;get user
speed-consistency observation. This is the first controlled host raw baseline.
Later wireless comparison needs normal receiver reboot plus OFF/unplugged
readiness,paired raw/serial capture,ONE p/DTR and actual arming confirmation
before mouse ON. Do not automatically arm or repeat completed builds/flashes/
baseline/extraction. All private captures/addresses/binaries stay ignored.

LATEST SAVED-DPI RESULT20261115-234324 COMPLETE:95s single-arm capture closed.
Ignored build-pairing-analysis/receiver-v7-dpi1600-retry-20261115-234324.txt,
.host.jsonl,.summary.json. One link,58365 ACKs:9813 idle,one status,48551 type1;
48444 unique HID submissions+107 retries.375 USB-busy events,no ERROR or
rediscovery,neutral-complete final stop in firmware log.73 stats lines,two
completed timing samples. Last good59255/bad10/invalid2 are cumulative counters.
User reports speed CHANGES DURING STEADY MOVEMENT,not a constant faster/slower
offset,despite saved wired Profile1=1600 and unchanged DPI button/Windows/source.
End-stop not separately user-confirmed. Do not claim matched wireless DPI,
constant-gain fix,perfect motion or measured latency. No Windows settings changed.
Board v7 expired/passive,one arm/discovery/58365 ACK attempts used;do NOT p again
this boot. COM12 closed,no live build/serial. User told reconnect cable;unknown.
NEXT SOFTWARE MEASUREMENT: local mouse-only Windows Raw Input logger to compare
wired vs DIY HID motion counts/arrival timing,filtered to known VID/PIDs,no
keyboard capture or input injection/settings changes. Keep v7/1600 unchanged.
Build/self-test/start-stop check before physical wired/receiver measurement
readiness. Raw arrival timing does NOT measure physical-event latency/DPI alone.

SAVED-DPI ATTEMPT20261115-233835 COMPLETE;MISSED CUE/INCONCLUSIVE.95s continuous
drained/single-arm COM12 capture closed/disposed. Ignored
build-pairing-analysis/receiver-v7-dpi1600-20261115-233835.txt,.host.jsonl,.summary.json.
One actual arming confirmed before power-on pop-up,but NO discovery reply/link
start,zero ACKs/idle/status/type1/HID submissions,no ERROR. User explicitly chose
'Could not perform or missed cue'. Some later passive RX counters progressed;
do not infer active input or DPI comparison. Last good1247/bad6/invalid3 are
cumulative since boot,not loss counts for an active link. No performance result.
Board v7 arming opportunity EXPIRED/passive;discovery/ACK budgets unused but
do NOT send p again on this boot. COM12 closed,no live build/serial. Saved CORE
wired Profile1=1600 unchanged;wireless retention/equivalence still unverified.
NEXT PHYSICAL PAUSE: normal receiver unplug/replug(no double-reset),same harmless
document/surface/other pointer,Glorious OFF/USB unplugged,no DPI-button change.
Have hand near power switch and watch next arming pop-up;select Ready only when
able to switch ON promptly within30s. No rebuild/flash/baseline/DPI-save repeat.
Then same95s single-arm comparison;no automatic retry. User can pause until later.

DPI PROFILE CHECKPOINT: user connected Glorious USB;Glorious CORE recognizes
SERIES ONE PRO WIRELESS in wired mode. Official UI shows PROFILE1 with selected
1600 DPI(red outlined slider stage),presets400/800/1600/3200. No separate wired/
wireless DPI control exposed on Performance page. Under user's explicit request
to match wired DPI,clicked SAVE with existing1600 profile;UI refreshed and still
shows selected1600. No DPI preset values,Windows settings,firmware/gain scaling
or other exposed settings manually changed. This official UI save is authorized
mouse configuration;do not confuse with guessed vendor HID/config commands.
Saved profile does NOT independently verify battery-mode DPI retention. Raw
receiver still forwards signed X/Y1:1. CORE left open. Mouse last wired.
Quiet-active candidate remains deferred privately;source/binary stays testedv7,
branch feature/idle-ack-queue-guard. No live build/serial;receiver last v7 expired/
passive,one arm/discovery/59121 ACK attempts used (no subsequent reset confirmed).
NEXT PHYSICAL PAUSE: normal RECEIVER unplug/replug(no double-reset),same harmless
document/surface/other pointer;Glorious OFF/USB unplugged,do not press DPI button.
Then single-arm95s v7 comparison using saved1600 profile,after separate readiness
and actual arming confirmation before power-on. No build/flash/baseline repeat.
Compare comfortable wired1600 control with wireless response;do not claim actual
wireless DPI equivalence from CORE's one profile display alone.

DPI MATCH TASK / PHYSICAL PAUSE: user clarified high DPI may have made circles
hard to control,then explicitly asked to set wireless DPI same as wired if possible.
Do not treat this as proven residual RF variation. Deferred the unflashed quiet-
active candidate:own firmware/test edits restored EXACTLY to tested v7;current
UF2 hashb6a63b99... reverified,no V8 build/flash. Candidate patch/plan saved ignored
at build-pairing-analysis/deferred-quiet-active-window.patch and
deferred-quiet-active-window-plan.md. Empty unpublished candidate branch removed;
active branch again feature/idle-ack-queue-guard. No user files changed.
Using computer-use skill/sky to inspect installed Glorious CORE. Launched existing
C:/Program Files (x86)/Glorious CORE/Glorious Core.exe. Current UI says PLEASE
CONNECT YOUR GLORIOUS PRODUCTS;no DPI value available and NO setting changed.
NEXT physical pause via pop-up: user connects Glorious USB cable directly to PC,
leaves DIY receiver unchanged. Then refresh CORE UI,read wired active DPI/profile
and inspect whether separate wireless DPI exists. Match through supported official
UI if exposed;no guessed HID/config writes or receiver scaling. Windows speed/
acceleration unchanged. Hardware still v7 expired/passive,COM12 closed,no serial.
Mouse wired connection needs confirmation;continue UI task after that readiness.

NEWEST V7 COMPARISON20261115-231733 COMPLETE:95s drained/single-arm capture
closed. Ignored build-pairing-analysis/receiver-v7-motion-20261115-231733.txt,
.host.jsonl,.summary.json. One link,59121/65536 ACKs:11305 idle,one status,
47815 type1;47750 unique HID submissions+65 retries.191 USB-busy events,no
ERROR or rediscovery;final neutral-complete stop in firmware log. Two completed
timing samples. First stats good2534/bad18/invalid6,last61999/22/7 are cumulative
since boot,not capture totals. User says 'definitely ... more consistent now'
than v6,but not absolutely consistent. End-stop not separately user-confirmed.
Subjective benefit supports retaining v7 for next comparison;queue-race cause/
perfect delivery/latency still unproven. Board v7 expired/passive,one arm/
discovery/59121 ACK attempts used;do NOT p again this boot. COM12 closed,no
live build/serial. User told release buttons/reconnect cable;reconnection unknown.
NEXT SOFTWARE comparison: retain v7 ACK rule;remove periodic stats and USB-busy
text from ACTIVE RF loop. Keep counters/final link summaries and limited
discovery/first-ACK timing samples;busy input gets a cumulative counter instead.
Do not change RF bytes/timing,bounds,USB mapping or Windows preferences. Test/
build/push then physical flash pause. Ask optional residual symptom/stop detail.

NEWEST HARDWARE CHECKPOINT2026-11-15 about23:12 Manila: v7 FLASHED and5s
passive startup PASSED. User confirmed NICENANO/mouse wired or OFF;reverified
bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and b6a63b99... finalUF2 hash/240
blocks/0x26000..0x34FFF application bounds before copy. Windows composite/CDC
COM12/HID/Input statuses all OK. Passive log:v7 idle-sequence-hold banner,
8 discoveries/65536 ACKs/60000ms/1000ms gap,1MHz timing,19channels,six zero
scans,no ERROR or arming. Ignored evidence
build-pairing-analysis/receiver-v7-passive-20261115-231231.txt and .summary.json.
Board v7 fresh/passive/UNARMED,all budgets unused;NO p sent. COM12 closed/
disposed,no live build/serial. Software checkpoint2cb2c5c pushed on
feature/idle-ack-queue-guard. Queue-race hypothesis and v7 motion quality unproven.
NEXT PHYSICAL PAUSE: same harmless document/surface/DPI/unchanged Windows
settings,other pointer,Glorious OFF/USB unplugged readiness via pop-up. Receiver
stays connected/no reset. Do NOT repeat flash/passive baseline/build. Same
continuous2s-drained95s single-arm capture;confirm NEW arming before mouse
power-on pop-up. Move steady circles30s,no clicks/wheel/OFF cycle;compare speed
consistency with smooth wired and variable v6. Inspect idle/type1/HID/retry/busy
counters and user response;do not claim race/fix based on totals alone. If worse,
preserved v6 c0e18e29... remains available for verified restoration.

V7 SOFTWARE EXPERIMENT READY;PAUSED FOR PHYSICAL FLASH. Branch
feature/idle-ack-queue-guard;plan
docs/plans/2026-11-15-idle-ack-queue-guard.md. Wired/static finding pushed50fff9a.
Tested v6 preserved and hash-verified at ignored build-pairing-analysis/receiver-v6-c0e18e29.uf2.
Receiver-only length2 idle ACK now echoes its sequence;data/status ACK stays+1.
Second ACK byte,timing,identity/CRC/packet filters,65536/60s/eight-discovery
bounds,USB mapping/recovery unchanged. Non-HID probe keeps idle+1 as control.
Actual native receiver assertion failed before code;all8 tests now PASS,
including repeated idle,255/0 idle followed by same-sequence data,and probe-mode
control. Inferred queue-release risk is NOT a proven cause/fix yet.
Receiver/probe/radio builds/UF2 bounds and mode/binary/staged-source checks PASS.
Probe/radio/default HID hashes EXACTLY unchanged from v6 software checkpoint.
V7 receiver UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2:
SHA256b6a63b99667a764ed32efb2a3f46bf0721c99f43ecbd9131f81ea08370fde16a,
flash61220/RAM16128,240 blocks,payload0x26000..0x34FFF. V7 NOT FLASHED.
Ignored metadata build-pairing-analysis/receiver-v7-software-validation.json.
Final native tests also prove status/compound ACK wrap still+1. No source/API/
config/USB descriptor changes beyond receiver idle ACK and banner.
Hardware still v6 expired/passive,one arm/discovery/58171 ACK attempts used;
COM12 closed,no live build/serial. No v7 input/fix claim. No Windows settings changed.
NEXT PHYSICAL PAUSE via pop-up: mouse wired or OFF,user double-resets receiver
into NICENANO. Verify bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and finalv7
hash/bounds,application copy,5s passive startup WITHOUT p. Then separate
harmless-document/other-pointer/Glorious OFF-unplugged readiness. Same continuous
2s-drained95s single-arm capture;verify NEW arming before power-on pop-up.
Compare30s steady circles/speed consistency to smooth wired and variable v6;
keep Windows settings/DPI/surface unchanged. Inspect input/idle/HID/retry/busy
counters plus user observation. If behavior worsens,preserved v6 permits recovery.

LATEST WIRED COMPARISON: user confirms same circles/surface/settings are smooth
and consistent WIRED;wireless feels different. Windows acceleration alone is
therefore not a sufficient explanation. No Windows settings changed.
READ-ONLY STATIC FOLLOW-UP: existing ignored mouse-disassembly.txt shows
0x1890A..0x18950 snapshots queue indices and can send length2 when empty;
0x16088..0x160B8 handles ACK sequence+1 by rereading CURRENT queue indices,
popping a queued report if no longer empty,and advancing outgoing sequence.
Inference: a new report queued while idle is in flight could be released by our
idle sequence+1 ACK without ever being transmitted. Race not proven on hardware.
NEXT SOFTWARE experiment: receiver-only idle ACK keeps received sequence;data/
status ACKs retain+1. Preserve tested v6,add actual ACK-byte/wrap/same-sequence
tests,build/validate all variants,then physical flash/readiness pause. Keep
probe's prior behavior and all RF timing/USB/budgets unchanged. No speculative
gain curves or Windows preference changes. Board still v6 expired/passive,
one arm/discovery/58171 ACK attempts used;COM12 closed,no live session.

NEWEST FOCUSED MOTION RESULT20261115-225106:95s unchanged-v6 single-arm capture
completed/closed. Ignored build-pairing-analysis/receiver-v6-motion-20261115-225106.txt,
.host.jsonl,.summary.json. One continuous link,58171/65536 ACKs:4769 idle,one
status,53401 type1.53314 unique HID submissions+87 retries match type1 ACKs.
387 USB-busy events withheld ACK;no ERROR,no rediscovery;normal neutral-complete
final stop.71 stats lines,last cumulative good59017/bad_crc6/invalid_len1.
Two completed timing samples,not total TX count. User reports NO hard freezes
noticed in this focused run,BUT varying sensitivity/slower pointer than hand
motion during circles. Previous mixed-input intermittent-stop observation is
not erased;cause remains unknown.1s bins cannot diagnose subsecond motion loss
or Windows pointer behavior;no motion-payload/nonzero-delta telemetry exists.
Read-only Windows SPI_GETMOUSE/SPI_GETMOUSESPEED:acceleration1(enabled),speed9/20,
thresholds6/10. Registry corroborates. No settings changed. Saved ignored
build-pairing-analysis/windows-pointer-settings-20261115.json. Microsoft reference:
https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-systemparametersinfow
Acceleration could affect perceived sensitivity;NOT established as cause.
Board v6 expired/passive,one arm/discovery/58171 ACK attempts used;do NOT p again
this boot. COM12 closed,no live build/serial. Mouse cable reconnection unconfirmed.
NEXT PHYSICAL PAUSE via pop-up: plug Glorious USB in and try15-20s same circles
on same surface/settings in WIRED mode. Receiver stays connected/expired,
no reset/arming/flash or setting changes. Ask whether speed variation also occurs
wired or wired stays consistently smooth. Use that result before firmware fixes.

NEWEST V6 RF RESULT20261115-224452:95s drained/single-arm capture completed,
COM12 closed/disposed. Ignored build-pairing-analysis/receiver-v6-input-20261115-224452.txt,
.host.jsonl,.summary.json. One arm,two links,55417/65536 ACK attempts;40808 idle,
one status,14608 type1 ACKs.14557 unique HID input submissions+51 suppressed
retries exactly account for type1 ACKs. Four completed TIMING samples;no ERROR,
95 USB-busy lines (ACK withheld,not fatal). Link1 stops neutral at22573 ACKs;
link2 starts samecount with33244ms remaining;final neutral stop at55417 ACKs.
No unplanned extra rediscoveries observed.62 stats lines,2 cumulative CRC failures,
zero invalid lengths at last stats. About999 ACK/s median over1s intervals
includes idle;max observed HID submission rate864/s over1s interval. These are
counter rates,NOT measured USB polling or physical-event latency/packet loss.
User CONFIRMED controls before/after mouse OFF3s/ON and clean stop,no stuck
buttons. Subjectively noticeably more responsive/no obvious v4-style delay.
BUT intermittent pointer stops despite continued motion,then resumption remain
an OPEN defect. USB busy may contribute;cause not established by aggregate stats.
Board v6 expired/passive,one arm/two discoveries/55417 ACK attempts used;do NOT p
again this boot. No live build/serial. User told release/reconnect cable;
reconnection unconfirmed. V6 is hardware-tested with intermittent-stall limitation.
NEXT PHYSICAL PAUSE: focused continuous-motion trial on SAME v6,no code guesses.
Ask normal receiver unplug/replug (no double-reset),harmless document/other
pointer,Glorious OFF/unplugged,then ready via pop-up. No flash/build/baseline.
Same continuous2s-drained95s single-arm capture;after confirmed arm/power-on,
keep moving steadily for about30s,no buttons/wheel/OFF cycle. Note approximate
freeze times if any;then let session end. Compare per-second HID/type1 progress,
busy bursts and link transitions against observed stalls before choosing a fix.

NEWEST HARDWARE CHECKPOINT2026-11-15 about22:41 Asia/Manila: v6 FLASHED and
passive startup PASSED after user NICENANO/mouse wired-OFF readiness. Verified
bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and finalc0e18e29... UF2 hash/240
blocks/application bounds0x26000..0x34FFF before application copy. COM12 returned;
Windows composite/CDC/HID/Input statuses all OK. Five-second DTR startup shows
v6 input-counters banner,8 discoveries/65536 ACKs/60000ms/1000ms gap,1MHz timing,
19channels,six zero-counter SCAN lines,no ERROR or arming. Evidence ignored
build-pairing-analysis/receiver-v6-passive-20261115-224120.txt and .summary.json.
Board v6 fresh/passive/UNARMED;all budgets unused. NO p sent. COM12 closed/
disposed,no live build/serial session. Branch feature/receiver-input-diagnostics.
NEXT PHYSICAL PAUSE: harmless scrollable document/other pointer/Glorious OFF
and USB unplugged readiness via pop-up. Leave receiver connected/no reset;
do NOT repeat flash/passive baseline/build. Then same continuous reader saves/
drains2s,ONE p,confirm NEW 'RECEIVER armed:' with65536 ACK cap before power-on
pop-up;capture95s. Try deliberate movement,buttons,wheel;mouse OFF3s/ON after
about15s,try controls again,observe final stop. Inspect successful idle/status/
type1 ACKs vs hid_reports/retries and user response/delay. Counts of queued
USB input do not prove Windows delivery. No v6 input success or latency claim.
Earlier break/departure and v5 state below are historical;user resumed.

RESUMED: user returned and explicitly requested continuing where we left off.
Previous departure/break pause is lifted. Software milestone501d183 is pushed
on feature/receiver-input-diagnostics. V6 image hash/bounds rechecked;no rebuild.
Board last known v5 expired/passive;COM12 closed. NEXT PHYSICAL PAUSE: user
confirms NICENANO bootloader visible with Glorious wired or OFF. Then reverify
bootloader/finalv6 UF2,flash application and5s passive startup WITHOUT p.
Separate input readiness is still required before any v6 arm. No completed
extraction/build/v4 recovery trial repeated. Keep per-step handoff current.
EARLIER RECEIVER CHECKPOINT (v5/software v6; superseded by hardware note above):
FINAL SOFTWARE-ONLY STEP COMPLETE;PAUSED FOR BREAK per user's latest request.
V5 inconclusive result pushed8d48e7e. New branch feature/receiver-input-diagnostics;
plan docs/plans/2026-11-15-receiver-input-diagnostics.md. V5 artifact preserved
and hash-verified at ignored build-pairing-analysis/receiver-v5-ab8e3001.uf2.
V6 source adds cumulative successful ACK type counts(idle/status/type1) and
unique HID submission/suppressed-retry counts to1s and link-stop summaries.
Finite shared ACK cap65536;60s deadline/eight discoveries/1s gap/manual arm and
RF bytes/timing/USB mapping unchanged. A queued report count is NOT proof of
Windows delivery. New native assertions failed before code,then all8 PASS.
Receiver/probe/radio builds/UF2 bounds and mode/binary/staged-source separation
checks PASS. Default HID unchanged058c1ff5... . No binaries/private header tracked.
V6 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2:
SHA256c0e18e29413dd720c70f3779f25edaee9513253235a3bfdf0e5f8ad1f38bfcef,
flash61200/RAM16128,240 blocks,payload0x26000..0x34FFF. V6 NOT FLASHED.
Probe v9 hashc3fd1efa911fde6d323fb153bbcba51b9d91135a97f97f2a047dbecc766429b6;
radio4c53da0a8d462b36757dcada52d50386d73a756498b4f9a3eec3ef497c9fb172.
Ignored metadata build-pairing-analysis/receiver-v6-software-validation.json.
Board still v5 expired/passive (one arm/discovery,8192 ACK attempts used);
COM12 closed,no live build/serial. NO further hardware flash/test before break.
RESUME AFTER USER RETURNS: ask NICENANO bootloader readiness with mouse wired/
OFF via pop-up. Reverify bootloader0.6.0/nicenano/S1406.1.1 and finalv6 hash/bounds,
flash application,5s passive check. Then separate harmless-document/other-pointer/
Glorious OFF-unplugged readiness. Same2s-drained continuous95s single-arm capture,
confirm NEW RECEIVER armed text with65536 cap before power-on pop-up. Inspect
idle/status/type1 ACK counts vs hid_reports/retries and user controls/delay.
Stats counters count successful RF ACKs and accepted unique USB submissions;
they are cumulative across links. Submissions do NOT prove Windows delivery;
neutral reports do not count as new mouse input. No v6 hardware or latency claim.

V5 COMPARISON COMPLETE20261115-142958:95s single-arm/drained capture closed.
Ignored build-pairing-analysis/receiver-v5-comparison-20261115-142958.txt,
.host.jsonl,.summary.json. One link,8192/8192 ACKs,neutral-complete budget stop,
49 stats lines,no ERROR/USB busy. Counter rose ACK1->8192 across9719ms between
observed stats samples (not exact active duration). Two completed TIMING samples.
User selected missed/could-not-perform cue and reports NO movement/buttons or
usable control. Latency/recovery/control comparison INCONCLUSIVE,not passed.
Quiet counters do not distinguish idle/status/type1 or HID report submission.
Budget exhaustion could have prevented deliberate input;cause of no response
NOT established. No claim quieter logging fixed delay. Board v5 expired/passive,
one arm/one discovery/8192 ACK attempts used;do NOT p again this boot.
COM12 closed;no live session. User told release buttons/reconnect cable.
LATEST USER STEERING after result: 'proceed with the next step and then ... break'.
Authorized ONE software-only step: ACK-type/HID-submit/retry counters and finite
65536 ACK cap for60s trial at observed higher ACK cadence. Keep RF timing/bytes,
recovery/USB semantics unchanged. Test/build/push,then PAUSE BEFORE flashing.
No further hardware trial before break. Exact next steps after software checkpoint.

PAUSED UNTIL USER RETURNS for requested14:30 departure. At about14:25 Manila,
user confirmed NICENANO visible/mouse wired or OFF. Reverified bootloader0.6.0,
nRF52840-nicenano,S1406.1.1 and finalv5 ab8e3001... UF2 hash/238 blocks/application
bounds0x26000..0x34DFF;copied application only. V5 IS NOW FLASHED.
Five-second COM12 DTR passive startup PASSED:v5 quiet banner,8 discoveries/8192
ACKs/60000ms/1000ms gap,1MHz timing,19channels,six zero SCAN lines,no errors or
arming. Windows composite/CDC/HID/Input statuses all OK. Ignored evidence:
build-pairing-analysis/receiver-v5-passive-20261115-142507.txt and .summary.json.
Board v5 fresh/passive/UNARMED;all budgets unused. NO p sent on v5. COM12
closed/disposed;no live build or serial session. No armed test before departure.
Resume ONLY after user returns/asks to resume: separate harmless scrollable
document/other-pointer/Glorious OFF-unplugged readiness via question pop-up.
Do not repeat flash,passive baseline,build or v4 extraction/recovery trials.
Then continuous reader saves/drains2s,ONE p,confirm NEW RECEIVER armed text
before power-on pop-up,95s capture. Compare perceived response to delayed v4;
try movement/clicks/wheel and mouse OFF3s/ON recovery within60s session,then stop.
Analyze cumulative stats/link-stop shared ACK totals;no raw packets/per-ACK
logs in v5. TIMING lines only sample discovery/first ACK per link,NOT total TXs.
V5 latency improvement NOT tested/claimed. Tested v4 remains preserved2162a852...
Software milestone645822d pushed on feature/receiver-quiet-diagnostics.

QUIET-DIAGNOSTICS SOFTWARE READY;PAUSED FOR PHYSICAL WORK. V4 recovery milestone
pushed78315c9. Branch
feature/receiver-quiet-diagnostics;plan docs/plans/2026-11-15-receiver-quiet-diagnostics.md.
Tested v4 preserved/verified at ignored build-pairing-analysis/receiver-v4-2162a852.uf2.
V5 removes receiver RX/ACK per-packet text,uses1s cumulative stats,and samples
discovery/first-ACK timing per link. RF/USB mapping/recovery/budgets unchanged.
New native assertions failed before implementation;all8 tests now PASS.
V5 receiver build/UF2 PASS:flash60904/RAM16128,238 blocks,0x26000..0x34DFF,
SHA256ab8e3001d0a075a2e2480379894f868e9987000a3752965e41bc93984c6dbf2d.
Probe/radio regression builds and UF2 checks PASS. Binary/config/source separation
PASS:receiver has no raw RX/per-ACK format strings,probe/radio keep intended
verbose paths. Default HID unchanged058c1ff5... . Ignored validation metadata
build-pairing-analysis/receiver-v5-software-validation.json.
Current probe UF2 hash2676573f60010419047f1db5958e4a0bcc369a964a633c20c963f28a397a3be1;
radio a7e6609c1b209c0a36d56eb3db331f43f4c172bfccd84657b2e6511c263378a0.
Board still tested v4 expired/passive (2 discoveries/3348 ACKs used);COM12
closed,no live builds/serial. V5 NOT flashed;no latency improvement claim.
NEXT after user readiness/return: NICENANO bootloader entry with mouse wired/OFF,
verify identity/finalv5 hash/bounds,application copy,5s passive startup;then
separate harmless-document/other-pointer/Glorious OFF-unplugged readiness.
One continuously drained95s capture/single p with actual arming confirmation
before mouse power-on. Compare responsiveness with v4;analyze cumulative
RECEIVER stats/link-stop ACK totals. Timing lines are SAMPLES,not all TXs.
No raw RX/per-ACK output in v5,so do not reuse v4 packet-count analyzer.
Pause by14:30 Manila;do NOT start a new armed95s trial after14:26 today.
NEWEST RF MILESTONE: v4 power-cycle recovery PASSED,with visible pointer delay
reported by user.95s continuous/drained single-arm COM12 capture completed/closed.
Ignored build-pairing-analysis/receiver-v4-recovery-20261115-140838.txt,
.host.jsonl and .summary.json. One arm,two links,3348 ACKs,2310 unique type1,
28 duplicate type1 ACKs,3350 complete TXs,no ERROR/USB busy. Link1 used1586 ACKs;
after neutral-complete gap stop,link2 starts ACKcount1586 with28632ms remaining,
uses1762 more ACKs,then neutral-complete final stop. Budgets/deadline not refilled.
User CONFIRMED movement/clicks/wheel worked before/after ONLY mouse OFF3s/ON,
receiver untouched,and stopped with no stuck buttons. BUT user noticed delay
between mouse movement and pointer. Record as open limitation,not gaming-ready.
Both links median unique-report gap16ms;max360ms (includes mixed/idle activity).
Per-link observed unique-report cadence about44Hz includes user pauses;not a
polling-rate or latency measurement. TX handler/READY/END ranges20..487/
151..618/196..663us include discovery. Logging/polling contribution is hypothesis.
Board v4 expired/passive,one arm used,2 discoveries/3348 ACK attempts used;
do NOT p again this boot. COM12 closed,no live build/serial. User told reconnect
mouse cable;physical reconnection unconfirmed. Cutoff14:30 Manila still applies.
NEXT software: preserve tested v4;new feature branch for quieter receiver
diagnostics comparison. Replace per-packet receiver text with periodic counters,
retain limited discovery/first-ACK timing trace and link logs,leave non-HID probe
verbose and RF/USB mapping unchanged. Test/build/check before physical pause.

NEWEST HARDWARE CHECKPOINT (supersedes flash-pending notes below):v4 FLASHED
on2026-11-15 about14:04 Asia/Manila. Verified D:/NICENANO bootloader0.6.0,
nRF52840-nicenano,S1406.1.1 and final2162a852... UF2 hash/application bounds,
then copied application only. COM12 returned;Windows composite/CDC/HID/Input
statuses all OK. Five-second DTR passive startup PASSED:v4 banner,8 discoveries/
8192 ACKs/60000ms/1000ms gap,1MHz timing,19channels,six zero-counter SCAN lines,
no ERROR,no arming. Evidence ignored build-pairing-analysis/receiver-v4-passive-20261115-140431.txt
and .summary.json. Board v4 fresh/passive/UNARMED,all budgets unused;COM12
closed/disposed,no live build/serial. Mouse last wired or OFF by user readiness.
NEXT: separate harmless scrollable-document/other-pointer/mouse OFF-and-unplugged
readiness via pop-up. Do NOT reboot/flash/repeat baseline. Same continuous reader
drains/saves2s,send ONE p,confirm new 'RECEIVER armed:' before power-on pop-up,
capture95s. User tries controls15s,mouse OFF3s then ON without receiver reset,
continues controls and checks stop. Inspect per-link/shared-budget/neutral timing.
Pause by14:30 Manila as requested;reminder active. No v4 RF recovery tested yet.

LIVE FLASH STEP: user confirmed NICENANO visible/mouse wired or OFF. On2026-11-15
14:03 Asia/Manila,D:/ bootloader verified0.6.0/nRF52840-nicenano/S1406.1.1;
v4 UF2 revalidated239 blocks/0x26000..0x34EFF and hash2162a852... .
Next authorized action: application UF2 copy,then identify CDC and5s passive
startup check WITHOUT p. No v4 arm sent;update outcome before next action.
Software milestone cddf342 committed/pushed with upstream tracking on
feature/bounded-link-recovery. PAUSED for physical bootloader readiness via
question pop-up;no live build/serial session and board still v3.
Branch feature/bounded-link-recovery;plan docs/plans/2026-11-15-bounded-link-recovery.md.
Receiver-only60s session starts at first successful discovery;8192 total ACK
attempts,eight total discovery attempts.1000ms since successful ACK triggers
radio disable and neutral release before discovery scanning. Deadlines/budgets
never refill;initial arm30s/once per boot. New successful discovery resets report
retry marker. Idle/status refresh link only after successful ACK;USB busy does
not. USB reset/disconnect/error remains fatal/reboot-required. One-second gap
is an experimental threshold,not measured stock behavior. No guessed hopping.
New tests failed before implementation;all8 tests now PASS,including both modes,
gap boundary,failed neutral preventing recovery,shared budgets/deadline,discovery
TX failure/exhaustion,ACK exhaustion and new-link same-sequence USB delivery.
GCC tests require elevated execution due sandbox DLL relocation restriction.
Receiver/probe/radio builds and UF2 bounds PASS. Binary/config/staged-source
separation PASS. Default HID artifact unchanged. Validation metadata ignored at
build-pairing-analysis/receiver-v4-software-validation.json.
Current receiver UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2:
SHA2562162a852cb11c8a6c5b26630cdcaa1456390cdb56223dc2c447c38b727362ff3;
flash60968/RAM16064 bytes,239 blocks,payload0x26000..0x34EFF.
Probe v9 (same256/2s policy):SHA256
16cb6a37d503d09846616a176ad0af6817ca5e38a735842650ffe4f9068072c4;
flash58840/RAM15936,230 blocks,payload0x26000..0x345FF.
Radio receive-only:SHA256466aa22444354b85d1749e312ec0b40e59a4dc17fcfe45a63f8fad4902695a47;
flash56760/RAM15872,222 blocks,payload0x26000..0x33DFF.
Default HID unchanged058c1ff5...;hardware-tested v3 preserved/verified at ignored
build-pairing-analysis/receiver-v3-4bc43f35.uf2,hash4bc43f3543638f337eb7d50d42621cc20be321e79ebdefed2ba8d94d9b73d4da.
Board remains v3 expired/passive;COM12 closed,no live build/serial session.
NEXT PHYSICAL PAUSE via pop-up: user puts receiver into NICENANO via double-reset,
mouse wired or OFF. After readiness,reverify bootloader identity0.6.0/nRF52840-
nicenano/S1406.1.1 and v4 UF2 bounds/hash,copy only application UF2,identify CDC
port and verify5s passive v4/HID startup (no p). Then separate harmless-document/
other-pointer/mouse-OFF-unplugged readiness. Same continuous2s-drained reader,
ONE p,confirm NEW 'RECEIVER armed:' before power-on pop-up,and capture95seconds
(30s discovery+60s session+5s cleanup). Try controls,then mouse OFF3s/ON during
session to check recovery;do not reboot receiver/rearm. Inspect link started/
stopped logs,shared budgets/deadline,neutral errors and user controls/stop result.
Reset analyzer dedup marker on each new link;sequence restart is expected.
No v4 hardware/recovery/performance claim.2:30PM pause reminder is active.

NEWEST MILESTONE (2026-11-15): forced LEFT release at10s expiry PASSED.
Fresh normal-reboot/mouse-OFF readiness;continuous COM12/DTR reader drained2s,
sent ONE p,verified new arming before power-on/hold-left pop-up,captured45s.
Ignored evidence: build-pairing-analysis/receiver-v3-held-20261115-015320.txt,
.host.jsonl,and .summary.json.820 complete post-arm RX:180 discovery,136 idle,
504 length11.617 ACKs over9983ms:136 idle,one status,480 type1 (478 unique,
two retries).428 unique ACKed reports LEFT held,50 released. Final ACKed type1
seq222 at81531ms has buttons1;later unACKed seq223 at81796ms also buttons1.
No later type1 release in captured log. All618 TXs complete,no ERROR/USB busy.
ACK handler/READY/END ranges12..308/143..439/187..483us;not latency benchmarks.
User CONFIRMED: after holding LEFT about15s,beyond window,moving OTHER pointer
did NOT extend selection while Glorious LEFT remained physically held. Together
with last ACKed held state and no release error,this validates forced release
at normal expiry. No claim for USB disconnect/reset/error-path physical tests.
Bounded HID validation requested for this stage is COMPLETE: movement,left/right
clicks,wheel UP/DOWN,normal stop/release,and held-LEFT timeout release.
Board v3 expired/passive,one arm/discovery used,617 ACK attempts used;do NOT p
again on this boot. COM12 closed/disposed,no live serial/build session. User told
release LEFT/reconnect mouse cable;reconnection itself not confirmed.
NEXT unresolved development milestone: sustained operation/link recovery and
remaining controls/performance,requiring its own scoped plan/feature branch.
Do not repeat completed controls,extraction,build/flash or baseline to resume.
Further physical trials require normal receiver reboot and separate readiness
via pop-up;pause whenever intervention is needed. No firmware edits this session.

NEWEST MILESTONE (2026-11-15): v3 clicks/wheel/normal-release validation PASSED.
User confirms ALL left/right clicks and wheel UP/DOWN worked in Windows,then
stopped with no stuck buttons. Prior v3 movement stopped at limit without unplug
also explicitly confirmed. Continuous COM12/DTR reader drained2s before ONE p,
verified new arming before pop-up power-on,then captured45s;port closed/disposed.
Ignored evidence: build-pairing-analysis/receiver-v3-controls-20261115-014826.txt,
.host.jsonl,and .summary.json.1031 complete post-arm RX:393 discovery,229 idle,
409 length11.611 ACKs over9980ms:225 idle,one status,385 type1 ACKs representing
383 unique reports and2 retries. Button transitions0->1->0 (three clicks) and
0->2->0;wheel fifteen -1 and eleven +1 reports. Last unique report buttons0.
All612 TXs complete,no ERROR/USB busy. ACK handler/READY/END ranges12..461/
143..592/187..637us after RX END;these are instrumentation,not latency benchmarks.
Board v3 expired/passive,one arm/discovery used,611 ACK attempts used;do NOT p
again this boot. No live serial/build session. User told stop/reconnect cable.
Current feature/wireless-hid-prototype remains appropriate;user authorizes
milestone commits/pushes and new feature-based branches when needed.
Still NOT tested: forced neutral when a button is held at expiry,other three
buttons,sustained/reconnect/channel recovery,latency/throughput/gaming1000Hz.
PAUSED next for physical readiness via pop-up: harmless plain-text document
with selectable text (no links/actions),other pointer,normal receiver reboot,
Glorious OFF/USB unplugged. Then same continuous/drained45s single-arm capture.
After actual arming confirmation,power-on cue: hold LEFT while selecting text,
keep held about15s (beyond10s window),then move OTHER pointer without buttons
while Glorious left remains held. Selection should no longer extend if forced
release reached Windows;finally release Glorious. Inspect final ACKed buttons1,
no release errors,and user observation before claiming forced neutral passed.
No firmware changes,build/flash/baseline/extraction. Private evidence ignored.

NEWEST controls-trial result (2026-11-15): fresh receiver reboot/mouse-OFF
readiness;COM12 DTR continuous reader drained2seconds,sent ONE p,confirmed NEW
arming text,then power-on pop-up.45seconds after arm completed;COM12 closed.
Ignored evidence: build-pairing-analysis/receiver-v3-drained-20261115-014330.txt,
.host.jsonl (host UTC/elapsed/phase per chunk),and .summary.json.
1049 complete post-arm RX:403 discovery,35 idle,611 length11.622 ACKs across
9974ms:35 idle,one status,586 unique type1 reports. All623 TXs completed,no ERROR
or USB busy,no duplicate type1 ACKs. All586 reports buttons0/wheel0;X-56..43,
Y-32..23. ACK handler/READY/END ranges13..115/144..246/188..291us after RX END.
User CONFIRMED pointer moved but could not try buttons/wheel. Clicks/wheel remain
untested; user subsequently CONFIRMED movement stopped at the limit without
unplugging receiver. No stuck-button release claim from
all-zero button reports. This establishes v3's bounded movement,not gaming use.
Board v3 expired/passive,one arm/discovery used,622 ACK attempts used;do NOT p
again on this boot. No live serial/build session. User told to stop/reconnect.
PAUSED next via pop-up: ask whether motion stopped at limit;request normal
receiver reboot,mouse OFF/unplugged,harmless scrollable page and other pointer.
Then same drained continuous-reader45s capture;after confirmed arm tell user
to prioritize left-click,right-click/dismiss,and wheel up/down immediately,
then release buttons and observe stop. No need to spend window on movement.
No rebuild/flash/baseline/extraction. Private raw/host logs remain ignored.

NEWEST RF diagnostic result (2026-11-15): user prepared mouse ON/unplugged/near
unchanged receiver.15-second COM12/DTR read,NO serial commands,completed/closed.
Ignored receiver-v3-rf-silence-20261115-013920.txt and .summary.json under
build-pairing-analysis.456 complete RX,ALL exact known mouse discovery payload;
204 meet exact3/49/78/mouse-base/match1 discovery guards.22 SCAN,no ERROR,no TX.
IMPORTANT: RX device timestamps94389..270012 span175623ms within15s host capture;
largest gap161223ms. Serial contains queued historical output. First scan0 and
last cumulative good4556/bad44/invalid8 are NOT capture totals.457 RX markers,
456 complete parsed lines. Earlier zero-counter captures do NOT establish RF
silence at the time of host capture; their timing interpretation is inconclusive.
Mouse discovery reception is established; no firmware change justified yet.
Board v3 remains expired/passive; no new p,reset,flash/build. COM12 closed.
Installed Zephyr legacy CDC poll_out uses TX ring buffer; when full it blocks
with flow control or drops bytes without it. Driver/source inspection supports
buffering limits,not precise attribution of each old line. No firmware edits.
PAUSED next: normal receiver USB unplug/replug (no double-reset),mouse OFF/USB
unplugged,harmless scrollable document/other pointer,then ready through pop-up.
Open COM12/DTR and continuously drain/save pre-arm output before ONE p; verify
new arming text,then power-on cue via pop-up. Maintain same reader throughout
45seconds after arm. Do not open/close between drain/arm/capture. Record host
timing alongside phases to distinguish stale output. No rebuild/flash/baseline.

NEWEST checkpoint: retry capture20261115-013553 completed after user confirmed
fresh readiness. Exactly one p,actual arming text confirmed,then power-on/controls
cue sent through requested question pop-up.45seconds,COM12 closed/disposed.
Ignored evidence: build-pairing-analysis/receiver-v3-armed-20261115-013553.txt
and .summary.json. Again zero RX/discovery replies/ACKs; no controls validated.
User CONFIRMED powered on promptly with USB unplugged; NOTHING responded.
Zero RF is now an observed blocker despite prompt power-on; cause unknown.
V3 arming expired/passive; do not send p again this boot. No live build/serial.
User told to stop/reconnect mouse cable. PAUSED for physical RF-silence diagnostic:
ask user to leave receiver connected/unchanged (no reset), unplug mouse USB,
switch mouse OFF then ON near receiver,leave it ON,and reply ready via pop-up.
Then capture15seconds on COM12/DTR WITHOUT p to inspect discovery reception.
This is a targeted no-RF diagnostic,not an armed retry or startup baseline.
No HID output expected with expired arming. Do not rebuild/flash or send p.
Use pop-ups for required physical actions; document and push result afterward.
This newest checkpoint supersedes the earlier readiness/reboot request below.

Live continuation checkpoint (2026-11-15, Asia/Manila; supersedes older board/
next-step descriptions below): user confirmed harmless-document/other-pointer/
mouse-OFF-and-unplugged readiness and requests context updates after each step.
Initial sandbox COM12 open failed before sending anything. Authorized elevated
device access then succeeded: DTR enabled, exactly ONE `p` sent, actual v3
arming text verified before user power-on/input cue.45-second capture completed:
ignored build-pairing-analysis/receiver-v3-armed-20261115-013158.txt and .summary.json.
One arming,81 SCAN lines,zero RX/CRC failures/invalid lengths,zero discovery
responses,zero ACKs,zero errors/USB busy. No input reached the HID bridge.
Do NOT interpret this as a click/wheel or neutral-release test result.
Board v3 arming window EXPIRED/passive; discovery/ACK budgets remain unused,
but the one-arm opportunity was used. Do NOT send `p` again on this boot.
COM12 closed and disposed; no live serial/build session. User told to stop and
reconnect mouse cable; physical action and Windows control observations pending.
Next: ask whether mouse was powered ON/unplugged within30seconds of arming and
whether any controls worked. A fresh attempt requires a normal receiver reboot
(physical intervention) and fresh readiness; do not rebuild/reflash/repeat baseline.
No firmware extraction/build/flash/baseline repeated. No cause for zero RX proven.
Milestone is an inconclusive no-RF capture, not a receiver regression conclusion.
User subsequently confirmed: powered mouse on late or missed the cue. This run
therefore did not test controls; no firmware failure inferred. User requests an
explicit pause whenever physical intervention is needed so it is noticeable.
PAUSED: ask user to turn mouse OFF/unplug USB, normally unplug/replug RECEIVER
(no double-reset), prepare harmless document/other pointer, then reply ready.
Keep mouse OFF until new actual arming confirmation; no automatic retry.

Updated 2026-11-15 (Asia/Manila). Read this section, README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-15-wireless-hid-prototype.md.
Detailed experiment history is preserved in RADIO_ANALYSIS.md; do not repeat
firmware extraction or completed generic captures merely to resume.

User preferences / authorization:
- Continue autonomously until physical intervention is needed.
- Keep this context updated after meaningful results, with board state/evidence/next step.
- Commit and push tested milestones. Use feature-based branch names, no codex prefix.
- Keep proprietary firmware, addresses, raw captures and UF2/build artifacts ignored.
- Leave local .vscode/ and INFO_UF2.TXT untracked unless specifically needed.

Git / current implementation:
- Remote: https://github.com/gabiXDpapi/mouse-diy-dongle.git (origin redirects from
  the older lowercase username). Main remains at initial commit782f701.
- V8 milestone3a40a90 was pushed to feature/bounded-ack-probe.
- Active new branch: feature/wireless-hid-prototype, based on3a40a90.
  Receiver checkpoint1879f7e was pushed with upstream tracking; use git status/log
  on resume. V2 pointer milestone7ecfe6d and v3 sourceeef48d3 are pushed.
- Added separate `firmware/build.ps1 -App receiver`, preserving hid/radio/probe.
- Receiver product DIY Mouse RX HID Probe, local USB ID2FE3:0008, CDC+HID.
  COM port may differ from COM11. HID endpoint interval1ms is not measured1000Hz.
- Same V8 discovery/ACK bytes/timing,one arm/discovery per boot. V3 receiver source
  now uses1024 ACK cap/10-second window;non-HID probe stays256/two seconds.
  Board now runs v3,passive/unarmed. No output before explicit serial `p`.
- Six-byte independent USB report: five buttons/padding, signed16 LE X/Y, signed8
  wheel. Unknown consumer/horizontal fields are omitted. No clipping or startup
  prefix dropping. Unique type1 reports only; duplicates are ACKed without replay.
  Idle packets do not consume report deduplication state (idle and data can share seq).
- USB enqueue occurs before ACK. Busy/error does not consume RF budget or advance
  deduplication; busy lets mouse retry, fatal error stops. Stop/error attempts neutral
  button release with bounded waits. USB failure cannot guarantee host delivery.
- USB reset/disconnect after configuration requires board reboot for this prototype.
  Continuous operation, recovery/hopping, latency and1000Hz remain unimplemented/unverified.

Verified radio / input evidence:
- V8 controlled wheel/idle capture:
  ignored build-pairing-analysis/probe-v8-wheel-up-20261115-002506.txt + summary JSON.
  1069 complete valid RX:928 discovery,18 length11,one length20,122 length2.
  118 ACKs (19 reports,99 idle),1973ms ACKed RX span,no errors,119 TXs complete.
  Fresh data seq17 and18 followed idle with the same sequence and each had buttons0,
  wheel+1,X/Y0. This supports idle acceptance/resumption and positive wheel-up byte1.
- Earlier controlled tests support left bit0,right bit1,rightward positive X and
  downward positive Y, with startup/cross-axis caveats. First16 reports repeatedly
  retained previous input; a large following report suggests queued/accumulated
  data, not proven queue/overflow behavior. Preserve evidence; no arbitrary filter.
- Static 91-byte mouse HID descriptor at0x1A750..0x1A7AA, pointer tuple at0x1A7AC:
  byte0 buttons1..5,byte1 signed wheel,bytes2..3 X16LE,bytes4..5 Y16LE,
  byte6 consumer usage0x238,byte7 usage0. Radio type1 queues the same eight bytes.
  Consumer/type5 meaning, other button tests and negative directions remain incomplete.

Board / local artifacts:
- Physical board now runs bounded Wireless HID Probe v3,flashed after user
  confirmed NICENANO ready/mouse wired. Verified bootloader and final4bc43f35... UF2.
  Passive/unarmed,one discovery and1024 ACK budgets unused. No `p` sent on v3.
  COM12 closed,no live serial/build session. Mouse last confirmed wired.
- Receiver USB enumeration:2FE3:0008,CDC COM12/MI_00,HID-compliant mouse/MI_02,
  all Windows PnP statuses OK. V1 five-second DTR startup passed: receiver v1 banner,
  256-ACK/two-second cap,1MHz timing,19channels,six zero-counter SCAN lines,no errors.
  Evidence: ignored build-pairing-analysis/receiver-v1-passive-20261115-005017.txt.
  No RF packets or arming occurred during baseline.
- First armed test: ignored build-pairing-analysis/receiver-v1-armed-20261115-005234.txt
  and summary JSON. User explicitly prepared off/unplugged mouse/empty desktop,
  then was told to switch ON/move only after arming confirmation.285 valid RX:
  254 discovery,one idle `00 00`,30 length11 standalone type5 status packets
  `00 00 45 06 01 00 00 00 00 00 00`. One idle ACK(seq0->1),two TXs completed,
  no USB busy/errors,no type1 movement packet. User confirmed NO wireless pointer
  movement. V1 filter rejected standalone tag45, so no report reached USB; this
  identifies a filter limit and does not validate/disprove the HID forwarding path.
  Queue-empty idle seq0 legitimately remained0 for the later status packet.
- V1 stopped after that test;user confirmed mouse wired/NICENANO ready for v2.
- Receiver v2/non-HID probe v9 now accept
  only the additional observed length11/tag45 status form. ACK it but never send
  it to HID or consume movement deduplication. Preserve all existing bounds/bytes/
  timing. Actual matcher/USB tests and receiver/probe rebuilds passed; radio/HID
  artifacts unchanged and compile separation verified. Fix pushed as b1b80cd.
  Source fix implemented;all eight tests pass,including actual matcher/status ACK
  and bridge no-output/no-dedup fixtures while USB busy. V1 UF2 preserved locally
  as ignored build-pairing-analysis/receiver-v1-cfc2eb58.uf2. Receiver v2 build/
  UF2 validation passed below;probe v9 build/separation checks also pass.
- V2 flash/passive baseline on2026-11-15: verified bootloader0.6.0,nRF52840-nicenano,
  S1406.1.1 and e80eb30c... UF2 hash/bounds,then copied application UF2. COM12
  returned;five-second DTR log shows v2 banner,256/two-second limits,1MHz timing,
  19channels,six zero-counter SCAN lines,no errors/no arming. Windows HID/CDC OK.
  Evidence: ignored build-pairing-analysis/receiver-v2-passive-20261115-010250.txt.
  User then confirmed off/unplugged/empty-desktop readiness. Armed v2 capture:
  ignored build-pairing-analysis/receiver-v2-armed-20261115-010552.txt + summary.
  415 complete valid RX:271 discovery,34 idle,110 length11.122 ACKs over1971ms:
  34 idle,one status tag45,87 unique type1 reports,all buttons/wheel0. No errors/
  USB busy;all123 TXs complete. X range-117..149,Y-64..96. ACK hardware timing
  ranges13..112/144..243/189..288us(handler/READY/END),including USB enqueue cost.
  USER CONFIRMED Windows pointer moved wirelessly with unplugged Glorious mouse,
  then stopped at the brief limit;no other pointer used. First end-to-end wireless
  USB pointer milestone achieved. No click/wheel HID or gaming-performance claim.
  User told to stop/reconnect cable;physical reconnection unconfirmed.
- Next source work: receiver v3 longer bounded input test (fixed1024 ACK attempts/
  10 seconds),while non-HID probe remains256/two seconds. Same radio bytes/timing,
  packet filters,USB mapping,deduplication,backpressure and neutral release. Shared
  fixed constants must drive actual deadlines and printed limits. Add protocol
  deadline/limit assertions in both modes,build/check before next flash. Capture
  must cover30s arming +10s data window:use45seconds,not old35second duration.
  V3 source is implemented;all eight tests pass,including both fixed-limit modes
  and actual deadline/expiry. Longer-window review caught a status-only256-frame
  wrap gap:advanced status now invalidates the old report retry marker without
  sending input;idle leaves it unchanged. Actual bridge regression passes.
  Final v3/probe/radio builds and separation checks pass;no live build/serial
  session remains. Do NOT flash intermediate a020e8ab... v3 build.
  Preserved working v2 at ignored build-pairing-analysis/receiver-v2-e80eb30c.uf2.
- V3 flash/passive baseline on2026-11-15: verified UF2 bounds/final hash4bc43f35...,
  bootloader0.6.0,nRF52840-nicenano,S1406.1.1;copied application UF2. COM12 returned.
  Five-second DTR log passed:v3 banner,1024 ACKs/10000ms,1MHz timing,19channels,
  six zero-counter SCAN lines,no errors/no arming. Windows composite/CDC/HID all OK.
  Evidence: ignored build-pairing-analysis/receiver-v3-passive-20261115-012451.txt.
  User requested next-chat handoff before any v3 arming. No controls test yet.
- V8 UF2 preserved: ignored build-pairing-analysis/probe-v8-4ad89938.uf2.
  SHA256 4ad89938b68c166ebb3b73fea46878b763cd97974c3f9424acc227f0e45824f4.
- Earlier V4/V5/V6/V7 artifacts are also preserved in ignored evidence.
- Known bootloader:0.6.0,nRF52840-nicenano,S1406.1.1. Last bootloader drive D:;
  verify identity anew before flashing, do not erase/update bootloader.
- Preserved receiver v1 (FLASHED; status filter blocked first armed test):
  ignored build-pairing-analysis/receiver-v1-cfc2eb58.uf2,
  SHA256 cfc2eb5820c34969f7299ee43b47a28b20bd1421dd5ac70acce816f566441b4b.
- Preserved receiver v2 artifact (FLASHED; pointer test passed,user confirmed):
  ignored build-pairing-analysis/receiver-v2-e80eb30c.uf2
  SHA256 e80eb30c51fa375b39a2af5a6d15de65341765673db0682fc9fe60a95591b6c9.
  Build/UF2 pass:flash60,304/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF.
- Current receiver v3 (FLASHED; passive passed,extended controls test pending):
  C:\Users\Gabriel\zephyrproject\build-mouse-receiver\zephyr\zephyr.uf2
  SHA256 4bc43f3543638f337eb7d50d42621cc20be321e79ebdefed2ba8d94d9b73d4da.
  Flash60,332/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF.
- SDK DMA macro rounds six-byte allocation up to eight; code explicitly copies/
  transmits six bytes. First staged build stopped at obsolete allocation assertion;
  corrected build above passed. Eight tests pass, including the native bridge
  with padded-buffer/asynchronous USB stubs and protocol in probe/receiver modes.
  Native tests alone do not establish gaming performance;v2 hardware proved
  bounded pointer output,and v3 extended controls still need hardware validation.
- All four builds, UF2 checks and config/binary separation checks pass. No live
  build/serial session remains. HID descriptor/forwarding strings exist only in
  receiver; RF arming exists only in receiver/probe. Regression artifacts:
  current probe v9:flash58,808/RAM15,936,230 blocks,0x26000..0x345FF,
  SHA256 8fc95a4738701e29ed84f72e3430ccce89a450c917c7498f2879338402ff4de5.
  radio:flash56,760/RAM15,872,222 blocks,0x26000..0x33DFF,
  SHA256 6f0f263f5095a064aa8c782d42bb06973eef9c569eaf6c9ab745f0e3689eab8e.
  default HID:129 blocks,0x26000..0x2E0FF,unchanged SHA256
  058c1ff58130ab07c14f258cde93df331e5345f99b4b673db0ede46d2f4a5953.
  These supersede earlier hashes at the same build paths; V8 preserved separately.

Exact next steps:
1. Handoff requested;resume by reading this file,README.md,RADIO_ANALYSIS.md and
   docs/plans/2026-11-15-wireless-hid-prototype.md. Source/build checks complete;
   v3 is flashed/passive/unarmed. Do not rebuild,reflash or repeat passive baseline.
2. Ask separate readiness on a harmless scrollable document/page,another pointer,
   mouse OFF/unplugged,left OFF until arming confirmation. Mouse last wired;
   do not assume the user has prepared this yet.
3. After explicit readiness,open COM12/DTR,send `p` ONCE,emit immediate confirmation,
   save a45-second receiver-v3-armed-... raw log under ignored build-pairing-analysis.
   When exec yields,tell user to switch mouse ON promptly and exercise movement,
   deliberate left/right clicks and wheel within the10-second active window.
   Discovery arming is30s;capture45s covers30s+10s. No automatic rearm/retry.
4. At capture end,tell user to stop/reconnect cable;ask which controls worked and
   whether output/buttons stopped/released. Inspect ACK count<=1024,packet/sequence
   progress,idle/status vs type1,deduplication,USB busy/errors and hardware timing.
   Do not count repeated idle sequence as failed ACK or duplicate movement.
5. Save summary,update context/research,push tested milestone on feature branch.
   No repeat firmware reads or generic v2 pointer trial.
   Do not claim reliable gaming receiver completion from a bounded HID demonstration.

The firmware snapshot is an application-region read, NOT a full-chip backup.
Do not flash proprietary receiver firmware onto this board or write mouse config.
Do not invent speculative RF decoding beyond established evidence.

Live step: COM12 opened with DTR; sending one p now. Capture: C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-armed-20261115-013158.txt. If interrupted, do NOT rearm; inspect this log first.
Live step: actual v3 arming text confirmed; 45-second capture running. One p sent; do NOT send another. Await power-on/input observations.
Live step: 45-second capture finished; arming confirmed=True. Port closing; inspect raw log and ask user controls/neutral-stop outcome before conclusions.

LIVE RETRY 20261115-013553 : user confirmed normal-reboot readiness. COM12 opened/DTR; sending exactly one p. Raw capture C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-armed-20261115-013553.txt. Do not rearm if interrupted; inspect log first.
LIVE RETRY: actual v3 arming confirmed. Capture running45seconds; one p sent. User requests pop-up for intervention cues.
LIVE RETRY:45-second capture completed; armingConfirmed=True. COM12 closing; inspect log and await user observations. No automatic rearm.

LIVE RF DIAGNOSTIC 20261115-013920 : user confirms mouse ON/unplugged/near unchanged receiver. COM12 DTR open;15-second capture WITHOUT any serial writes. Raw log C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-rf-silence-20261115-013920.txt. No p/rearm/reset.
LIVE RF DIAGNOSTIC: serial disposed/closed; inspect raw log before next action. No commands sent.

LIVE DRAINED TRIAL 20261115-014330 : user confirms receiver reboot/mouse OFF/unplugged. COM12/DTR open; same reader drains2seconds before one p,then45seconds. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-drained-20261115-014330.txt and .host.jsonl. Inspect logs before any retry.
LIVE DRAINED TRIAL: pre-arm drain complete; exactly one p sent. Await actual arming confirmation; no second p.
LIVE DRAINED TRIAL: actual new arming text confirmed after drain;45-second capture running. Power-on pop-up next; no automatic retry.
LIVE DRAINED TRIAL: COM12 closed/disposed;armSent=True confirmed=True. Raw/host-phase evidence saved;inspect before conclusions. No live serial session.

LIVE CONTROLS TRIAL 20261115-014826 : user confirmed previous v3 movement stopped at limit without unplugging. Fresh receiver reboot/mouse OFF/unplugged ready. COM12/DTR open;same reader drains2seconds then ONE p and45second capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-controls-20261115-014826.txt and .host.jsonl. No automatic retry.
LIVE CONTROLS TRIAL: drain complete;one p sent. Await new arming text;never send second p this boot.
LIVE CONTROLS TRIAL: NEW actual v3 arming confirmed;capture running. Power-on pop-up directs immediate clicks/wheel. No automatic retry.
LIVE CONTROLS TRIAL: COM12 closed/disposed;armSent=True confirmed=True. Inspect raw/host logs and user observations. No live serial session.

LIVE HELD-BUTTON TRIAL 20261115-015320 : user confirms normal receiver reboot,mouse OFF/unplugged,plain-text/other pointer ready. COM12/DTR open;continuous reader drains2s then ONE p/45s. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v3-held-20261115-015320.txt and .host.jsonl. No automatic retry.
LIVE HELD-BUTTON TRIAL: drain complete;exactly one p sent. Await actual new arming text;no second p this boot.
LIVE HELD-BUTTON TRIAL: NEW actual v3 arming confirmed;45s capture running. Power-on/hold-left pop-up next. Inspect last ACKed buttons and user selection observation before pass.
LIVE HELD-BUTTON TRIAL: COM12 closed/disposed;armSent=True confirmed=True. Raw/host evidence saved;await user observation. No live serial session.

LIVE FLASH: v4 application UF2 copied to verified D:/NICENANO. Await USB enumeration/passive startup;no p sent.

LIVE V4 RECOVERY 20261115-140838 : user confirmed document/other pointer/mouse OFF-unplugged. Receiver NOT reset since passive v4 check. COM12/DTR open;continuous reader drains2s before ONE p/95s capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v4-recovery-20261115-140838.txt and .host.jsonl. No automatic retry.
LIVE V4 RECOVERY: drain complete;one p sent. Await NEW RECEIVER armed text;never send second p this boot.
LIVE V4 RECOVERY: NEW actual arming confirmed;95s capture running. Power-on/15s controls/OFF3s/ON pop-up next. Shared budgets/deadline;no receiver reset.
LIVE V4 RECOVERY: COM12 closed/disposed;armSent=True confirmed=True. Inspect raw/host/link-phase evidence and user recovery/stop observations. No live serial session.

LIVE V5 FLASH: user NICENANO/mouse wired-OFF ready;verified bootloader0.6.0/nicenano/S1406.1.1 and ab8e3001... hash/bounds. Copying application only;NO arming. Time14:24 Manila;passive check then pause.
LIVE V5 FLASH: application UF2 copied;await enumeration/passive check. NO p sent.
LIVE V5 PASSIVE: COM12 closed/disposed;log C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v5-passive-20261115-142507.txt. No commands sent;inspect before conclusions.

LIVE V5 COMPARISON 20261115-142958 : user explicitly selected readiness acknowledging wrap-up past14:30,then break. Glorious OFF/unplugged,document/other pointer ready. COM12/DTR open;continuous2s drain thenONE p/95s capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v5-comparison-20261115-142958.txt and .host.jsonl. Do not rearm/reset;no new work after this test.
LIVE V5 COMPARISON:2s drain complete;ONE p sent. Await NEW arming text;no second p.
LIVE V5 COMPARISON: NEW actual arming confirmed;95s capture running. Power-on/delay-comparison/recovery pop-up next. This is final trial before break.
LIVE V5 COMPARISON: COM12 closed/disposed;armSent=True confirmed=True. Inspect cumulative stats/link-stop totals and user latency/control observations. No live session;save result then BREAK.

LIVE V6 FLASH: user NICENANO/mouse wired-OFF ready;verified bootloader0.6.0/nicenano/S1406.1.1 and c0e18e29... hash/bounds. Copying application only;NO arming.
LIVE V6 FLASH: application UF2 copied;await enumeration/passive check. NO p sent.
LIVE V6 PASSIVE: COM12 closed/disposed;log C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v6-passive-20261115-224120.txt. No commands sent;inspect before conclusions.

LIVE V6 INPUT 20261115-224452 : user confirmed document/other pointer/Glorious OFF-unplugged. Receiver unchanged since passive check. COM12/DTR open;continuous2s drain thenONE p/95s capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v6-input-20261115-224452.txt and .host.jsonl. Do not rearm/reset if interrupted.
LIVE V6 INPUT: drain complete;ONE p sent. Await new v6 arming text;no second p.
LIVE V6 INPUT: NEW actual65536-cap arming confirmed;95s capture running. Power-on/controls/recovery pop-up next;no automatic retry.
LIVE V6 INPUT: COM12 closed/disposed;armSent=True confirmed=True. Inspect ACK-type/HID submission/retry counters and user observations. No live serial session.

LIVE V6 MOTION 20261115-225106 : user confirmed normal receiver reboot/Glorious OFF-unplugged/document-other pointer ready. Same v6;NO build/flash/baseline. Continuous COM12/DTR2s drain thenONE p/95s capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v6-motion-20261115-225106.txt/.host.jsonl. No rearm/reset if interrupted.
LIVE V6 MOTION:2s drain complete;ONE p sent. Await new65536-cap arming;do not send second p.
LIVE V6 MOTION: NEW actual arming confirmed;95s capture running. Power-on/30s steady motion pop-up next;no clicks/wheel/OFF cycle requested.
LIVE V6 MOTION: COM12 closed/disposed;armSent=True confirmed=True. Inspect time-binned counters/busy bursts/link transitions and user freeze times. No live serial session.

LIVE V7 FLASH: user NICENANO/mouse wired-OFF ready;verified bootloader0.6.0/nicenano/S1406.1.1 and b6a63b99... UF2 hash/bounds. Copying application only;NO arming.
LIVE V7 FLASH: application copied;await enumeration/passive check. No p sent.
LIVE V7 PASSIVE: COM12 closed/disposed;log C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v7-passive-20261115-231231.txt. No commands sent;inspect before conclusions.

LIVE V7 MOTION 20261115-231733 : user clarified detection concern was expected unarmed state and asked proceed. Windows COM12/composite/HID/Input all OK;no Glorious wired device enumerated. Told keep Glorious OFF/unplugged until cue. Continuous COM12/DTR2s drain thenONE p/95s. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v7-motion-20261115-231733.txt/.host.jsonl. No reset/rearm if interrupted.
LIVE V7 MOTION: drain complete;ONE p sent. Await NEW arming text;no second p.
LIVE V7 MOTION: NEW actual arming confirmed;95s capture running. Power-on/30s steady-circle comparison pop-up next. Idle ACK holds sequence only;no automatic retry.
LIVE V7 MOTION: COM12 closed/disposed;armSent=True confirmed=True. Inspect stats/type1/HID/retry/busy and user speed-consistency outcome. No live serial session.

LIVE V7 DPI1600 20261115-233835 : user confirms normal receiver reboot/Glorious OFF-unplugged/document-surface-other pointer ready. CORE Profile1 saved1600 via wired;wireless retention unverified. No DPI-button or Windows/firmware changes. COM12/DTR2s drain thenONE p/95s capture. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v7-dpi1600-20261115-233835.txt/.host.jsonl. Do not reset/rearm if interrupted.
LIVE V7 DPI1600: drain complete;ONE p sent. Await NEW arming text;no second p.
LIVE V7 DPI1600: NEW actual arming confirmed;95s capture running. Power-on/comparison pop-up next;do not press DPI button.
LIVE V7 DPI1600: COM12 closed/disposed;armSent=True confirmed=True. Inspect counters and user wired-vs-wireless control/stop observations. No live serial session.

LIVE V7 DPI RETRY 20261115-234324 : user confirmed normal receiver reboot,Glorious OFF/unplugged,hand near power switch,same document/surface/other pointer. CORE Profile1 saved1600;no DPI-button/Windows/firmware changes. COM12/DTR2s drain thenONE p/95s. Evidence C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\receiver-v7-dpi1600-retry-20261115-234324.txt/.host.jsonl. Do not rearm/reset if interrupted.
LIVE V7 DPI RETRY: drain complete;ONE p sent. Await NEW actual arming;no second p.
LIVE V7 DPI RETRY: NEW actual arming confirmed;95s capture running. Immediate switch-ON/no-DPI-button comparison pop-up next.
LIVE V7 DPI RETRY: COM12 closed/disposed;armSent=True confirmed=True. Inspect counters and user saved-DPI control/consistency/stop result. No live serial session.
