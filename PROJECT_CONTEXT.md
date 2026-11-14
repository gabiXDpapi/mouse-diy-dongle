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
- [ ] Investigate the mouse's pairing commands
- [ ] Discover the proprietary RF protocol
- [ ] Implement RF reception and pairing
- [ ] Translate wireless packets into USB HID reports
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

## Immediate Next Task for Codex

The USB HID prototype source is in `firmware/`; see `README.md` for
build, UF2 validation, flashing, and Windows verification. Zephyr v4.2.0 and
SDK 0.17.2 are installed. Blinky execution, power-cycle restart, and double-reset
bootloader re-entry have been confirmed by the user. Wireless reception is not implemented.

HID hardware results confirmed on 2026-11-14 (Asia/Manila):
- Device Manager screenshot shows HID-compliant mouse with `VID_2FE3&PID_0007`.
- User confirmed right movement followed by left movement, then LED off.
- User confirmed the NICENANO bootloader drive returns.
- No polling-rate or wireless performance measurements have been made.

The wired mouse Hardware Id has now been recorded above.
Next, inspect the pairing utility and investigate public firmware information,
and USB/RF protocol evidence before implementing wireless reception.

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
User requests ongoing updates to this file after meaningful results, including
current hardware state, evidence paths, and the exact next step, so a new chat can
resume safely. Preserve this practice throughout the project.
User also requests committing and pushing tested milestones to origin
`https://github.com/gabixdpapi/mouse-diy-dongle.git`. Exclude proprietary firmware,
device-specific address observations, raw captures and generated build artifacts.
User permits separate branches when useful and requests feature-based names,
without the codex prefix. Experimental source milestone uses
`feature/bounded-ack-probe`, with push target `origin/feature/bounded-ack-probe`; main remains at initial
commit782f701. Milestone `e7441e3` (source, tests, research and V5 prep) was
successfully pushed to that feature branch. Firmware/captures/address evidence
remain ignored; local `.vscode/` and `INFO_UF2.TXT` were deliberately not staged.
Use `git status -sb` and `git log -1` to check the current checkpoint
and upstream state on resume. No automatic version tag or binary release requested.
Resume check on 2026-11-14: V4 artifact hash above matches; UF2 validator passes
230 blocks, 0x26000..0x34600 (end exclusive). User confirmed NICENANO ready
and mouse wired. Verified D: INFO_UF2.TXT: bootloader0.6.0, board-ID
nRF52840-nicenano, S1406.1.1. Rechecked V4 hash and copied UF2 successfully.
Five-second DTR-enabled COM11 baseline passed: V4 banner, `TIMING hardware
capture initialized at 1 MHz`, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v4-passive-20261114-232306.txt`.
User then confirmed battery-mode readiness. Armed V4 once and captured35seconds.
Evidence: ignored `build-pairing-analysis/probe-v4-armed-20261114-232436.txt`
and `.summary.json`. Exactly one arm, discovery send, and ACK send; no errors.
991 CRC-valid RX lines:960 discovery,31 eleven-byte reports on channel51.
FIRST POSITIVE ACK RESULT: first report sequence0 at t119735, then30 reports
sequence1 at t119749..119974, with changed payload. Payloads:
`00 00 41 00 00 01 00 00 00 00 00` then
`01 00 41 00 00 00 00 FF FF 00 00`. Supports acceptance of one-shot seq0->1 ACK;
does not prove sustained link, movement decoding, or deterministic ACK timing.
Hardware timing (RX END to handler/TX READY/TX END, us): discovery19/401/449;
data ACK63/194/239. Both completed=1, MODECNF0=0x200. Timing instrumentation
works on hardware. V4 differs from V3, so cannot isolate why V3 showed no advance.
Board runs V4 with both TX budgets consumed, passive until reboot. User told
to reconnect mouse cable; actual reconnection not confirmed. No live serial/build
session; COM11 closed cleanly. No additional arming or flash is needed now.
Resume by reading this file, README.md and
research/RADIO_ANALYSIS.md; do not repeat the firmware extraction or old probes.
User authorized continuing until physical intervention is needed. V5 source now
permits one discovery plus at most16 data ACK attempts in the unchanged two-second
window; timeout exhausts remaining ACK budget, no rearm or deadline extension.
V4 response bytes, timing helpers, exact filters and channel51 retained. No HID.
All seven tests pass, including extended native count/expiry/timeout/wrap checks.
Plan: `docs/plans/2026-11-14-bounded-ack-probe.md`.
Preserved V4 UF2: ignored `build-pairing-analysis/probe-v4-2bef7486.uf2` (hash above).
V5 Zephyr build and UF2 validation passed: flash58,716/RAM15,936 bytes,
230blocks, payload0x26000..0x345FF. Current probe UF2 path unchanged; SHA256:
`a1b78c57b5010fb7d7de5e7eb997e1f96e012de0040ee4455b4e1abae49031fe`.
V5 subsequently flashed; passive baseline passed (details below). Receive-only regression build passed: flash56,704/RAM15,872,
222blocks,0x26000..0x33DFF; its new UF2 SHA256 supersedes earlier radio hashes:
`8a19a4c22319ddb93fd8f2e49eabb6dd6548f973a8eed519cda28a0b188f3339`.
CMake flags ON(probe)/OFF(radio) and binary banner/arming/ACK strings confirm
compile separation. USB-reader self-tests passed without hardware; exact-address
scan found no device address in22 candidate source/doc files. Source milestone is
pushed on feature/bounded-ack-probe; context checkpoint15f52bd followed e7441e3.
User confirmed NICENANO ready and mouse wired. Reverified bootloader0.6.0,
board-ID nRF52840-nicenano, S1406.1.1 and V5 hash a1b78c57...; copied V5 UF2.
Five-second DTR-enabled COM11 passive baseline passed: V5 banner,1MHz timing
initialization,19channels,six zero-counter SCAN lines,no errors. Evidence:
ignored `build-pairing-analysis/probe-v5-passive-20261114-233754.txt`.
After battery readiness, armed V5 once and captured35seconds. Evidence: ignored
`build-pairing-analysis/probe-v5-armed-20261114-233953.txt` and summary JSON.
1003 valid packets:958 discovery,15 eleven-byte reports(seq0..14,each once),
30 twenty-byte reports(seq15 repeated),all45 data packets on51. Fifteen ACKs
sent(count1..15),no errors; data report timestamps139159..139623ms. Eleven-byte
reports were16ms apart, an observation of this polling/logging experiment only.
Type1 payload varied; no controlled movement/button decoding yet. Twenty-byte
payload:`0f004100000000ffff0000450601000000000000` contains sequence bytes,
tag41/eight-byte type1 report,then tag45/eight-byte type5 block. V5 exact-length
filter refused it, explaining no ACK16; do not label this as radio timing failure.
Discovery timing42/424/472us(handler/READY/END); data ACK ranges24..70/
155..201/199..245us. All16 transmissions completed=1,MODECNF0=0x200.
Board V5 discovery consumed,15/16 ACK attempts used,ACK window expired; passive
until reboot with no rearm. COM11 closed,no live build/serial session. User told
to reconnect cable; physical reconnection unconfirmed.
V6 source accepts only the observed 11-byte tag 0x41 or 20-byte tag 0x41+0x45
shapes, with the same address/channel/CRC filters, 16-ACK cap, two-second window,
timing and response bytes. All seven tests pass, including the captured compound
fixture, all unsupported lengths 0..64, wrong tags and address/channel cases.
V6 build and UF2 validation passed: flash 58,740 / RAM 15,936 bytes; 230 blocks,
payload 0x26000..0x345FF. Current probe artifact (supersedes V5 at same path):
`C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`
SHA256 `dbf0b69cd0e4d61656d3bb6ef61b4782783d97c80b3293222d881a2de8c66020`.
V5 artifact preserved as ignored `build-pairing-analysis/probe-v5-a1b78c57.uf2`.
V6 subsequently flashed; passive baseline passed below. Receive-only regression build and binary separation checks
passed; radio UF2 hash remains 8a19a4c2... above. No live build/serial sessions.
Source/results checkpoint d032877 was pushed to feature/bounded-ack-probe.
User confirmed NICENANO ready and mouse wired. Reverified UF2 bounds/hash dbf0b69c...,
bootloader 0.6.0, board-ID nRF52840-nicenano and S140 6.1.1; copied V6 UF2.
Five-second DTR-enabled COM11 passive startup passed: expected V6 banner,
1 MHz timing initialization, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v6-passive-20261114-234710.txt`.
V6 capture completed after battery readiness: ignored
`build-pairing-analysis/probe-v6-armed-20261114-234923.txt` and summary JSON.
1004 valid RX packets: 958 discovery and 46 data on channel 51. Sequences 0..15
each appeared once, then sequence 16 repeated 30 times. One length-20 packet at
sequence 15 was ACKed successfully; other 45 data packets were length 11.
All 16 ACKs sent, 17 total transmissions completed, no errors. Data timestamps
153381..153860 ms. Compound payload:
`0f00410000ffffffff0000450601000000000000`.
Final repeated payload `1000410100bdfb4e130000` includes changed type-1 bytes;
movement/buttons still need controlled interpretation. Discovery timing
18/399/447 us (handler/READY/END); ACK ranges 13..72/144..203/189..248 us.
This supports compound ACK acceptance and sequence advancement to 16. Repetition
after budget exhaustion is expected, not evidence of failed ACK16.
Board V6 all TX budgets consumed/passive until reboot. COM11 closed; no live
build/serial session. User told to reconnect mouse cable; reconnection unconfirmed.
Next development step: V7 raises ACK cap to 256 while retaining the two-second
deadline, observed packet shapes, timing/bytes, explicit arming and no HID. It
tests a longer bounded exchange; do not claim sustained receiver performance.
V7 source implemented; all seven tests pass, including fixed 256 cap and expiry
after partial budget use. V6 UF2 preserved at ignored
`build-pairing-analysis/probe-v6-dbf0b69c.uf2`. V7 build and UF2 validation pass:
flash 58,732 / RAM 15,936 bytes, 230 blocks, payload 0x26000..0x345FF.
Current probe UF2 path unchanged; SHA256 (supersedes V6 there):
`e9e79cce535839d72fc4ca338ee1fc85a62f8b9ee68194af6139deaad69c8cee`.
V7 subsequently flashed; passive baseline passed below. Receive-only regression and binary separation checks pass:
flash 56,704 / RAM 15,872 bytes, 222 blocks, payload 0x26000..0x33DFF; new radio
UF2 SHA256 (supersedes previous radio hashes):
`3b459262846fbae3f30ded047c5a768bc9e44ad7bf2490fdbddfee85c6f584d1`.
Tested source/results checkpoint 05bbdb7 was pushed on feature/bounded-ack-probe.
User confirmed NICENANO ready and mouse wired. Reverified UF2 bounds/hash e9e79cce...,
bootloader 0.6.0, nRF52840-nicenano and S140 6.1.1; copied V7 UF2 successfully.
Five-second DTR-enabled COM11 baseline passed: V7 banner, 256-ACK/two-second limit,
1 MHz timing initialization, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v7-passive-20261114-235645.txt`.
V7 armed capture completed after battery readiness. Evidence: ignored
`build-pairing-analysis/probe-v7-armed-20261114-235814.txt` and summary JSON.
1059 CRC-valid packets: 913 discovery, 146 data (145 length11, one length20).
124 ACKs sent for sequences0..123, followed by received sequence124, no errors.
Acknowledged RX timestamps112128..114097 ms (1969 ms span); timing is consistent
with the fixed two-second window ending before the256-ACK cap. Exact deadline
start is not directly timestamped in the text; guard tests independently pass.
Channel51:130 data packets (seq0..123 once,seq124 six times);16 additional
seq124 repeats logged after ACKs stopped on27/76/4 and49/78. The former match
group0 retry channels;49/78 may be adjacent-channel reception, not exact TX
frequencies. Do not claim full hopping/recovery implementation from this.
One compound report was ACKed. All125 TXs completed,MODECNF0=0x200;
discovery timing40/421/469us (handler/READY/END),data ACK ranges12..73/
143..204/187..248us. Acknowledged RX gaps15..17ms are logger observations,
not measured mouse/receiver throughput. No HID output or1000Hz claim.
Board V7 discovery consumed,124/256 ACK attempts used,ACK window expired;
passive until reboot and cannot rearm. COM11 closed,no live build/serial session.
User told to reconnect mouse cable; physical reconnection unconfirmed.
User confirmed normal V7 reboot and mouse wired. Fresh five-second DTR-enabled
COM11 startup on 2026-11-15 passed: V7 banner, 256-ACK/two-second limit, 1 MHz
timing initialization, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v7-input-passive-20261115-000319.txt`.
Controlled battery capture completed after user explicitly confirmed holding
left button and moving only right/lifting to return (no wheel/other buttons).
Evidence: ignored `build-pairing-analysis/probe-v7-left-right-20261115-000519.txt`
and summary JSON. 1070 valid RX:924 discovery,146 data (145 length11,one length20).
124 ACKs,seq0..123 uniquely ACKed,1968 ms span,no errors. Decode unique ACKed
reports only; repetitions must not be counted as fresh movement.
Button byte0:seq0..15 mask0,seq16..123 mask1 (108 reports); wheel and consumer
fields0 throughout. First16 reports may be previously queued data (hypothesis).
Seq16 has X22768,Y7359; do not silently discard this large transition report.
Remaining107 reports(seq17..123) all have X positive25..58,Y positive9..37.
This supports left-button bit0 and rightward positive X, but substantial Y means
axis isolation was imperfect. Initial backlog/accumulation is plausible, unproven.
No claim of full movement decoding or measured latency; button release/right
button/Y direction/wheel still need controlled checks. User told to stop, release
button and reconnect cable; physical reconnection unconfirmed.
Board V7 discovery consumed,124/256 ACK attempts used,window expired/passive
until reboot. COM11 closed; no live serial/build session. No source change needed.
User then confirmed normal V7 reboot and mouse wired for the right/down test.
Five-second fresh COM11/DTR baseline passed: expected V7 banner, 256-ACK/two-second
limit, 1 MHz timing initialization, 19 channels, six zero-counter SCAN lines,
no errors/no arming. Evidence: ignored
`build-pairing-analysis/probe-v7-right-down-passive-20261115-000859.txt`.
Board V7 now passive/unarmed with fresh unused budgets; COM11 closed, no live
build/serial session. User asked for battery readiness while holding right button
and moving straight down toward them/lifting to return, no left button/wheel.
Readiness pending. Next: after confirmation, arm once/capture 35 seconds under
distinct `probe-v7-right-down-...` filename. Tell user to stop/release/reconnect
after capture. Compare bit1 and signed Y direction using unique ACKed reports;
retain initial packets/outliers and report cross-axis motion without assumptions.
Keep this distinct from the completed generic V7 capture; do not repeat generic
captures or firmware reads. Further separate Y/wheel checks follow if supported.
Additional static finding: 91-byte HID mouse descriptor at 0x1A750..0x1A7AA,
referenced by length/type/pointer tuple at 0x1A7AC (91,0x22,0x1A750). It describes
the eight-byte wired report also queued as RF type1 by 0x14D20..0x14DC8: byte0
buttons1..5 (upper3 padding), byte1 signed wheel, bytes2..3 signed16 LE X,
bytes4..5 signed16 LE Y, byte6 signed consumer usage0x238, byte7 consumer usage0.
Controlled left/rightward test supports bit0 and positive X, with limitations
above; the other layout fields remain static evidence. Type5 remains unknown;
no pointer output is implemented. See RADIO_ANALYSIS.md for details.
Offline descriptor-based interpretation of124 uniquely ACKed V7 reports found
button masks0/1,wheel0,X -43..412,Y -142..196,consumer fields0. These came from
uncontrolled movement/clicks and do not validate specific physical directions.
Do not repeat firmware reads or old captures. Do not claim pairing completion or
verified movement decoding from these bounded exchanges.
Power-cycle mouse if a transient connection state needs clearing.
Sustained pairing, movement decoding, reliable acknowledgment scheduling, and
1000 Hz performance remain unimplemented/unverified. Snapshot is not full-chip backup.
See `research/PAIRING_FINDINGS.md` and `research/RADIO_ANALYSIS.md` for evidence.

Do not begin writing speculative Glorious RF decoding code until the actual RF protocol has been established.
