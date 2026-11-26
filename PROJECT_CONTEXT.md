# Glorious Series One PRO Centauri â€” Custom Wireless Receiver Project

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
- Purchased from Shopee for approximately â‚±200
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

### Milestone 1 â€” Development Environment

Set up a C/C++ firmware development environment suitable for the nice!nano-compatible nRF52840.

Prefer a workflow that preserves the existing UF2 bootloader and supports firmware flashing over USB.

### Milestone 2 â€” USB HID Prototype

Create firmware that enumerates as a USB HID mouse.

Initially generate simulated mouse reports to verify USB communication.

### Milestone 3 â€” Radio Initialization

Configure the nRF52840's proprietary 2.4 GHz radio and establish controlled radio experiments.

### Milestone 4 â€” Protocol Investigation

Investigate Glorious' proprietary wireless protocol using publicly available firmware, Glorious pairing software, USB/HID captures, and RF observations where appropriate.

Determine the packet format, RF address, channel selection, bitrate, acknowledgements, pairing information, and any authentication.

### Milestone 5 â€” Receiver Implementation

Implement the recovered radio and pairing behavior on the nice!nano.

Translate the mouse's movement, clicks, and scrolling into standard USB HID reports.

### Milestone 6 â€” Gaming Performance

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

NOVEMBER 26 — WIRED HID LAYOUT INSPECTION PASSED; AUXILIARY HID FORWARDING NEXT.
Resumed on feature/glorious-core-compatibility. User confirmed CORE closed,
Glorious wireless switch OFF, direct USB DATA cable connected with wired movement
verified, and DIY receiver unchanged (exact tested normal v14 remains installed).
32-bit SysWOW64 research/inspect_wired_hid.ps1 passed read-only (no Feature or
settings requests, no streams, no flash, no input recording):
WIRED_HID_LAYOUT_PASS: 7 collections. Private result:
build-pairing-analysis/wired-hid-layout-20261126-183213-b93b7b4b.json.

Verified wired HID collections (HidSharp Windows view cross-checked with already-
extracted local mouse dump 0x1a7b8 and disassembly 0x14bb6..0x150b2 + CORE static
ModelOSeries.js; no new extraction performed):
- Mouse (0001:0002, ID-less, 8B payload) <- RF type 1 (0x41, 8B)
- Consumer (000C:0001, Report ID 1, 2B payload, 3B total) <- RF type 3 (0x13, 2B)
- System Control (0001:0080, Report ID 2, 1B payload, 2B total, usages 81..83) <- RF type 4 (0x0C, 1B)
- Keyboard (0001:0006, Report ID 3, 8B input + 1B LED output, 9B/2B total) <- RF type 2 (0x42, 8B)
- Vendor Notify (FFA0:0001, Report ID 4, 8B payload, 9B total) <- RF type 5 (0x45, 8B; matches CORE 04 06 01 wireless-on notify)
- Vendor Notify (FFFF:0001, Report ID 5, 8B payload, 9B total) <- RF type 6 (0x46, 8B)
- Vendor Config Feature (FFFF:0000, ID-less, 64B Feature) <- RF type 7 (0x47/0xC7, 8B chunks)
Note: HidSharp Windows view masks array fields (keyboard keycodes, 16-bit consumer
usage) as Constant; local dump confirms standard 6KRO keyboard (1B mod, 1B pad,
6B keycodes 0x00..0x65) and 16-bit consumer usage array (0x0000..0x023C).

NEXT autonomous software milestone: implement auxiliary HID forwarding (Consumer,
System Control, Keyboard, and Vendor FFA0/FFFF notifications) on a separate
prototype staging candidate (v26, local prototype USB IDs, normal v14 untouched)
with full native C tests for multi-record RF parsing, HID-busy withholding without
duplicating already-forwarded records in compound packets, and neutral release of
active keyboard/consumer/system state on link loss or stop. Build and validate UF2
before requesting physical readiness. Full CORE identity remains disabled until
auxiliary forwarding and startup setters qualify.

PAUSED BY USER NOVEMBER21 â€” FUTURE RESUME MAY USE A DIFFERENT OPENAI ACCOUNT.
User requested stop until a later date. No hardware work after readiness check:
Windows wired258A:2018 root count0; inspect_wired_hid.ps1 NOT run. No settings
request, flash, capture, build or live serial/helper/timer. Normal exact tested
v14 remains last user-confirmed working. V25 original400/800/1600/3200 DPI list
and selectedstage2=800 restored/readback verified. Private current-config-hid
backup checked: NeedsWiredRestore=false. No pending recovery.
Keep project PAUSED until user explicitly resumes. Later account should open the
SAME local workspace, read RESUME_PROMPT.md and this newest handoff, and continue
from files rather than assuming access to this account's chat history.
NEXT fresh physical popup: COREclosed, Glorious wirelessswitchOFF, connected via
working USB DATA cable with wired movement confirmed; leaveDIYreceiverunchanged.
Then run32-bit research/inspect_wired_hid.ps1 read-only Windows descriptor view.
No settings requests/flash/inputrecording. This supports keyboard/media/system
forwarding for CORE remapping; full CORE app support remains unfinished.
Retain the WHOLE local folder, including ignored build-pairing-analysis and .git.
GitHub alone omits private RF parameters, backups, source evidence and rollback
UF2s needed by this hardware project. Do not publish those ignored files.
Same Windows user/computer also retains external Zephyr SDK/toolchain under
C:\Users\Gabriel\zephyrproject. A different computer requires environment and
private-path verification before any hardware work. Branch remains
feature/glorious-core-compatibility. All milestones should be committed/pushed,
context/resume prompt updated for meaningful steps, and physical intervention
requested with popup/pause. User prefers autonomous progress until intervention
or an explicit stop. Four-hour reliability trial deferred for CORE compatibility.

NOVEMBER21 RESUME READINESS CHECK: NO WIRED MOUSE PRESENT.
User said continue. CORE process check clear; present healthy wired258A:2018
root count0. inspect_wired_hid.ps1 was NOT invoked; no streams, settings requests,
flash, capture or build. Receiver unchanged; last confirmed normalv14 works.
NEXT physical popup/pause: COREclosed, Glorious wirelessswitchOFF, connect its
USB DATA cable directlyPC and confirm wired movement. LeaveDIYreceiver unchanged.
Then recheck wired presence before the prepared read-only HID-layout inspection.

RESUMED NOVEMBER 21 by user: continue where we left off.
Repository checkpoint aded995 on feature/glorious-core-compatibility; only
unrelated .vscode/ and INFO_UF2.TXT untracked. Saved pause is revoked by explicit
resume. No hardware action, serial session, configuration query, build or capture
started during resume review. Last confirmed installed firmware is exact normal
v14 with original 400/800/1600/3200 DPI values and selected800 restored; no pending
settings recovery. V25 real DPI-value edit/restore passed (3e1089e), normal v14
restoration was user-confirmed. Full CORE app support remains unfinished.
NEXT physical readiness popup: CORE closed; Glorious wireless switch OFF and
connected directly by the working USB DATA cable, wired movement verified;
DIY receiver unchanged. Then run prepared32-bit research/inspect_wired_hid.ps1
read-only Windows HID layout inspection. No settings requests, flashing, streams
or input recording. Keep report output private; distinguish library Windows
views from raw USB capture. If unsupported, preserve partial evidence and report
limitation before choosing fallback. Use layouts for later auxiliary keyboard,
media and system report forwarding; do not enable CORE identity yet.
User preference continues until user says stop; popup/pause for intervention.
Keep context/resume prompt current, push tested milestones on feature branches.

PAUSED BY USER â€” NOVEMBER20, AFTER V25 NORMAL RESTORATION.
User selected "Pause here â€” keep normal wireless operation" at read-only wired
HID-layout readiness. STOP project work until user explicitly returns.
Exact tested normalv14 remains installed and user-confirmed working. Original
400/800/1600/3200 DPI values and selectedstage2=800 restored/readback verified;
no pending settings recovery. No live serial/capture/build/helper/timer.
Last pushed software checkpoint5b051a3; DPI-value hardware milestone3e1089e.
Prepared inspect_wired_hid.ps1 self-test PASS; no live layout inspection started.
NEXT SESSION: read newest handoff and CORE_COMPATIBILITY.md. Ask fresh readiness
for COREclosed, Glorious wirelessswitchOFF and working wiredUSBdata connection,
receiver unchanged. Then32-bit research/inspect_wired_hid.ps1 read-only descriptor
inspection. No Feature requests/settingswrites/flash/inputrecording. Keep normal
v14 installed. This supports future keyboard/media/system forwarding for CORE
key remapping. Full CORE app compatibility remains unfinished; four-hour trial
deferred. Preserve private backups/captures/addresses/binaries ignored. Keep
context and resume prompt current; push milestones on feature branches.

NOVEMBER20 â€” V25 NORMAL RESTORATION USER-CONFIRMED; WIRED HID LAYOUT NEXT.
User confirmed automatic normalv14 wireless movement/click after v25; private
core-v25-normal-restoration.json updated. Original400/800/1600/3200 stage list
and selected800 verified restored; NO pending recovery. Exactnormalv14 remains
installed, no live build/helper/capture/serial/timer. CORE stays closed.

Prepared research/inspect_wired_hid.ps1 read-only Windows/HidSharp descriptor
inspection for258A:2018. Synthetic descriptor self-test covers reportID/length,
keyboard usage and field width/count/offset. No streams opened, no FeatureSET,
settings changes, flashing or input recording. Output private; no unique device
paths printed. Reports are library Windows descriptor views, not raw USB capture;
usage lists bounded16 entries with total count, payload offsets exclude reportID.
NEXT fresh popup: switchGlorious wirelessOFF and connect directly via working
USBdata cable, COREclosed; leaveDIYreceiverunchanged. AfterReady verify own
wired collections and run32-bit research/inspect_wired_hid.ps1. Preserve partial
failure if Windows disallows descriptors; no blind firmware/driver changes.
Use actual keyboard/consumer/system field layout to prepare auxiliary USB input
forwarding, including busy retries and neutral release, before CORE keybindings.
No new configuration write is part of this step. Full CORE remains unfinished.

NOVEMBER20 â€” EXACT NORMAL V14 RESTORED AFTER V25 DPI-VALUE PASS.
Fresh bootloader readiness; pending DPI backup flag checked false before flash.
Board/exact93eb987a image hash verified. Runtime product DIY Mouse Dongle v14,
healthy mouse/CDC devices verified, no serial opened. Private evidence
core-v25-normal-restoration.json; user wireless check NEXT/PENDING.
Original profile1 list400/800/1600/3200 and selectedstage2=800 were restored and
read back before flash; no pending recovery. Hardware DPI edit milestone3e1089e
pushed. No live helper/build/capture/serial/timer. CORE closed.
NEXT brief user wireless movement/click confirmation. Then continue CORE startup
coverage: remaining setters, input forwarding and backup-aware recovery. Current
normal v14 forwards mouse reports only; full CORE support not claimed. Independent
wired USB data/Feature connection now works and all40 wired settings match radio
backup. No need repeat completed builds/flash/extraction/baselines. User untilstop.

NOVEMBER20 â€” V25 REAL DPI-VALUE EDIT AND RESTORE HARDWARE PASS.
Fresh user temporary-write readiness; 32-bit test_receiver_config.ps1
-ExerciseDpiValues completed successfully. Profile1 inactive stage4 changed
3200->3250 X=Y, complete four-stage list read back, then restored400/800/1600/3200
X=Y with full readback and original selectedstage2=800 restored/read back.
Private config-trial-20261120-211429-90d63770.json and its .backup.json:
Success/Changed/Restored true; NeedsWiredRestore false IN BOTH. Schema2 backup
pointer current-config-hid-backup.txt saved before mutation. Exact write echo
payloads checked; no pending mouse recovery. Independent wired recovery code
self-tested and data connection verified, but this trial did not need it.
V25 remains installed, pointer disabled; mouse last ON/unplugged/STILL. COREclosed.
No live serial/capture/build/helper/timer. NEXT fresh physical popup: Glorious
wired or OFF, double-reset ONLY receiver to NICENANO. Restore exact normalv14
build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
verify USB then user wireless movement/click check. Full CORE app/bindings/lighting
still unfinished. This proves configuration value transport/readback, not physical
sensor DPI calibration. User continue until stop; four-hour trial deferred.

NOVEMBER 20 â€” V25 FLASHED; PASSIVE USB STARTUP PASS; NO DPI EDIT YET.
Fresh user NICENANO readiness, CORE closed/Glorious wired-OFF. Board and exact
f577b7e0 image verified; Windows product DIY Mouse Config HID v25 healthy, mouse
and vendor Feature collections present. Private core-v25-startup.json. No serial
opened, no query or settings write during flash/startup. No live build/helper.
NEXT separate physical readiness: CORE closed, Glorious ON/USB unplugged/STILL
near unchanged receiver, other pointer available. Explain test temporarily edits
one inactive profile1 stage by50 DPI, saves originals first, then restores full
stage list and original selected800-DPI stage with readback. Pointer disabled.
After Ready run32-bit research/test_receiver_config.ps1 -ExerciseDpiValues once.
Check result and backup NeedsWiredRestore flag. If pending, stop and popup for
wired recovery (mouse wired with switchOFF, receiver unchanged), then run32-bit
helper -RestoreWired with exact pending backup. Do not flash normal before pending
mouse settings recovery is resolved. After successful edit/restore, fresh NICENANO
popup to restore exact normalv14/93eb987a, then USB and user movement/click check.
Normalv14 is currently NOT installed. All40 configuration records previously
matched wireless and wired backups; CORE full support still unfinished.

NOVEMBER 20 â€” V25 DPI-VALUE CANDIDATE VALIDATED; NOT FLASHED.
Normal exact tested v14 remains installed/user-confirmed working. Glorious last
wired with wireless switch OFF; CORE closed. Wired 258A:2018 healthy, all40 wired
backup reads passed and match wireless backup exactly. No pending recovery.
No live build/serial/capture/helper/timer. User continue until stop.

Prepared v25: same Feature/RF transport, no pointer output, prototype identity.
Adds only profile1 four-stage DPI-value setter group1/op1,18-byte payload,
big-endian X/Y50..19000 in50-DPI steps, zero padding. Exact returned setter
payload must match. Existing selected-stage writer remains restricted1..4.
Native failing test reproduced old rejection before implementation; all13 native
tests PASS after change. Shared packet parser and host helper self-tests PASS,
including ambiguous change/readback/restore/selection failures. No settings writes
occurred in preparation; hardware setter/restore not qualified yet.

research/test_receiver_config.ps1 -ExerciseDpiValues requires exact v25 USB
product. Saves schema2 original values/selection with NeedsWiredRestore=true
BEFORE mutation. Changes one INACTIVE stage by50 DPI, reads all values, then
finally restores original list, reads it back, restores selection and verifies.
Only full readback clears pending flag. On failure pause for fresh wired recovery:
32-bit test_receiver_config.ps1 -RestoreWired -BackupFile saved.backup.json;
checks saved values, mouse version and active profile1 before value recovery.
Do not blindly retry an ambiguous RF write. CORE stays closed.

Build log config-hid-v25-build.log; staged code/config hashes match current.
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v25,2HID,1ms.
50300 flash /16896 RAM;197 blocks,0x26000..0x32500 exclusive.
Preserved private image build-pairing-analysis/config-hid-v25-f577b7e0.uf2
SHA256 f577b7e095a9297da3d2fb34218fe5ba4646d1b36f2c7882041e9461f68b2426.
NEXT fresh popup CORE closed, Glorious wired/OFF, double-reset ONLY receiver to
NICENANO; verify board/exact image before flash. Check USB startup, then separate
stationary wireless mouse readiness that explains temporary inactive-stage edit
and verified restoration. Run32-bit test_receiver_config.ps1 -ExerciseDpiValues
ONLY after readiness. On failure preserve private pending backup and pause for
wired recovery before restoring normal receiver. After batch restore EXACT
normal v14 via fresh bootloader readiness, verify USB and user movement/click.
Normal rollback build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Full CORE identity/keyboard-consumer forwarding/lighting/binding writes remain
unfinished. Four-hour trial deferred. No new baseline/extraction/build needed.

NOVEMBER 20 â€” WIRED RECOVERY CONNECTION RESTORED AND READ-VERIFIED.
User confirms wired movement with wireless switch OFF. Windows sees one healthy
258A:2018 mouse with mouse/keyboard/consumer/system/vendor collections. CORE
closed; normal receiver v14 untouched. 32-bit wired backup helper completed40
read-only queries; private mouse-profile-backup-20261120-210214-9dfacd49.json.
All40 raw response records match the earlier wireless backup exactly. Current
private backup pointer now references wired copy. No settings writes occurred.
Independent wired data/Feature access is now available; actual DPI-value restore
still must be qualified. No pending recovery or live helper/serial/capture/build.
NEXT software milestone: prepare v25 narrowly scoped profile1 four-stage DPI
value edit/restore, with backup before any mutation and separate physical flash
and temporary-write readiness. CORE remains closed; no full app identity yet.
User continue until stop. Normal v14 installed and user-confirmed working.

NOVEMBER 20 â€” NORMAL V14 RESTORATION USER-CONFIRMED; RECOVERY CONNECTION NEXT.
User confirmed automatic wireless movement/click after restoring exact normal
v14 (93eb987a). Private core-v24-normal-restoration.json now records confirmation.
No live serial/capture/build/helper/timer; no pending settings recovery. Backup
milestone 06e36c8 is pushed, all 40 configuration reads passed, no settings changed.

Review confirms current normal firmware forwards mouse input only. Before CORE
button/key remapping, auxiliary keyboard/consumer/system outputs still need
implementation and neutral/busy/retry qualification. Do not claim CORE support.
Also establish a usable independent recovery connection before new profile write
trials: previous USB cable attempts yielded no present wired Glorious device.
NEXT physical popup: CORE closed; leave DIY receiver unchanged, switch Glorious
wireless power OFF, connect it directly using a known USB DATA cable. Check
whether wired movement works with wireless switched OFF. This distinguishes
actual wired data from continuing receiver input. Recheck present 258A:2018 and
65-byte Feature interface before sending any read-only verification. No flashing,
settings writes, CORE launch or change to Windows/DPI settings for this check.
If wired data remains unavailable, document the blocker and continue safe
auxiliary software work; do not assume wired restoration is available.
User continue until stop. Keep private backup/binaries ignored and context current.

NOVEMBER 20 â€” EXACT NORMAL V14 RESTORED AFTER V24 BACKUP.
Fresh NICENANO readiness, board identity and preserved 93eb987a image hash
verified before copy. Runtime product DIY Mouse Dongle v14 and healthy Windows
mouse/CDC USB devices verified. No serial opened. Private evidence:
build-pairing-analysis/core-v24-normal-restoration.json. User wireless movement/
click check PENDING next popup. V24 no longer installed; backup passed all40 reads
and remains private (milestone06e36c8 pushed). No settings changes, no pending
DPI recovery. Current original profile1 selected stage2 = 800 DPI.
No live build/capture/serial/helper/timer. Continue until user says stop.
After user control confirmation, resume CORE compatibility software work:
qualify bounded startup setters and required keyboard/consumer forwarding before
stock identity/app launch. Read docs/plans/2026-11-20-core-startup-coverage.md.
Wired USB still unresolved; do not assume wired recovery is available or start
new configuration write trials without a qualified restoration path.

NOVEMBER 20 â€” V24 WIRELESS PROFILE BACKUP HARDWARE PASS; RESTORE V14 NEXT.
Fresh stationary mouse readiness; 32-bit backup_mouse_profiles.ps1 -Wireless
completed all 40 read-only USB/RF queries successfully. Private result
mouse-profile-backup-20261120-205523-d2d97068.json; private pointer
build-pairing-analysis/current-core-profile-backup.txt updated only on success.
Raw request/reply lengths, getter opcodes, status/header/profile/slot/counts
checked. All three profiles have four stages: 400/800/1600/3200 X=Y (big endian).
Selected stages: profile1=2 (800), profiles2/3=3 (1600). All 24 slot0 binding
records preserved; DPI colours, debounce, global polling/LOD/version included.
No settings writes occurred. Full-device restoration and macro-body backup are
NOT qualified. CORE must remain closed; do not enable stock identity yet.
V24 remains installed with NO pointer forwarding. Mouse last ON/unplugged/still.
No live serial/capture/build/helper/timer. No pending DPI recovery.
NEXT PHYSICAL POPUP: Glorious wired or OFF, CORE closed, double-reset ONLY
receiver into NICENANO. Restore EXACT tested normal v14 image
build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
verify healthy USB product DIY Mouse Dongle v14, then ask wireless movement/click
check. After restoration continue CORE command coverage/input forwarding work;
no repeated extraction/build/baseline. Continue until user says stop.

NOVEMBER 20 â€” V24 FLASHED; PASSIVE USB STARTUP PASS; BACKUP NOT STARTED.
Fresh user readiness confirmed CORE closed, Glorious wired/OFF, NICENANO visible.
Verified board and exact frozen v24 hash875a85b9 before flash. Windows reports
DIY Mouse Config HID v24, healthy composite/mouse/vendor HID/CDC COM12, no errors.
Private core-v24-startup.json saved. No serial opened or configuration query sent.
Installed firmware is now V24, NOT normal v14; wireless pointer output disabled.
NEXT popup/pause: keep CORE closed, Glorious ON/USB unplugged/STILL near receiver,
other pointer available; leave receiver connected unchanged. After Ready run
32-bit SysWOW64 PowerShell research/backup_mouse_profiles.ps1 -Wireless once.
Validate 40 read-only responses. No settings changes or new write trials. After
batch restore exact tested normal v14 through fresh NICENANO readiness and user
wireless check. Continue until user stops; no live build/capture/serial/helper.

NOVEMBER 20 â€” V24 WIRELESS PROFILE-BACKUP CANDIDATE READY; NOT FLASHED.
Normal exact tested v14 remains installed and working. User reports active mouse
use; Windows still sees no Glorious wired USB device after reconnection. No wired
stream or query was opened. Keep CORE closed. Continue until user says stop.

V24 expands canonical GET coverage for the 40-query profile backup. The existing
profile-1 selected-stage setter remains restricted, but this backup operation
sends GETTERS ONLY and changes no settings. No new write trials while wired
recovery is unavailable. No pointer output in this diagnostic.
Build finished successfully: all 13 tests PASS, staged source hashes match,
RF_CONFIG_READ/HID ON, FAST_POLL OFF; product DIY Mouse Config HID v24.
197 UF2 blocks, 0x26000..0x32500 exclusive; 50188 flash / 16896 RAM.
Preserved private image build-pairing-analysis/config-hid-v24-875a85b9.uf2
SHA256 875a85b941cd15930f13b19e1a1eabacacc00c2133dcb2f8abe0db33de4d1a15.
Both backup helper self-test modes passed. No flash, settings write, serial,
capture, helper, build or timer remains running. Hardware getter coverage for
all profiles is still unqualified. Full automated profile restore is not ready.

NEXT: fresh popup/pause for CORE closed, Glorious wired or OFF, double-reset ONLY
receiver into NICENANO. Verify board and exact preserved image before flashing.
Check passive USB startup, then separate readiness for Glorious ON / USB unplugged
and STILL near receiver. Use 32-bit SysWOW64 PowerShell to run
research/backup_mouse_profiles.ps1 -Wireless. Validate all 40 responses; only full
success updates private current-core-profile-backup.txt. On failure preserve
partial evidence, do not retry blindly or change settings. After this batch,
popup/pause to restore EXACT tested normal v14 image dongle-v14-93eb987a.uf2,
SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052,
verify USB identity and ask for wireless movement/click check. No pending DPI
recovery: original 800-DPI selection previously verified restored.
CORE identity/full settings/key forwarding remain unfinished; four-hour trial
is deferred. No app launch or stock identity until required coverage qualifies.

NOVEMBER20 WIRED DEVICE STILL ABSENT; WIRELESS PROFILE-BACKUP FALLBACK PREPARING.
UserreconnectedUSB andreportsactivelyusingmouse, but present258A:2018 still0;
broaderpresentUSBcheck found noGlorious/bootloader/failedUSBdevice. No wired
HIDstream/query opened; no settingschanged. PointermaystilluseDIYnormalv14.
COREmuststayclosed. No need furthercablechanges forproposedwirelessbackup.

Extended config_request_valid ONLYgetter coverage: profiles1..3 stages/selected,
DPIcolors2/81 size19,debounce0/88 size2,globalLOD1/88,bindings3/80size10 known
8buttonIDs/slot0. Canonicalpadding/profile/button/slotguards; selectedWRITE
remains restrictedprofile1/stage1..4 (notusedbybackup). Nativegettertests first
failedoldallowlist, nowPASS; invalidslots/buttons/profiles/no writeexpansion
covered. backup_mouse_profiles.ps1 -Wireless gates exactv24product/uniqueFeature;
defaultwiredtargetpreserved. BothselftestmodesPASS; all40packetsgettersonly.
Productsource nowDIY Mouse Config HID v24; sameworkingdescriptor/prototypeIDs,
no pointeroutput. Buildstarted(logconfig-hid-v24-build.log); noflashyet.
Nextfinishbuild/config/hash/UF2 andpreservev24; updateprompt/commit/push thenfresh
NICENANOreadiness. Afterflash separateGloriousON/USBunplugged/STILL readiness,
run32-bit backup_mouse_profiles.ps1 -Wireless for40READ-onlyrequests; actual
fullprofile getter responses stillunqualified. RestoreEXACTnormalv14 afterbatch.
No settingswrites duringfallback; wiredrecoveryunavailable, do notaddwrite trials.
Normaltestedv14 remainsinstalled/userusing. PriorDPIbackupNeedsWiredRestorefalse.
Four-hourtrialstilldeferred forCORE. Usercontinueuntilstop/no cutoff.

NOVEMBER20 WIRED BACKUP ATTEMPT STOPPED BEFORE ANY QUERY: NO WIRED MOUSE PRESENT.
UserconfirmedCOREclosed/wiredreadiness, but backup_mouse_profiles.ps1 couldnot
find one wiredconfiguration interface. Present-onlyWindowsPNP showsNO VID258A
devices;32-bitHidSharptarget258A:2018 count0. No streamopened, no FeatureSET,
no settingswrite, no backupcreated. Private readinessfailure observation saved
core-profile-backup-readiness-failure.json. No livehelper/serial/capture/build.
Normalreceiverv14 remainsunchanged; lastknown800DPIrestored/no pendingrecovery.
NEXT physicalpopup: reconnectGloriousUSBdatacable atbothends directlyPC, tryknown
workingdatacable/portifneeded, keepCOREclosed andreceiverunchanged. SelectReady
oncewiredconnectionestablished; recheckpresentUSBbefore40read-onlybackup.
Do not alterfirmware orretryquerieswithoutthewireddevice. Usercontinueuntilstop.

RESUMED November20 by user after requestedprofile-backup pause. Git121fb6e,
feature/glorious-core-compatibility; only unrelated .vscode/ andINFO_UF2.TXT
untracked. Lastknownnormalv14 working; priorselectedDPIbackup confirms NO pending
wiredrecovery. No livecapture/serial/build/helper/timer; no hardware/settings
changes inresumereview. Continueuntiluser saysstop (prior preference persists).
NEXT freshpopup COREclosed + GloriousUSBconnectedWIRED; receiverunchanged.
Runprepared40-query READ-ONLY backup_mouse_profiles.ps1 afterReady, validateall
records andsaveprivatepointer onlyoncomplete. No flash/newbuild/baseline repeat.
COREidentity/fullsettings/keyforwardingremainunfinished; four-hourtrialdeferred.

PAUSED BY USER November20: selected "Pause here - keep normal wireless operation"
on wiredprofile-backup readiness popup. Resume ONLYwhenuser returns.
Normal EXACTtestedv14 (93eb987a) remainsinstalled/userconfirmedworking, original
800DPI restored; NO pending selected-DPI recovery. No serial/capture/build/helper/
timer running. No wiredprofilebackup executed; no furtherconfiguration changes.
Latestsoftwaremilestonec630442 pushed: offlineCOREstartup inventory and READ-only
backuphelper. V23 real800->1600->800 selection/restore PASS milestone4ddc36c.

NEXT SESSION: read docs/plans/2026-11-20-core-startup-coverage.md and
research/CORE_COMPATIBILITY.md, asknewphysicalreadiness for COREclosed + Glorious
USBconnectedinWIREDmode; leaveDIYreceiverunchanged. Run32-bit SysWOW64
powershell -NoProfile -ExecutionPolicy Bypass -File research/backup_mouse_profiles.ps1.
40read-onlyqueries coverglobals plus3profilesDPIstagevalues/selection/colors/
debounce and24slot0bindings. Rawbackupprivate; onlySuccessupdates
build-pairing-analysis/current-core-profile-backup.txt. No RFaddress/macrobodies/
settingswrites/flash. Validateall40responses beforeclaimingcompletebackup.
Fullprofileautomatedrestoretoolnotyetimplemented; don'tenableCOREidentityyet.

Offlineactualvendor-source factorytrace42SET/29GET (mockHID/DB, no realpackets)
showed CORE initializesall3profiles automatically. Actualsaveduserprofilesmay
vary. Private tracecore-startup-trace-1795172092145.json. Backuphelpspreserve
mouseconfiguration beforeallowlistingfullstartup commands/keyboard-consumer
forwarding/officialidentity. GeneralRGBdisabledinCOREmodel;DPIcoloursseparate.
Four-hourtrialremainsdeferred for COREappcompatibility. Fullappsettings/realDPI
valueediting/bindingsstillunfinished. No promisesofcompletionbeforeNov21.

Exact testednormalrollback: build-pairing-analysis/dongle-v14-93eb987a.uf2 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Don'treflash/rebuild/re-extractcompletedmilestones. Preserveignoredcaptures,
addresses, proprietarysource/binaries; leave .vscode/ and INFO_UF2.TXT untouched.
Keepcontext/resumeprompt current; pushmilestones onfeaturebranches. Currentbranch
feature/glorious-core-compatibility. Popup/pause forphysicalintervention.

NOVEMBER20 OFFLINE CORE STARTUP INVENTORY COMPLETE; WIRED BACKUP NEXT.
Normal EXACTtestedv14 isinstalled/userconfirmedworking; original800DPI restored,
no pendingselectedDPIrecovery. No livehelper/serial/build/capture/timer.
New research/trace_core_startup.js reuses existingprivateModelOSeries source and
factorySupportDevice record with mockHID/DB, NO realapp/nativeaddon/hardware.
InlineassertsPASS:42SET/29GET for threeprofile factoryinitialization+receiverFWread.
Private tracecore-startup-trace-1795172092145.json; counts documented in
 docs/plans/2026-11-20-core-startup-coverage.md. User profiles may differ.
COREwrites24bindings,3DPIlists,3DPIcolors,3LOD,3poll,3debounce,profile/selectedstage
onconnect. Need backup/commandcoverage/inputforwarding beforeofficialPID.
Installedpackage1.1.36, envmissingBuiltType defaults0; traceuses that codepath.

Prepared research/backup_mouse_profiles.ps1 READ-ONLY wired helper:40queries,
3profiles,24slot0 bindingrecords plusDPIstages/selection/colors/debounce andglobal
version/activeprofile/poll/LOD. Getter shapes checked againstexistingmouse dump;
NO brightnessgetter/address/bootloader/macrobodies. SelfTestPASS with40request
coverage, getter-onlyopcodes, echoes/profiles/slots/lengths, malformed rejection.
No firmwarebuild/flash needed. Backup savesprivateopaque rawreplies; onlycomplete
successupdates current-core-profile-backup.txt. Fullrestoretool notyetimplemented.
NEXT physicalpopup: closeCORE, connectGloriousUSB inWIREDmode; leaveDIYreceiver
unchanged. Then32-bit backup_mouse_profiles.ps1, validate40responses andsave
resultsprivately. No settingswrites; normalreceiverremainsv14. Afterbackupcontinue
COREallowlist/keyboard-consumer-forwarding implementation; do not launchCOREyet.
Usercontinueuntilstop; nocutoff. Four-hourtrialstilldeferred.

NOVEMBER20 normalv14 USER CONFIRMED after v23;800DPI restored/no pendingrecovery.
No livecapture/serial/build/helper. Next autonomousstep: offline CORE startup
command inventory using existingprivateModelOSeries.js anddefaultdevice record,
mockHID only (no app launch/no realpackets/no DBwrites). Need exact startup
profile/setting commandset before enablingofficialappidentity. Currentprototype
transport only5reads+selectedstagewrite; COREdetection/fullDPIvalues/bindings not
implemented. Four-hourtrialremainsdeferred. Usercontinueuntilstop/nocutoff.

NOVEMBER20 EXACT NORMAL V14 RESTORED after v23 DPI-stage trial.
FreshNICENANO/mousewired-OFF readiness; unique board/exact93eb987a hashverified
beforecopy. USBproductDIY Mouse Dongle v14 and onehealthymousecollection verified.
No serialopened. Private core-v23-normal-restoration.json; wirelessusercheck
PENDINGpopup. V23 nolongerinstalled; currentmouseoriginal800DPI restored byradio
readback beforeflash. Private DPIbackupNeedsWiredRestore=false; NO RECOVERYneeded.
No livecapture/build/helper/timer. Hardwaremilestone4ddc36c pushed: real800->1600
->800 selectedstagewrite/restore PASS. FullCOREidentity/settingsUI notyetdone.
Afterbriefwirelesscheck continueCOREtransport scope, no repeatedbaseline/extraction.
Four-hourtrialstilldeferred; usercontinueuntilstop/no cutoff.

NOVEMBER20 FIRST REAL WIRELESS DPI SELECTION WRITE/RESTORE PASS.
UserexplicitReady for temporaryDPIstagechange, COREclosed/mouseONunpluggedSTILL.
Prepared -ExerciseDpi helper Success true; freshbaseline profile1/1000Hz/stage2,
selectedstage3 readbackconfirmed, originalstage2 readbackrestored.800->1600->800.
Nine successfulUSB/RFtransactions inthistrial; priorfiveREAD-onlysameboot had
passed. DpiTrialChanged/Restored true; NeedsWiredRestore false in resultANDbackup.
Private resultconfig-v23-20261120-183851-c34067f2.json, .backup.json; privatepointer
current-config-hid-backup.txt pointsbackup. CHECKflag oninterruptedresume but
thistrialhasNO PENDING RECOVERY. DPIstagevalues/lighting/bindings notmodified.
No serial/livehelper/capture/build/timer. V23 remainsinstalled, pointerdisabled;
mouse lastON/unplugged/still. Selectedstagewrite isrealmouseconfiguration,
not receivergain; no sensorDPIcalibration orfullCOREappsupport claim.
NEXT restore EXACTtestednormalv14 viafreshbootloaderpopup (COREclosed,mousewired/
OFF,double-resetONLYreceiverNICENANO). Image93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052. VerifyUSB,
thenuserwirelessmovement/clickcheck. Afterrestoration continueCOREcompatibility
scope; actualDPIvalue writes/fullprofile/bindings/lighting stillnotimplemented.
Four-hourtrialstilldeferred; usercontinueuntilstop.

NOVEMBER20 V23 FIVE READ-ONLY CONFIG QUERIES PASS; NO SETTINGS WRITES.
Initialhelper invocation hit reservedPowerShellPID variable BEFORE AddType/HIDopen
orquery. Renamed to mouseVendorId/mouseProductId and movedassignments before
SelfTest so runtime setup ischecked; offlineSelfTestPASS, sameauthorizedREAD-only
retryPASS withoutnewflash/reboot. Firmwareunchanged ee7f5a7a.
Private resultconfig-v23-20261120-183450-80b8c5f4.json: Success true, five ordered
transactions Version/Profile/Polling/DpiStages/ActiveDpi; firmware1.0.0.0,
activeprofile1,1000Hz, stages400/800/1600/3200 X=Y, selectedstage2=800DPI.
ExerciseDpi false;NeedsWiredRestore false; no backupneeded/created. No serial,
no livehelper/capture/build. V23 stillinstalled, pointeroutputdisabled.
Reusablehosttransactions/replyrelease gate now nominallyhardwarevalidated.
NEXT separatepopup readiness for TEMPORARY selectedDPIstage change/readback and
verifiedoriginalrestore via test_receiver_config.ps1 -ExerciseDpi. COREclosed,
mouseON/unplugged/STILL, otherpointer; receiverunchanged (no resetneeded).
Hostfreshreads/backup BEFORE write; expectedstage2->3->2 ifunchanged. Onfault,
checkprivatecurrent-config-hid-backup.txt NeedsWiredRestore and pauseforWIRED
recovery BEFORE normaluse. Restoreexacttestedv14 93eb987a afterbatch viafresh
bootloaderpopup thenuserwirelesscheck. No generalDPIvalue/lighting/bindingwrites.

NOVEMBER20 V23 NOW FLASHED; USB STARTUP PASS.
FreshuserNICENANO/COREclosed/Gloriouswired-OFF readiness. Verifiedunique board/
bootloader0.6.0 and exactee7f5a7a imagehash beforecopy. ProductDIY Mouse Config
HID v23; twohealthyHIDcollections andCDC, zeroerrors. Private startuprecord
config-hid-v23-usb-startup.json. NO FeatureSET/query/DPIwrite/serialopened.
No livecapture/build/helper. Installedv23 intentionallyNOwirelesspointeroutput.
NEXT separatephysicalpopup: COREclosed, GloriousON/USBunplugged/STILL nearreceiver,
otherpointeravailable, receiverunchanged. Run32-bit test_receiver_config.ps1
WITHOUT -ExerciseDpi for READ-ONLY fivequeries first. OnlyafterPASS askseparate
readinessfor temporaryselectedDPIchange/readback/originalrestore. No timecue.
Beforeanywrite persistbackupNeedsWiredRestore; onfailure checkprivatepointer
current-config-hid-backup.txt and usefreshwiredrecovery. RestoreEXACTtestedv14
93eb987a afterbatch withseparatebootloaderpopup andwirelesscontrolcheck.
Usercontinueuntilstop; no currentcutoff. Four-hourtrialstilldeferred.

V23 software milestone397f156 pushed; no live sessions or pending recovery backup.
Normal testedv14 remainsinstalled/userconfirmed. V23 stillNOTflashed. Nextstep
is freshNICENANO readiness, then verifiedv23flash and READ-ONLY multi-querytrial;
temporaryDPIselection mustwaitfor separate readiness afterreadsPASS. EOF-only
protocol-helper tidy doesnotchange testedlogic. No completedbuild/test repeats.

NOVEMBER20 V23 SOFTWARE READY, NOT FLASHED/HARDWARE-TESTED.
Normal EXACTtestedv14 remainsinstalled and userconfirmedworking afterv22.
No livebuild/serial/capture/helper/timer; NO realmouse settingswrites yet.
Frozen private config-hid-v23-ee7f5a7a.uf2; SHA256
ee7f5a7a220ba7a6c56c40f7a6bf1bc1db9f7ffac058d37a3777094cec148dbf.
Product DIY Mouse Config HID v23;196blocks;0x26000..0x32400;50084flash/16896RAM.
Stagedsources/config/quietflags/twoHIDs/localIDs2FE3:0008/1ms checked; fastpollOFF.
13nativechecksPASS (reusable SETs, reply-release/GET-consumption gates, dynamic
reply/selection echo, wrap, timeoutlockout, normal/probe paths). Hostnewhelper
SelfTestPASS incl busyGETwithoutSETretry, stage/backup bounds and actual trial
workflow faults: restoreattempt after read/writefailure; failedrestore remains
pending. Original wiredreader SelfTestPASS after sharedprotocol extraction.

Canonicalallowlist: version, activeProfile00/85, polling, profile1DPIstages and
selectedstage READs; onlyWRITE profile1 selectedstage1..4. No generalDPIvalues,
lighting/bindings/updater/address writes. COREcannotattach toprototypeIDs.
Main ownsRF;4sdeadline; failedtransactionquarantinesSETuntilreboot; completed
reply mustbe released bymousequeue advance and consumed byhostGET before reuse.
Vendor descriptor isunchanged from hardware-testedv22. Pointeroutputdisabled.
New research/test_receiver_config.ps1 defaultREAD-only. -ExerciseDpi backs up
freshselectedstage BEFORE change, triesalternateexistingstage, readbacks and
restoresoriginal in finally, verifiesrestore. Saved .backup.json and pointer
build-pairing-analysis/current-config-hid-backup.txt markNeedsWiredRestore=true
untilverifiedrestoration; unfinishedrecoveryblocksnewwrites. -RestoreWired uses
freshwiredreadiness, boundedprivatebackuppath andfixedstagecommand only. On
interruptedtrial CHECKthispointer/pendingflag BEFORE anyotherconfigurationwork.

NEXT commit/push thenfreshbootloaderpopup: COREclosed,Gloriouswired/OFF,
double-resetONLYreceiver intoNICENANO. VerifyuniqueBoard+exactv23UF2 thenflash,
checkUSBproduct/BOTHhealthyHIDs/65byteFeature (noqueryyet). Separate mouseON/
USBunplugged/STILL readiness; run32-bit test_receiver_config.ps1 WITHOUTswitches
for FIRST READ-ONLYfivequery hardwarecheck. OnlyifreadsPASS askseparatereadiness
for TEMPORARYselectedDPIchange+restore using -ExerciseDpi. No timedpowercue or
serialreader. Ifwrite/restorationfails, pauseforwiredrecovery usingbackup BEFORE
normalwirelessuse; do not reusequarantinedreceiver. Restore EXACTtestednormalv14
93eb987a hash93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
afterbatch withfreshbootloaderpopup andusercontrolcheck. Four-hourtrial deferred.
Usercontinueuntilstop; no currentdeadline. FullCOREsettings stillunimplemented.

NOVEMBER20 V23 REUSABLE CONFIG SOFTWARE IN PROGRESS; NORMAL V14 INSTALLED.
User confirmednormalwirelessrestorationafterv22. No realsettingswrite yet.
Plan docs/plans/2026-11-20-reusable-config.md; usercontinueuntilstop, nocutoff.
Changed config_read/config_hid: canonical version, activeprofile(op00/85), polling,
profile1DPIlist/selected-stage READs; onlyWRITE allowed profile1 selectedstage1..4.
Full64-byte canonicalpadding validation; real replyheader/paddedlength validation;
mouse dataqueue mustADVANCE afterfinalreply beforepublishing; hostGET mustconsume
publishedreply before nextSET. 4sdeadline/failurequarantine untilreboot. Tests
firstfailedfornewAPI, thenfocusedcodec/mailboxtestsPASS; includeswriteecho,
sequencewrap, busy/noresend, release/consumption gates andtimeoutlockout.
Newshared research/mouse_settings_protocol.ps1 extractedfromwiredreader; original
read_mouse_settings.ps1 remainsREAD-ONLY andSelfTestPASS. New
research/test_receiver_config.ps1 defaultREAD-only, -ExerciseDpi selectedstage
change/readback/restore, -RestoreWired savedbackup recovery. HostselfchecksPASS,
includingoneSETwithbusyGETpolls,failure,selectecho,stagebounds,backupbounds.
Beforeanywrite helperrecords .backup.json NeedsWiredRestore=true and private
current-config-hid-backup.txt; clears ONLYafterrestore+readback. Priorpending
recoveryblocksnewwrites; pathbounded toprivateanalysis. Ifrestorefails, require
freshwiredreadiness and -RestoreWired; no retries against quarantinedreceiver.
Check backup pointer/pendingflag onanyresumedinterruptedtrial before otherwork.

Build v23 currentlyrunningexec34288/logconfig-hid-v23-build.log onAppconfig-hid.
Product DIY Mouse Config HID v23, localprototypeIDs; COREcannotattach; pointer
output stilldisabled. No serial/capture/hosthelper live. Sourcehasnotbeenflashed.
NEXT finishbuild/config/UF2/hash checks; update docs/resumeprompt, commit/push;
thenfreshbootloaderpopup (COREclosed/mousewired-OFF). Firsthardware READ-ONLY
multi-requestcheck afterseparateON/unplugged/still readiness. OnlyafterreadsPASS
askreadyforTEMPORARYselectedDPIstagechange andverifiedrestore. Exactnormalv14
93eb987a rollback remains. Four-hourtrial deferred; COREfullsettings notyetdone.

NOVEMBER20 normalv14 restoration USER CONFIRMED afterv22PASS. No live sessions.
Preparing reusable config stagingv23 under prototypeID, no pointeroutput:
canonical version/polling/DPIreads plus bounded profile1 selectedDPIstage1..4.
Plan docs/plans/2026-11-20-reusable-config.md. Add finalRFreply release barrier,
hostGET consumption gate,4s deadline andfailurequarantine before repeatedSETs.
Hosttrial will save originalstage BEFORE anyselected-stage write, markwired
recovery needed untilradio restore/readbackconfirmed, withwiredfallback after
freshreadiness. Newhardwaretrial must explicitlymentiontemporaryDPIselection.
COREstaysdisconnected; no generalsettings/lighting/binding/updater forwarding.
Workingnormalv14 staysinstalled throughoutsoftwareprep. Usercontinueuntilstop.

NOVEMBER20 EXACT NORMAL V14 RESTORED after successful v22 end-to-end read.
FreshNICENANO/mousewired-OFF readiness; board+exact93eb987a frozenimage verified
beforecopy. USBproductDIY Mouse Dongle v14 and normalmousecollection healthy.
No serialopened. Private recordcore-v22-normal-restoration.json; physicalwireless
restorationcheck PENDINGpopup. V22 no longerinstalled. No livecapture/build/helper.
V22successmilestone623acb7 pushed: correctedvendorhealthy,65byteFeature, oneSET/
twoGETs returnedrealRFfirmware1.0.0.0matchingwired. No settingschanged.
Afteruserwirelesscheck continue multi-command/readback/COREtransport software;
four-hourtrialstilldeferred. User sayscontinueuntilstop, no timecutoff.

NOVEMBER20 V22 HOST USB->RF->USB VERSION READ PASS.
UserconfirmedGloriousON/USBunplugged/STILL, COREclosed. 32-bit preparedhelper
finished successfully, one FeatureSET and twoGETs(total includesinitialA3).
Returned firmware1.0.0.0 exactlymatcheswiredreference; Success=true.
Private observation config-hid-v22-20261120-175941-a3145cd2.json. No serial,
no settings write, no livehelper/capture/build/timer. V22 remainsinstalled with
pointeroutputdisabled; one-shotqueryused. This establishes vendorFeature host
roundtrip throughreceiver tostockmouse, not fullCOREdetection/configwrites.
NEXT freshphysicalpopup: Gloriouswired/OFF, COREclosed, double-resetONLYreceiver
intoNICENANO; restore EXACT TESTED normalv14 frozen93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
VerifyUSB thenbriefuserwirelesscheck. Afterrestoration continue multi-command/
allowlist/CORE compatibility implementation. Four-hourtrialstilldeferred.

NOVEMBER20 V22 NOW FLASHED; WINDOWS VENDOR STARTUP PASS.
UserfreshNICENANO/COREclosed/Gloriouswired-OFF readiness; verifiedunique board,
bootloader0.6.0 and exact9cb33d52image hash beforecopy. USBproduct
DIY Mouse Config HID v22; BOTH mouse/vendor HIDcollections healthy, CDC/COM12,
zeroerror devices (v21Code10 gone thisboot). Private config-hid-v22-usb-startup.json.
No serial opened, no FeatureSET/RF query/settings write. No livebuild/helper.
Currentinstalledv22 intentionallyNOwirelesspointeroutput. COREmuststayclosed.
NEXT physicalpopup: GloriousON/USBunplugged/STILL nearreceiver, otherpointer
available; receiverunchanged. No timedcue needed becauseUSBhost triggersquery.
Then32-bit read_receiver_version.ps1 exactv22gate; initialGETA3, ONEfixedSET,
boundedGET polls, compareactual RF version1.0.0.0withwiredreference, saveprivate.
Afterquery/failure restoreexactTESTEDv14 93eb987a viafreshbootloaderpopup and
briefwirelesscheck. Four-hourtrialstilldeferred. Continueuntiluser saysstop.

NOVEMBER20 V22 CORRECTION BUILT/VALIDATED; NOT FLASHED/HARDWARE-TESTED.
Normal exacttestedv14 is installed and user-confirmed working. No livebuild,
serial/capture/helper/timer. No settings writes. Continue until user saysstop.
Preserved failedv21; descriptorcorrected to known20-byte vendorfeature descriptor.
Newsemanticregression failedoldbeforefix and now passes; all13nativechecksPASS,
hosthelper SelfTest and32-bitCIM/Pnp commandavailabilityPASS.
Private frozen config-hid-v22-9cb33d52.uf2; SHA256
9cb33d5224c8eb517ab8a3e00f27c007e58c2ddd6c3ffdc4e7c95f6c73991523.
Product DIY Mouse Config HID v22;194blocks;0x26000..0x32200;49572flash/16768RAM.
Stagedsource/config/flags verified: twoHIDs, local2FE3:0008,1ms, quiet, read+HID
flagsON, fastpollOFF. One fixedversionREAD/boot, no pointeroutput, noCOREidentity,
no settings/updater writes. Build/test doesNOT prove Windowsstartupfix yet.
NEXT physicalpopup: COREclosed,Gloriouswired/OFF,double-resetONLYreceiver to
NICENANO. Verifyboard+exactv22hash,flash,thencheck BOTH HIDcollections healthy
(noCode10) andCDC. Ifhealthy, separate GloriousON/unplugged/STILL readiness,
then32-bit research/read_receiver_version.ps1 (v22gate), oneSET/boundedGET polls,
compareversion1.0.0.0wired. No timedpowercue or serialreader. Ifstartupfails,
noquery; restorev14 andinvestigate. Restoreexacttestedv14afterbatch via fresh
physicalpopup: frozen93eb987a hash93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Do not repeat extraction/completedbuild/baseline or flasholdv21. Four-hourtrial
stilldeferred for CORE; fullappsettings remainunimplemented.

NOVEMBER20 v21 rollback USER CONFIRMED; v22 correction BUILD IN PROGRESS.
Normal exacttestedv14 remains installed, automaticwireless userconfirmed after
v21rollback. Private restoration record updated. No serial/capture/queryrunning.
New semantic descriptor test FAILED on oldv21beforefix; it checks collection
usage, per-Main local Usage consumption,64x8-bit Feature, noReportID, balanced
collection/truncation and wrongreportcount. Corrected descriptor matches known
working mouse/stockreceiver20-byte descriptor. All13native checks PASS, host
helper SelfTest PASS after exactproductgate updatedto DIY Mouse Config HID v22.
Read-only RF/USB behavior unchanged; onlydescriptor, product label andhelpergate
changed. No settings writes. Hardware fix unproven until enumeration/trial.
Buildrunningexec98660/logconfig-hid-v22-build.log in existingconfig-hidSDKstage.
Preserve failedv21image; afterbuild verifyflags/UF2/hash, save distinctv22image,
update docs/commit/push, then freshbootloaderpopup. No oldv21reflash/queryretry.
Currentuser sayscontinueuntilstop; no timecutoff. Restoreexactv14afterv22batch.

NOVEMBER20 NORMAL V14 RESTORED after v21 USB vendor failure.
Fresh userNICENANO readiness; verified unique bootloader/board and exact frozen
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 beforecopy.
USB product DIY Mouse Dongle v14 and normal mouseHID healthy. No serial opened.
Private record core-v21-normal-restoration.json; physicalwirelesscheck PENDING.
No capture/build/helper/timer. V21 is no longer installed. No mouse settings
changed. NEXT popup: unplug GloriousUSB, turnON, try movement/click withoutarm.
Aftercheck continue descriptorfix/regressionsoftware prep for distinctv22;
oldv21vendor Code10 likelymissinglocalUsage but correction not yet hardwareproven.
Current user preference: continue until toldstop. Four-hour trial still deferred.

NOVEMBER20 V21 FLASHED; VENDOR USB INTERFACE FAILED - RESTORE V14 NEXT.
User confirmedNICENANO/COREclosed/mousewired-OFF and continue until toldstop
(no current time cutoff). Verified unique board/bootloader and exacta7631b8d
image hash beforecopy. Product DIY Mouse Config HID v21 enumerates; mouseMI_02
and CDC/COM12 healthy, vendor USB Input Device MI_03 Code10, ProblemStatus
3221225657 (0xC00000B9). No vendor HID child created. Host version helper NOT RUN;
no FeatureSET, RF query, serial session or settings write. No live sessions/build.
Private failure observation: config-hid-v21-usb-startup.json.

Static comparison found v21 vendor descriptor lacks local Usage before Feature;
known working mouse/stockreceiver descriptor includes Usage00 inside collection:
06ffff0900a10109001500250175089540b102c0 (20bytes). Omitted field usage is likely
cause, not yet corrected/hardware-proven. Current test checked descriptor size/
prefix only and missed it. After normalrestoration, add a failing semantic
local-usage/report-length regression, correct descriptor (preserve oldv21image),
use distinctv22product/hash and validate/build before separate physicaltest.
Microsoft primary HID value-capability docs support per-main-item usages;
Windows Code10 alone does not prove the exact cause. No driver replacement.

CURRENT INSTALLED v21 has NO wireless pointer output, vendor interface broken.
NEXT PHYSICAL POPUP: keep Gloriouswired/OFF, COREclosed; double-reset ONLY
receiver intoNICENANO. Restore EXACT tested normalv14 frozen93eb987a SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052,
verifyUSB product and fresh userwirelesscheck. Do not reuse sourcecontrol image.
Do not retry a v21 query against failedHID or repeat extraction/baselines.
Four-hour trial remainsdeferred, CORE compatibility is thepriority.

RESUMED November20 ~17:34Manila by explicit user request. Supersedes saved pause.
Rechecked saved v21 a7631b8d and exact tested normalv14 93eb987a image hashes;
both match. Git10b0f8b, feature/glorious-core-compatibility; only unrelated
.vscode/ and INFO_UF2.TXT untracked. No rebuild/extraction/flash/settings change.
No serial/capture/build/helper/timer running. Last known installed normalv14,
user-confirmed working. V21 remains NOT FLASHED/HARDWARE-TESTED.
Earlier05:00AM break/04:50 flash cutoff expired; ask new break time, do not assume
an ongoing deadline. NEXT popup: COREclosed, Gloriouswired/OFF, double-resetONLY
receiver intoNICENANO. Verifyidentity/hash and flash exactv21, passive USBcheck;
separate GloriousON/unplugged/STILL readiness before hostFeature query. V21 has
NO pointer output and one fixed version READ/boot; no CORE detection/settings
writes. Afterquery restore exacttestedv14 with physicalreadiness and controlcheck.
Four-hour trial remains deferred for CORE compatibility.

PAUSED BY USER November20 before05:00AM Manila. User explicitly selected
"Pause here - test v21 next session". Resume only when user returns.
Normal exact hardware-tested v14 remains installed and user-confirmed working.
V21 is BUILT/VALIDATED BUT NOT FLASHED/HARDWARE-TESTED. Software milestone3a13236
pushed on feature/glorious-core-compatibility. No capture/serial/build/timer/
helper running, no physical intervention requested now, no settings changed.

Next session read docs/plans/2026-11-20-usb-config-read.md and
research/CORE_COMPATIBILITY.md. Ask fresh bootloader readiness for receiver only,
Glorious wired/OFF and CORE closed. Verify unique NICENANO board and saved image:
build-pairing-analysis/config-hid-v21-a7631b8d.uf2; SHA256
a7631b8dcae26221b13bf1e1d397f0f5f93dcb795c767a553bb67f9beabf22df.
Product DIY Mouse Config HID v21; localUSB2FE3:0008; twoHID instances; no pointer
output; one exact version READ perboot. No CORE identity or settings writes.
13native checks, host-helper self-test, staged-source/config/UF2 validation PASS.
After flash/USBcheck, separate readiness: GloriousON/USBunplugged/STILL near
receiver, another pointer available. No timed power-on cue/serial reader needed.
32-bit SysWOW64 powershell -NoProfile -ExecutionPolicy Bypass -File
research/read_receiver_version.ps1 drives one FeatureSET and boundedGET polls.
Compare real RF response to wired1.0.0.0; save observation privately. Restore
EXACT testedv14 dongle-v14-93eb987a.uf2 / SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 via separate
bootloader popup, then brief wireless restoration check. Do not substitute any
normal software-control build. No completed builds/extraction/baselines repeated.

Earlier v20 RFread PASS:8fragment attempts,16replybytes, firmware1.0.0.0 equals
wired; exactv14 restored. Wired settings1000Hz, stages400/800/1600/3200, active800.
Four-hour trial deferred for official CORE compatibility; separate app fallback.
General lighting disabled in installed CORE model record; DPI colours separate.
The05:00AM pause is now honored. Ask new available time on a later resumed session;
old04:50 flash cutoff is not authorization to flash later while project paused.

NOVEMBER20 V21 SOFTWARE READY; BREAK BY05:00AM MANILA (no flash at/after04:50).
Normal exact testedv14 remains installed and user-confirmed; no serial/capture/
build/helper/timer running. V20 RF version-read PASS saved5bda374, normalrestore
confirmed817851a. Four-hour trial remains deferred for CORE compatibility.

New -App config-hid v21 is BUILT/UF2-VALIDATED, NOT FLASHED/HARDWARE-TESTED.
Private frozen config-hid-v21-a7631b8d.uf2; SHA256
a7631b8dcae26221b13bf1e1d397f0f5f93dcb795c767a553bb67f9beabf22df.
Product DIY Mouse Config HID v21;194blocks;0x26000..0x32200;49572flash/16768RAM.
Two HID instances, vendor Feature64bytes usageFFFF/0000 plus CDC, local2FE3:0008
identity (CORE WILL NOT DETECT IT). No pointer output. EXACT firmware-version
read only, once/boot. No command sent before host Feature SET. USB callbacks
queue/snapshot; main processes/provides real RF reply, busyA0/failureA2.
Arbitrary settings/bootloader requests and repeat SET rejected. v20 automatic
read path still separate; v21 only host-triggered. No settings writes implemented.
Native13 tests PASS including real RF-codec through USB callback/mailbox and five
shared probe build modes (normal/probe/receiver/v20/v21). Host read_receiver_version.ps1
SelfTest PASS. Source/staged equality, quietflags, bothRFflagsON, fastpollOFF,
localUSBIDs,1ms interval verified. No normal rebuild needed: conditional off-path
native checks passed; hardware-tested rollback remains EXACT93eb987a frozenimage.

NEXT pending physical readiness popup if BEFORE04:50: COREclosed, GloriousWIRED
orOFF; double-reset only receiver into NICENANO; verifyboard+v21image thenflash.
After USBcheck, separate readiness: GloriousON/USBunplugged/STILL nearreceiver,
otherpointer available. No timed power-on cue needed (host triggersquery).
Run 32-bit powershell -NoProfile -ExecutionPolicy Bypass -File
research/read_receiver_version.ps1. It checks exactv21 identity/uniqueFeature,
GETinitialA3, sends ONE fixed SET, pollsGET up to10s, savesrawreplyprivately.
No CDC reader/capture window or p/x command needed for that helper. Restore EXACT
TESTED normalv14 SHA93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
using separate bootloader readiness and brief wirelesscheck afterward.
If cutoff reached or user pauses, leave normalv14 installed and resumev21later;
5AM deadline must not be bypassed. Do not repeat extraction, completedbuilds,
versionbaseline or normalrestoration. See docs/plans/2026-11-20-usb-config-read.md.

NOVEMBER20 SOFTWARE CHECKPOINT IN PROGRESS; USER STOP BY05:00AM MANILA.
Normal testedv14 is installed and user confirmed restored wireless control.
Preparing -App config-hid v21: second vendor HID FFFF/0000/64-byte Feature,
local prototype IDs2FE3:0008 (NOT stock258A:2031), pointer output suppressed.
Exact fixed version read accepted once perboot. USB callbacks validate/enqueue
and snapshot; main claims/publishes under IRQ lock and uses proven RF codec.
No RF command before host SET; arbitrary settings/updater writes rejected.
All13 native checks PASS including new USB/mailbox/real-codec sequence test and
shared host-trigger ACK gate; host read_receiver_version.ps1 self-test PASS.
Build running in execsession65373/logconfig-hid-v21-build.log; stagedsource
CMake/main/discovery/configHID/configRead/config matched repo when inspected.
No v21 flash yet. No serial/capture/helper/timer. No settings changed.
Next finish build/config/UF2 and preserve private hash, update docs, commit/push.
Do not start physical batch too near05:00; exactv14 rollback93eb987a remains.
New host script uses65-byte Windows Feature API against exact v21 identity, one
SET then bounded10sGET polling; no serial. Mouse can beON/stationary beforequery,
so no timed power-on cue needed. Hardware interface behavior remains unverified.

NOVEMBER20 RESTORATION COMPLETE: user confirms automatic wireless movement/click
works normally on exact tested v14 after v20. No capture/serial/build running.
Next software milestone: native USB Feature-report to RF read-only bridge using
local prototype IDs first; CORE cannot connect to this staging identity. Existing
CORE native code matches VID/PID and HID usage, NOT interface number, so CDC may
remain while adding a separate vendor HID collection; no need to remove CDC or
reorder original mouse interface just to test transport. Reject settings/update
commands during this read-only stage; app initialization can write profiles.
No existing user settings changed; four-hour trial remains deferred.

NOVEMBER20 EXACT NORMAL V14 RESTORED after successful v20 RF read.
User confirmed NICENANO readiness/mouse wired-OFF. Verified board and exact
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052 frozen v14
before copy; USB product DIY Mouse Dongle v14 verified afterward. No serial
opened. Private record: core-v20-normal-restoration.json. Physical wireless
movement/click restoration check PENDING popup; no recording running.
No build/serial/helper/timer/capture remains. No settings writes occurred.
V20 hardware read milestone pushed5bda374: firmware1.0.0.0 equals wired,8fragment
attempts,16replybytes,complete/no failure. V20 is no longer installed.
After restoration check, continue CORE-compatible vendor transport preparation
on feature/glorious-core-compatibility; keep four-hour trial deferred. No need
to repeat version baseline/build/flash. CORE detection/settings writes remain
unimplemented. Ask fresh physical readiness for any future diagnostic flash.

NOVEMBER20 WIRELESS CONFIG READ PASS - CURRENT INSTALLED V20, STOPPED.
User confirmed OFF/unplugged readiness, then ON/still after capture-running cue.
One-reader research/capture_config_read.ps1 completed; x at35s then frozen d.
Private capture: config-read-v20-20261120-043100-1f1efba5 (.json/.txt).
Success=true, started1, offset64, attempts8 (eight fragments), reply16bytes,
complete1, failed0; firmware1.0.0.0 exactly matches wired reference. No movement
output/settings write/CORE vendor interface. This validates nominal RF config
read roundtrip only, not general settings writes/retry stress/full CORE support.
No live serial/capture/build/helper/timer remains. V20 is stopped by x and still
installed; user last confirmed Glorious ON/unplugged/still. Keep CORE closed.
NEXT PHYSICAL PAUSE: Glorious wired/OFF, double-reset RECEIVER into NICENANO;
verify unique board and restore EXACT TESTED normalv14 frozen93eb987a UF2:
SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Do NOT restore source-built normalcontrol6877579c. Verify USB product normalv14
without serial; then separate user check that wireless automatic control works.
After restoration continue CORE vendor transport work from this successful read
reference. Four-hour trial remains deferred; app compatibility before soak.

NOVEMBER20 V20 FLASH/USB STARTUP VERIFIED - overrides installed-v14 notes below.
User confirmed NICENANO visible, CORE closed, Glorious wired/OFF. Verified unique
D: NICENANO, Board-ID nRF52840-nicenano, UF2bootloader0.6.0 and exact saved v20
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632 before copy.
V20 NOW INSTALLED: DIY Mouse Config Read v20, COM12, healthy USB enumeration.
Private startup record: build-pairing-analysis/config-read-v20-usb-startup.json.
No serial opened, no live capture/build/helper, no settings writes, no mouse RF
trial yet. This diagnostic has NO wireless pointer output. Keep CORE closed.
NEXT pending physical popup: Glorious OFF/USBunplugged, other pointer available,
receiver unchanged. After Ready start capture_config_read.ps1, confirm RUNNING
before cue to turn mouse ON and leave STILL. Fixed read only; x35s/d, close40s.
After trial or failure ask bootloader readiness and restore EXACT TESTED v14
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052, not normal
software-control6877579c. Four-hour trial remains deferred for CORE work.

RESUMED November 20 by user; supersedes November19 PAUSED status.
Re-read handoff/CORE findings/plan. Git7899ecf on feature/glorious-core-compatibility;
only unrelated .vscode/ and INFO_UF2.TXT untracked. Rechecked preserved v20 SHA256
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632 and exact
hardware-tested v14 rollback SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Both match. No rebuild, flash, settings change or serial session. No background
capture/build/helper/timer. Last known installed normalv14; current mouse/cable
state needs fresh readiness. NEXT pending popup: keep CORE closed, mouse wired/OFF,
double-reset only receiver into NICENANO. After Ready verify board and copy exact
v20, passive USB check, then separate stationary wireless read-query readiness.
V20 has NO wireless pointer output and only a fixed firmware-version READ.
Restore testedv14 after this diagnostic batch. Four-hour trial remains deferred.

PAUSED AT USER REQUEST - November 19; resume only when user returns.
No physical intervention requested now. No serial/capture/build/timer/helper is
running. Normal TESTED v14 remains installed (93eb987a frozen image), unchanged.
Mouse last confirmed WIRED, Glorious CORE closed. No settings changed.

Latest completed steps: wired READ-only reference succeeded: firmware1.0.0.0,
1000Hz setting, profile1 stages400/800/1600/3200, active stage2=800DPI.
Private observation: build-pairing-analysis/core-wired-settings-reference.json.
Built/UF2-validated v20 READ-only RF diagnostic; native12 checks pass including
actual ACK/no-HID/x-stop integration, plus capture-parser self-test. It sends
only one fixed firmware-version query; no pointer output, settings writes,
CORE-compatible vendor interface or mouse bootloader/updater commands.
Private frozen: config-read-v20-68f92a56.uf2; SHA256
68f92a56a7297cd20c97b52c2a032b358709f03c757877e223733d1490429632.
Product DIY Mouse Config Read v20;191blocks;0x26000..0x31F00;48828flash/16512RAM.
NOT FLASHED/HARDWARE-TESTED. Source/config metadata and logs ignored.
Normal isolation rebuild also PASS, experimental flagOFF; normalcontrol SHA256
6877579cdeb4bfd5587894f7e238dcf31eb1c6ddcfce0ddf0ecf3704c5ca82c7,
46188flash/16064RAM. That control is NOT hardware-tested and NOT the rollback.

On resume: read research/CORE_COMPATIBILITY.md and November19 CORE plan. Ask
popup for v20 flash readiness: Glorious wired/OFF, receiver double-reset into
NICENANO. Verify board and exact preserved v20 hash before copy; check passive
USB startup. Then separate mouse-OFF/unplugged readiness and capture-running
cue for research/capture_config_read.ps1 (one reader, no p, x35s/d frozen summary,
closes by40s). Mouse should stay STILL; diagnostic has no wireless pointer output.
Compare radio version reply with wired reference; no speculative config writes.
Restore EXACT TESTED v14 (93eb987a) after trial or failure, using separate physical
readiness. Keep CORE CLOSED throughout v20; it does not support CORE detection.
Do not repeat completed extraction/build/baseline. Working branch:
feature/glorious-core-compatibility. Four-hour trial remains deferred; user
prioritizes official CORE compatibility before soak. Separate app fallback.
Deadline target before November21 is not guaranteed. General lighting disabled
in installed CORE's model record; DPI indicator colours separate.

CORE diagnostic preparation in progress after successful wired readback.
Added separate -App config-read (v20), opt-in RF_CONFIG_READ_DIAGNOSTICS flag.
It suppresses ALL wireless pointer forwarding, sends only a hardcoded 64-byte
firmware-version READ via eight offset fragments, retries identically until the
mouse advertises the next command sequence, caps attempts at512, reassembles
bounded type7 reply records, rejects gaps/malformed/status mismatch, and exposes
a frozen summary only after x then d. No vendor identity/config interface yet.
New codec offline tests pass; actual shared ACK/no-HID/x-stop integration passes;
existing full native12 tests passed before latest mock-only test expansion.
research/capture_config_read.ps1 parser self-test passes, fixed identity gate,
one reader, x at35s/d then close by40s; no p/settings commands. Not run live.
Build v20 currently running (exec session67476) after removing only disposable
CMakeCache.txt: initial CONFIG_ flag conflicted with Zephyr's reserved Kconfig
prefix; renamed RF_CONFIG_READ_DIAGNOSTICS. Prior cache line edit left a parse
error, so cache regenerated. No usable image claimed yet; do NOT flash until
build/config/UF2 validated. No serial/capture open. Normalv14 still installed.
Next: finish build, verify normal-mode build isolation, preserve validated v20
image/hash privately, update docs/commit/push, then popup for receiver bootloader
readiness. Physical query and restore exact v14 each need separate cues.

November 19 WIRED READBACK PASS (no settings changed): four fixed queries returned
A1/matching commands. Firmware 1.0.0.0; polling divider1=1000Hz; profile1 stages
400/800/1600/3200 (X=Y), active stage2=800DPI. Earlier CORE UI saved1600 was not
current device readback. Raw replies ignored: core-wired-settings-reference.json.
Glorious is wired/CORE closed by user readiness; receiver remains normalv14.
Next autonomous work: bounded radio READ-query codec/diagnostic; no settings
writes or identity impersonation. Four-hour trial remains deferred.

NOVEMBER 19 CORE COMPATIBILITY MILESTONE - overrides older pending steps.
User explicitly prioritizes Glorious CORE compatibility BEFORE the four-hour
trial. Scope is genuine DPI/button customization through receiver and 1 kHz;
separate app is fallback. Target before November 21, not guaranteed.
Branch: feature/glorious-core-compatibility. Normal tested v14 stays installed
and working. No live serial/capture/build/helper/soak/timer. No new flash or
mouse settings change. Private selected installed CORE sources are ignored.
Found CORE's exact Series One PRO device record and settings JS. LEDSetting=false
means this installed app disables general lighting for the model; DPI indicator
colours have a separate command. CORE detection can WRITE stored profiles on
initialization: do not expose a partial compatible receiver to the running app.
Static mouse firmware path reassembles type7 RF records into the USB settings
mailbox. Command sequencing/replies still need recovery and hardware testing.
Prepared research/read_mouse_settings.ps1: four allowlisted READ queries only;
32-bit PowerShell offline request/parser/malformed-field self-tests PASS. No live
query yet. Refuses running CORE/ambiguous mouse, bounded busy polling, strict
A1/echo/bounds checks. Existing HidSharp reused; no new dependencies.
NEXT PHYSICAL PAUSE: close Glorious CORE; connect Glorious USB in WIRED mode,
leave DIY receiver unchanged, select Ready in popup. Then run read-only helper,
save raw observation privately, inspect summarized version/polling/profile1 DPI.
If helper assumptions fail, stop and revise from evidence; no guessed writes.
Read research/CORE_COMPATIBILITY.md and docs/plans/2026-11-19-glorious-core-compatibility.md.
Keep four-hour test pending. No more baseline/build/flash/extraction needed now.


CURRENT OVERRIDE - November 19, resumed; supersedes older PAUSED/app-deferred notes.
User wants completion before November 21: genuine mouse DPI/lighting/keybind
customization THROUGH the DIY receiver, plus 1 kHz polling. User now asks whether
we can reuse Glorious CORE instead of building a separate app. Investigate that
compatibility route first; no promise of completion or unsupported lighting.
Normal tested v14 remains installed/working. No capture, serial, build, timer or
four-hour soak is running; no hardware/configuration changes in this review.
CORE already recognizes the wired mouse, but DIY receiver lacks its vendor
configuration interface/protocol. Renaming USB identity alone is insufficient.
Read-only installed app.asar inventory found shipped mouse protocol JavaScript,
hiddevice native source and device databases. Next: inspect these existing files
for Series One PRO detection and settings commands, then determine what USB/RF
bridge is required. Keep proprietary extracts/binaries private and ignored.
Existing live 1 ms USB interval and 964.9 movement events/s guided trial are
positive host evidence; fresh 1 kHz sensor cadence/physical latency unverified.
No further baseline/reflash/firmware dump needed at this point. Pause by popup
for physical intervention. Deadline is a target, not a guaranteed capability.

PAUSED / ACCOUNT-INDEPENDENT HANDOFF READY - 2026-11-19

The user explicitly paused and wants to resume under a different OpenAI account.
Stop project work; no four-hour reliability window has started. The30-minute
preliminary gameplay check PASSED by user report: smooth throughout, no issues or
interruptions, all five buttons and wheel actually used. Milestone e79e7de is
pushed. Manual diary only; no input/health recording or objective latency monitor.

Known installed firmware is exact tested normal v14 93eb987a, automatic and
user-confirmed working. No restoration/reflash is needed. No agent capture,
serial reader, build, timer service or background monitor is running.

ACCOUNT_HANDOFF.md and RESUME_PROMPT.md are ready for a new account/chat.
Account handoff checkpoint9983beb is pushed on feature/timed-host-polling-measurement.
On samePC/sameWindowslogin, retain the whole project folder and reopen it after
signing in with the other account. Start a new chat using RESUME_PROMPT.md rather
than depending on account/chat migration. Another computer also needs the ignored
private data and external toolchain; Git alone is insufficient. No auth copying.

NEXT ON EXPLICIT RESUME: read the handoff and ask readiness/available time for a
SEPARATE four-hour ordinary wireless-use window on unchanged firmware. Start a
new private manual diary only after readiness; log actual active use/idle breaks,
sleep, charging, resets/reconnects and issues. An early/interrupted run is not an
uninterrupted4h pass. Do not repeat extraction/builds/flashing/completed baselines.
Long4h/8h/multiday reliability and physical latency remain unverified. Companion
app/DPI/lighting writes deferred. Keep private captures/addresses/binaries ignored,
unrelated .vscode/ and INFO_UF2.TXT untouched; pause popups for physical steps.

GAMEPLAY30min COMPLETION REPORTED / OBSERVATIONS PENDING - 2026-11-19 19:07 Manila

User selected completed30minutes. Completion report received about11:07UTC,
more than30minutes after recorded10:35:22UTC cue, consistent with the requested
manual duration. Do not mark a pass until issue/continuity and controls reports
arrive. Private diary current-gameplay-diary.txt points to gameplay-normal-v14-
20261119-183522-482615bf.json, updated with actual report time/wall elapsed.

Two observation questions pending: smooth/no stalls/disconnects/overshoot/stuck
buttons or interruptions, and controls actually used. Game/mode optional. No
input/health recording, serial, build or automated game interaction ran. Normal
v14 remains the known installed automatic receiver; no firmware changes.
NEXT collect observations, mark only tested controls/user-reported duration,
save/push preliminary result and concise handoff.30min is not4h/8h soak or physical
latency proof. No further physical trial starts without fresh readiness.

GAMEPLAY START CUE / AWAITING USER PLAY AND REPORT - 2026-11-19 18:35:22 Manila

User confirmed game loaded and30minutes available, GloriousON/USBunplugged with
normal receiver and otherpointer ready. Unique USB productDIY Mouse Dongle v14
verified without serial. Known installed tested image remains93eb987a.
Private manual diary: build-pairing-analysis/gameplay-normal-v14-20261119-183522-482615bf.json
Start cue recordedUTC 2026-11-19T10:35:22.181590+00:00; planned30min endUTC 2026-11-19T11:05:22.181590+00:00
(about 19:05:22 Manila). User starts their own30min manual timer.
This records the cue, not independently observed play. Actual duration/controls/
issues must come from the user afterplay. No input recording, serial, automated
gameplay, health monitor, build or timer service is running. Normal mouse operation
continues indefinitely; diary ending does not stop the receiver.

NEXT: send start/completed-or-stop popup; await actualuserplay. If returned early,
record actualduration/reason; do not claim30min pass based only on selection.
Collect game/mode optional, controls actually tried, stalls/disconnects/overshoot/
stuckbuttons and interruptions. A30min pass is preliminary, not longsoak or physical
latency qualification. Currentbranchfeature/timed-host-polling-measurement.
Keepcontext/resume current and push milestone after actualresults. No firmware,
settings, driver, game automation or companion app work is needed duringplay.

RESUMED / GAMEPLAY READINESS NEXT - 2026-11-19 18:07 Manila

The user explicitly resumed after the November18 pause. The preliminary gameplay
trial has NOT started. Normal v14 exact93eb987a is the last verified installed,
user-confirmed automatic receiver. No flash/build/serial/capture is planned.
Physical connection and game readiness must be confirmed after the break.

NEXT: ask fresh readiness for a30-minute familiar practice/offline game check,
GloriousON/USBunplugged, receiverconnected and otherpointeravailable. The readiness
answer confirms available time for this bounded trial; do not reuse expired daily
cutoffs or start before readiness. Then record startUTC and a manual30-minute
expected end; no raw recording, game automation or continuous health monitor.
Collect actual duration, controls tried and issues. Early stop is not a fullpass.
Normaloperation continues after diary ends. Keep settings unchanged.

Saved checkpoint da2bcff and result e3ffb03 are pushed on
feature/timed-host-polling-measurement. No completed measurements need repeating.
Maintain this context and RESUME_PROMPT at readiness/start/result changes. Pause
with question popups for physical steps. Companion app/settingswrites deferred.

PAUSED AT USER REQUEST — 2026-11-18 19:37 Manila

The user selected “Pause here — gameplay check later.” This supersedes the earlier
open-ended work window. Stop project work after saving this handoff; resume only
when the user asks. No gameplay trial, timer, capture or soak has started.

Installed firmware remains exact tested normal v14 (93eb987a), automatic wireless
operation without serial, arming or a timed limit. It is user-confirmed working.
No live serial reader, recording, build, UAC helper or background monitor remains.
The receiver may continue ordinary use; no restoration or reflash is needed.

Latest tested milestone e3ffb03 is pushed on feature/timed-host-polling-measurement:
normal-wireless larger-motion delivery averaged 964.9 movement events/s across the
entire predefined 20 seconds, 916–993 each second, median gap 1.000ms. User confirmed
continuous, smooth motion with no lifts/issues. Live full-speed HID interval 1ms
and active-pipe matching were independently verified. These support near-1kHz HOST
movement delivery in this test, not sensor freshness, physical bus cadence or
end-to-end gaming latency. Rare 15.279ms host gap remains unexplained.

NEXT ON EXPLICIT RESUME: read docs/plans/2026-11-18-gameplay-qualification.md and
ask readiness/available time for a preliminary 30-minute familiar practice/offline
game session on unchanged normal v14. Only then record start time/manual diary;
no game automation, raw recording, setting changes or continuous health monitor
is implied. Record actual duration/controls/issues; an early stop is not a full
pass. No further firmware baselines, builds or flashes are needed for this step.
Longer 4h/8h/multiday reliability and physical latency checks remain unfinished.
Companion app, DPI and lighting writes remain deferred until gaming optimization.

LARGER WIRELESS HOST DELIVERY PASS / GAMEPLAY READINESS NEXT - 2026-11-18 19:26 Manila:
Session57635exit0,allguidedphases/focus/closedCSVchecksPASS. Privatev14-timed-motion-
20261118-192310-503bc56c.summary.json: fixed20s19298nonzeroupdates/964.9Hz,
persecond916..993,median1000us,p95=1392,p99=2047,max15279us,gaps>2ms406.
UsercontinuousSMOOTH/no lifts/issues. Wiredlarge842.85Hz/median1007us/noissues.
All4guidedresults retained privateguided-motion-comparison-20261118.json +tables.
Initialwireless3zeroeventsecondsdidNOTrecurwithtargetactivation/largermotion;
originalcausestillunknown, no retroactivelate-startorRFfaultclaim. Supportsnear1k
HOSTdeliveryinthisrun, notsensorfreshness/USBtoken/end-to-endlatency/perfectjitter.
Rare15.279mshostgapremainsunexplained. ActualUSBfullspeed1msactivepipeverified.
Normalv14exact93eb installed/automatic, GloriouswirelessONUSBunplugged; no live
capture/serial/build/helper. No newfirmware/settingschange. Useruntilsaystop.
NEXT pushlarger-motionmilestone thenphysicalgameplayreadiness: useropenfamiliar
practice/offlinegamemode, normalwireless+otherpointer. Plan docs/plans/2026-11-18-
gameplay-qualification.md; proposed30min manualdiary, NO capture/gameautomation/
inputinjection/continuoushealthmonitor implied. Onlystartafterready; earlystop
actualdurationrecorded.30minnotlongsoakqualification. App/DPIwritesdeferred.

WIRED LARGER MOTION PASS / MATCHED WIRELESS CAPTURE RUNNING - 2026-11-18 19:24 Manila:
Wiredsession59703finishedexit0:privatewired-timed-motion-20261118-192030-00e9c914.
Fixed20s16857events/842.85Hz,760..923/s,median1007us,p95=2044,p99=2210,max6271,
gaps>2ms1866. UsercontinuousSMOOTH/no lifts/largercircles. Thisrosefromsmallwired
623.45Hz; motionmethodaffectseventrate, notphysicalpollfrequencyproof.
UsermatchingwirelessONUSBunpluggedsameDPI/surface/largercirclesready. Session57635
basebuild-pairing-analysis/v14-timed-motion-20261118-192310-503bc56c nowrunning.
No serial/flash/settingschange; normalv14exact93ebactive. PopupactivatewithGlorious
itself/countdown5/MOVE30largecontinuous/STOP5 sent. Samevalidatedbinary06f74472,
controller-Source receiver. NEXTpoll57635 closedcompleteanalysis/userobservation;
comparesamefixed20s withwiredlarge. Thenifbothhostratesbelow1k, inspectlower-level
USB/hostcapturepathbeforeRFclaims (don'tequateeventHzwithUSBpollHz). Useruntilstop.

WIRED LARGER-MOTION GUIDED CAPTURE RUNNING - 2026-11-18 19:21 Manila:
Userwired/roomforlargercircles/samesetupready. Session59703 base
build-pairing-analysis/wired-timed-motion-20261118-192030-00e9c914. Samevalidated
binary06f74472 andcontroller-Sourcewired. Windowready180shardlimit,popupactivate
withGloriousitself/5scountdown/comfortable3–5cmcontinuouscirclesMOVE30s/STOP sent.
No firmware/settings/serialchanges; normalv14receiverunchanged. NEXTpoll59703,
closedcompletefixedcentral20sanalysis, collectcontinuity/smoothness/surfacecontact
observation; thenmatchedwirelessmotionifuseful. Useruntilsaystop. No1kclaimyet.

WIRED SMALL-CIRCLE REFERENCE USER-CONFIRMED / LARGER-MOTION CHECK NEXT - 2026-11-18 19:06 Manila:
UserwiredcontinuousSMOOTH, mostlySMALL/CONTROLLEDcircles. Privatewired-timed-motion-
20261118-190223-853da817.summary.json updated. Fixed20s12469wiredmoveevents/
623.45Hz,328..879/s,nozeroseconds,median1024us,p95=3031,p99=5063. All18103raw
rowswiredonly, noerrors/focusloss. Firstwireless553.9Hz/3initialzeroseconds with
usercontinuoussmoothreport stillunexplained. Lowmeanmovementeventsalsoonwired;
notRFfaultproof. NEXT scopedlargersteadycircle WIREDtrial (alreadywired): target
activateswindowitself,keepmouseinhand, comfortable~3–5cmcirclesifroom/sensoronpad,
no pauses/lifts/clicks/wheel,DPIunchanged. Same5/30/5window/fixed20sanalyzer. Then
matchmotionwirelessly tocheckwhetheroriginalgap/ratepersists. No firmwareflash/
settingschange/newcode/build. Normalv14receiverunchanged, no livecapture/serial.
Useruntilsaystop; physicalreadinesspopupbeforecapture. Save/pushwiredreference.

WIRED GUIDED REFERENCE COMPLETE / MOTION DETAIL PENDING - 2026-11-18 19:04 Manila:
Session68661exit0, nativephases/focus/closedCSVguardPASS. Privatewired-timed-motion-
20261118-190223-853da817.summary.json. Fixedcentral20s12469movementevents/623.45Hz,
counts[830,764,879,751,841,821,707,685,635,633,565,530,599,520,471,665,328,434,373,438],
nozeroseconds;median1024us,p95=3031,p99=5063,max184256,gaps>2ms2708,nocoalesce0.
Wiredalso below1kmovement-eventavg: averagealonecannotdiagnosewirelessRFfault.
UserfollowedwiredGUI; popupnowpendingcontinuoussmooth+small/slowvslarger/brisk/
pause-lift details. Needreportbeforeinterpretation. Normalv14receiverunchanged,
Gloriouswirednow;no livecapture/serial/build. NEXT likelynormalwirelessretryusing
GLORIOUSITSELFactivation (nohandoff) andcontinuouscomfortablelargercircles sensor
onsurface, tocheckoriginalinitialgapunder matchedprep. Ifcurrentwiredtinycircles,
nonzeromotion cadence/hand/hostcoalescingconfounds1kclaim. No trimming/causalRF
lossclaim/firmwaretweak. Useruntilsaystop. Save/pushresultwhenobservationarrives.

GUIDED WIRED REFERENCE RUNNING - 2026-11-18 19:02 Manila:
Userwired/samesetupreadinessconfirmed. Session68661 base
build-pairing-analysis/wired-timed-motion-20261118-190223-853da817. Controller-Source
wired verifiesunique258A:2018root/sourcehashes; noserial/receiverchange. Visible
windowready180shardlimit; popupactivateusingGLORIOUSITSELF,releaseclick,keepitinhand
through5scountdown/30sMOVE/5sSTOP sent. Normalv14 receiverunchanged, noflash.
NEXTpoll68661; controllerwaitsclosedcompletecapture thenfixedcentral20sanalysis.
Collectusercontinuous/focus/feelobservation andcomparewithwirelessfirsttrial;
no posthoctrimming/nofreshsensor/physicalbusclaim. Useruntilsaystop.

FIRST GUIDED WIRELESS RESULT UNRESOLVED / WIRED READINESS NEXT - 2026-11-18 18:59 Manila:
UseransweredcontinuousfromMOVEstart,motionfeltSMOOTH. DoNOTassumelate/pause.
Privatev14-timed-motion-20261118-184803-b718bd7e passedphases/focus/closedCSV:
fixed20s11078events/553.9Hz, first3secondszero, later286..809/s, median1028us,
p95=2972,p99=4187. Recheckedall13930rawrows receiverONLY, wired0; absoluteRF
rows5..7s(beforefocus)then24..47s; MOVEstart16.108s. No rawerrors/focusabort.
Thisdoesnotalignwithusercontinuoussmoothreport; preserveboth, nocausalRFclaim,
no trimming/bestepoch. Normalv14exact93eb STILLinstalled/automatic; no live
capture/serial/build. Controllernow-Source receiver(default) orwired; unique258A:
2018root forwired,normalproductguardforreceiver; sourcepassedtoanalyzer.18analysis
checks/controllerSelfTest/syntaxPASS. SAMEtimedbinary06f74472, C/headerunchanged,
legacy09d2preserved. Needpushresult/softwarethenphysicalwiredreadinesspopup.
NEXT GloriousUSBwiredmode,sameDPI/surface/Windowssettings; receiverunchanged;
startguided -Source wired. UseractivateswindowwithGLORIOUSITSELF (pre-epochclick),
keepsitinhandthrough5scountdown,30sMOVE/5sSTOP, no clicks/wheelinsideMOVE. This
reducespointerhandoffambiguity; comparehostpathwithoutfirmwarechange. No sustained
fresh1kHz/physicalbuslatencyclaim. Useruntilsaystop; nofixedcutoff.

GUIDED CAPTURE COMPLETE / USER MOTION CONTINUITY QUESTION PENDING - 2026-11-18 18:50 Manila:
Session80325completedexit0; allnativephases/focus/RAWDONE/closedCSVanalysisPASS.
Privatev14-timed-motion-20261118-184803-b718bd7e.summary.json. Fixedcentral20s:
11078movementevents/553.9Hz, persecond[0,0,0,286,626,683,713,696,678,701,747,652,
809,748,676,716,752,510,577,508], median1028us,p95=2972,p99=4187,max98262,
2378gaps>2ms,no_coalesceflag0,stationaryrows0. DO NOTcallthissustainedrateproof:
threeinitialseconds noevents; useransweredfollowedwindow,butcontinuity/timing
popupnowpending (late/pause/lift vscontinuous smooth vsfreezes). Needuseranswer
beforeinterpretation/retry. No posthocbestepochselection. Userwindowuntilsaystop.
Normalv14exact93eb unchanged/active; no serial/capture/buildlive, noflashneeded.
NEXT iflate/pause confirms, recordlimitedtrialandfreshguidedretryphysicalreadiness;
ifcontinuous, scopedwiredcomparison toseparatehost/motion/toolvswirelessissue.
HardwareGUIguidedtimingqualified; sustainedfresh1kHz/physicalbuslatencyNOTqualified.

GUIDED NORMAL V14 MOTION CAPTURE RUNNING - 2026-11-18 18:48 Manila:
UserGloriousONUSBunplugged/samesetupreadiness. Session80325, base
build-pairing-analysis/v14-timed-motion-20261118-184803-b718bd7e. Normalv14product/
validatedtimedbinary/source/headerchecked. Visiblewindowready180shardlimit; no
serial/arm/flash/powercycle/x. Popupactivatewindowfollow5scountdown/30sMOVE/5sSTOP
sent. Windowwaitsactivation; focuslossinvalidates; automaticallyclosesafterguided
phase. Normalmouseinputcontinuesafterrecording. NEXTpoll80325; controlleranalyzes
ONLYclosedcompletecapture, private20sfixedepochsummary; collectusercontinuous
motion/focus/pauses/feel thenreportlimitedhostdeliveryevidence. No1kHzclaimyet.
Abortbyclosingcapturewindow (resultinvalid); no automaticretry. Normal93eb remains.

TIMED HOST MOTION SOFTWARE READY / PHYSICAL READINESS NEXT - 2026-11-18 18:46 Manila:
Branchfeature/timed-host-polling-measurement. Normalv14exact93eb stillinstalled/
autoactive/userconfirmed; no livecapture/serial/build. Guided --timed-motion Cmode
waitsfocus->5scountdown->30sMOVE->5sSTOPautoclose, immediatefocuslossabort, native
phase/oldrawselftestsPASS. Newprivatecapture_raw_mouse_timed.exe SHA256
06f74472976c207f4565991d8c1cc309fdc2aa3e9d211749ea716282896bd982.
Source/header/binaryhashvalidationJSON saved; legacycapture_raw_mouse.exe09d2
UNCHANGED. CSV/legacyCLI/1200scap preserved.17analysis testsPASS incl6motioncases;
controllerSelfTest/syntaxPASS. Analyzerfixedcentral20s all20secondcounts+gapstats,
refusespartial/error/background/absolute/controls, no peakcherry-picking/freshsensor
orphysicalbusclaim. Controlleronlynormalv14, hashchecks,180shardcapture limit,
waitscompleteclosurethenanalyzes. User untilsaystop; no morefirmwareflashesneeded.
NEXT pushsoftware, physicalpopupnormalreceiverconnected/GloriousONUSBunplugged,
sameDPI/surface/otherpointer; startvisibleguidedcapture thenpopupactivatewindowand
followitscountdown/MOVE/STOP, keepfocus/no clicks/wheel. No power-oncue/arm/xstop;
normalinputkeepsworkingafterrecording. Validateusercontinuousmotion/focus andsave
result. GUI/liveepoch hardwareunprovenuntiltrial. Privatecaptures/binariesignored.

TIMED HOST MOTION MEASUREMENT SOFTWARE PREPARATION - 2026-11-18 18:38 Manila:
USBdescriptor/activepipe1ms verification7e23c76 pushed. Newbranchfeature/timed-host-
polling-measurement; normalv14exact93eb remainsactive/userconfirmed. No live
capture/serial/build. Addguided --timed-motion toexistingrawsource, legacyCLI/CSV/
1200scapunchanged. Separatetimedbinarypreservesoriginal09d2 foroldcontrollers.
Waitfocus->5scountdown->30sMOVE->5sSTOPautoclose, timestampmarkers/focuslossguard.
Analyzerpredefinescentral20s, all20per-secondcounts+gapdistribution, no bestepoch
selection/freshsensor/physicalbusclaims. Plan docs/plans/2026-11-18-timed-host-motion.md.
Needstate/parser behavioralselfchecks, sourcebuild/nativeoldselfchecks, software
pushBEFOREphysicalmotionreadinesspopup. No firmwareflash/settingschanges.

LIVE USB1ms CONFIGURATION VERIFIED / SUSTAINED HOST MEASUREMENT NEXT - 2026-11-18 18:34 Manila:
Normalv14exact93eb installed/autoactive/userconfirmed; no livecapture/serial/build.
ReadonlynativeUSBinspector builtwithinstalledMSVC/SDK, /W4/WX compileandhardwarefree
selftestPASS. ActualreadPASS:fullspeed,HIDinterface2,interruptIN0x83,maxpacket16,
bInterval1,activepipe matches; both2presenthubsreadable,exact1localtarget. No
serial/reset/SET/vendor/driver/setting/firmwarechanges. Privateusb-endpoint-
verification.json source/binaryhashes+normal-v14-usb-endpoint.json. Tooldeliberately
verifiesonlyfullspeedinterval1; rejectsothersinsteadofguessingWindowsmapping.
This confirmsconfigured1ms/target1000Hz, NOTmeasuredfreshreportfrequency orphysical
latency. NativeparserstubfailedfirstthenvalidationPASS; source/buildhelper/plan
newonfeature/usb-endpoint-polling-verification. Needpushmilestonebeforefuturework.
NEXT scopednormalv14foregroundsustained-motion measurement withpredefinedtimed
activeepoch; currentmixedcontroltrials cannotqualifysustainedpolling. Inspect/reuse
existingrawlogger/analyzers, retain CSVcompatibility and1200scap, noinputinjection.
Thenuserphysicalreadinesspopup. No firmwareflashneeded; no moreABBArepeats.
UserUNTILTHEYSAYSTOP. Companionapp/DPIwritesdeferred; longgamingsoakstillpending.

READ-ONLY USB ENDPOINT INSPECTOR BUILT/SELFTESTED - 2026-11-18 18:30 Manila:
Fourrunphasecomparison+normalrestore milestone0a7c14e pushed. Branchfeature/usb-
endpoint-polling-verification. Normalv14 userconfirmedinstalled, no livecaptures/
serial/build. Newresearch/inspect_usb_endpoint.c withbuild_usb_endpoint.ps1 uses
installedMSVC/WindowsSDK; onlySetupAPI+hubGET_NODE_INFORMATION/GET_CONNECTION_EX/
GET_DESCRIPTOR, noreset/SET/vendor/serial/injection. No newdependencies orflash.
Parserstub failedvalidfixture, thenfullselftestPASS; MSVC/W4/WXcompilePASS.
Validatesbounds/fullspeed/currentconfig/uniqueHIDalt0interruptIN/activepipematch,
rejectsambiguity/truncation/zero fields. JSONonlypublicendpointfields,nohubpaths/
serials/RFaddresses. Toolbinary/obj privateignored. NEXT readonlylivechecknormal
USBproduct, runinspector andsaveJSON; ifsuccessdocumentactualinterval/speed, NOT
measuredfreshreportfrequency/gaminglatency. No newphysicalreadiness neededforread.
Useruntilsaystop. Needcontext/resumeconsolidationandtestedmilestonepushaftercheck.

PHASE ABBA COMPLETE / NORMAL CONFIRMED / USB POLLING VERIFY NEXT - 2026-11-18 18:23 Manila:
Userconfirmed restorednormalwirelessworks. InstalledEXACTnormalv14 93eb987a,
autoactive/noarm/notimedstop. No livecapture/reader/build/helper. Fourruntables
savedresearch/plan; privatephase-comparison-abba-20261118.json. A1/B1/B2/A2 all
counts/contentmatch21818/19376/22994/29967, alluser smooth/cleanstopped. FASTradio
RFsubmitmeans33.196/33.192us vsbaseline50.774/52.084us (~18.235usstagegain).
Phaseweightedbootmeans572.265vs547.562us difference24.703us descriptiveONLY;
100uswithinbin/manualworkload/two-bootlimitations andtail/backpressure prevent
reliablewholegaminggainclaim. No fastnormalpromotion. HistoricalA2tworecordcause
unknown. SameactualUSBconfiguration1ms target1kHz; 50/10us isCPUradio-checkwait.
NEXT read-onlyliveUSBendpointinterval/speed verification onnormalv14, thenfocused
sustainedmotion/polling+longgamingreliability checks. No firmwareflash orrepeat
baseline needed now. UsercontinueUNTILSAYSTOP,no fixedcutoff. Companionapp/settings
writesdeferred. Keepprivatecaptures/addresses/binariesignored; unrelatedfilesalone.

NORMAL V14 RESTORED AFTER PHASE ABBA / USB VERIFIED - 2026-11-18 18:21 Manila:
UserNICENANOwired/OFFreadinessconfirmed; exact93ebSHA/BoardID/S140/181blockUF2
verified andrestored. ProductDIY Mouse Dongle v14 uniqueUSB/CDC/HID verified
WITHOUTserial. Private normal-v14-restored-after-phase-abba.json. InstalledNOW
normalv14autoactive/notimedstop/noarm; NOTv18/v19. No capture/reader/buildlive.
NEXT briefuserautomaticwirelesscheck, save/pushfourruntables/qualifiedlimits and
concisehandoff. Userasked50usmeaning: explainedreceiverCPUwaitbetweenRFchecks,
separatefromUSB1msinterval/target1kHz andtotalgaminglatency. Useraskedcurrentstep:
wrappingexperiment/restoringnormal; nextsustainedgamingreliability andfresh1kHz
measurement beforefurtheroptimization. Useruntilsaystop. No fastpollpromotion.

FOUR-RUN PHASE ABBA COMPLETE / NORMAL RESTORE NEXT - 2026-11-18 18:19 Manila:
A2session25390 finished100s exit0/raw29967read_errors0/failed0. All3frozenframes,
calibrated1/invalid0, accepted29967=hostALLforeground;dx-247/dy-88/wheelnet0,
all5buttons3down/up, no aggregate/countdifference. User smooth/cleanstopped/noissue.
PrivateA2v18-latency-20261118-181513-15c1edc9.summary.json. All4runsinputmatch.
A1/B1/B2/A2 RFsubmitmean50.774/33.196/33.192/52.084us: repeatable~18.235us
receiverstagegain. RFUSBglobal565.837/427.624/427.069/505.845us. A1phaseweights
565.837/553.031/542.093/578.692us; conditionbootmeans572.265vs547.562us (~24.703us
Badvantage) but100uswithinbinskew/manualworkload/twoindependentbootseach prevent
causalphysicalgainclaim. Fastp99histupper1119/1113us vsbaseline1000/1000us; tail
notuniformlybetter. Busy0.592/1.134/0.293/2.943%. B1invalid1explicitlyexcludedfrom
validphaseweighting; otherinvalid0. Privatephase-comparison-abba-20261118.json.
NEXT physicalNICENANOwired/OFF restoreEXACTtestednormalv14 93eb thenUSBverify+
briefuserwirelesscheck. Installedv18 STOPPEDbyx; no livecapture/reader/build.
No newbuilds/firmwarechanges. Useruntilsaystop. No normalfastpromotion/physical
latency/sustained1kHzclaim. Needpush4runresultandfinalnormalhandoffafterrestore.

V18 A2 FINAL BASELINE CAPTURE RUNNING - 2026-11-18 18:15 Manila:
UserOFF/unplugged/samesetup readiness confirmed. UseraskedpurposeA2: explainedrepeat
50usbaseline AFTERtwo10usruns checksrepeatability/USBphase/workload drift; not
newfirmware. Session25390 basebuild-pairing-analysis/v18-latency-20261118-181513-
15c1edc9. OneCDCreader+100sforegroundraw,noarm,d50quiet/x70/frozen3frames. ON/
controlsmotioncuepopup sent. Expectedfinish~18:17. Abortbase.stop-request. NEXT
poll25390; onlyanalyzeAFTERcapture.success/raw_exit0/closedCSV. Collectfeel/stop,
restoreEXACTnormal93eb viafreshphysicalreadiness, verifyUSB/useroperation; then
fourrunphase-awareconclusion/push. Installedv18 untilrestore. Useruntilsaystop.

V18 A2 SAVED BASELINE FLASHED / INPUT READINESS NEXT - 2026-11-18 18:14 Manila:
UserNICENANOreadywired/OFF. Exactsavedv1805eaf02a/BoardID/S140/199blockverified,
flashed; productDIY Mouse Phase v18 uniqueUSB/CDC/HID verifiedWITHOUTserial.
Privatev18-a2-flash-verification.json. Installedv18autoactive/listening, notnormal
v14. No livecapture/reader/build. NEXT separateOFFUSBunpluggedsameDPI/surface/
otherpointerreadiness; A2same100sforegroundtrial noarm,d50quiet/x70/threeframes.
Gateanalysisoncapture.success/raw_exit0 andclosedCSV. ThenrestoreEXACTnormalv14
93eb beforefurtherwork, verifyUSB/useroperation; analyze4runphase-awarecomparison.
No newbuilds/code; useruntilsaystop. No fastpromotionclaim.

V19 B2 PASS / V18 A2 FLASH READINESS NEXT - 2026-11-18 18:12 Manila:
Session19970 FINISHEDexit0,raw22994read_errors0/failed0. InitialanalysisreadCSV
beforeclose andsawpartialline; rerunAFTERcapture.success/raw_exit0 fullyvalidates,
no corruptedcapture orfirmwarefault. Alwaysgatefutureanalysisoncapturecompletion.
Calibrated1/invalid0; accepted22994=host22994ALLforeground,dx164/dy-24/wheelnet3,
all5buttons3down/up; content/countdifferences0. User smooth/cleanstopped/noissue.
Privatev19-latency-20261118-180904-51e58c81.summary.json. RFhandler23.180us,
RFsubmit33.192us,RFUSBcallbackmean427.069us/max1113/p99upper1113us,busy146.
Installedv19 STOPPEDbyx; no reader/capture/buildlive. NEXTfreshNICENANOwired/OFF
flashEXACTsavedbaselinev18 05eaf02a, verifyUSBproduct, separateOFFunpluggedinput
readiness,A2same100strial. Thenrestoreexactnormal93eb andverifyuseroperation.
No intermediate normalflash/newbuild. Useruntilsaystop. No fastpromotionclaim.

V19 B2 CAPTURE RUNNING - 2026-11-18 18:09 Manila:
UserOFF/unplugged/samesetupready. Session19970, base
build-pairing-analysis/v19-latency-20261118-180904-51e58c81. OneCDCreader+100s
foregroundraw, noarm,d50quiet/x70/frozen3frames. ON/controlsmotionpopup sent.
Expectedfinish~18:10:50;abortbase.stop-request. NEXTpoll19970/analyze/userfeel,
thenfreshNICENANOwired/OFF flashEXACTsavedv18A2, separateinputreadiness/trial.
Restoreexactnormalv14 afterbatch/abort. Installedv19 untilthen; no newbuilds.

V19 B2 SAVED IMAGE FLASHED / INPUT READINESS NEXT - 2026-11-18 18:08 Manila:
UserNICENANOreadywired/OFF; exactsavedv19 8ad6dd03/BoardID/S140/199blockverified,
flashed andUSBproductDIY Mouse Phase Fast v19 withuniqueCDC/HID verifiedwithout
serial. Privatev19-b2-flash-verification.json. Installedv19autoactive/listening;
NOTnormalv14. No livecapture/reader/build. NEXT separateOFFUSBunpluggedsamesetup
inputreadiness thenB2capture100s noarm/d50quiet/x70/threefrozenframes. AfterB2,
flashsavedv18A2 throughfreshphysicalreadiness (nointermediate normalflash), then
restoreexactnormalv14 afterbatch orabort. No newbuilds/codechanges.

B1 RESULT SAVED / NORMAL CONFIRMED / REVERSE REPEAT NEXT - 2026-11-18 18:07 Manila:
Userconfirmed restorednormalwireless works. Installedexactnormalv14 93eb autoactive;
no livecapture/reader/build/helper. V19B1pass19376exactcounts/content, calibrated1,
invalid1inexplicitbucket, user smooth/cleanstopped. A1/B1radiohandler41.672/22.830us,
RFsubmit50.774/33.196us, RFcallbackglobal565.837/427.624us butA1phaseweightsyield
565.837/553.031us(only12.807us descriptive difference,coarse100usbins). Busy/tail
notbothimproved; no reliabletotalgain/promotionclaim. PrivatecomparisonJSON+tables.
NEXT reverseorderreplication usingEXACTSAVED v19B2 thenv18A2, no newbuilds/code.
These are independentboots forrepeatability, notblindbaseline repeats. Two100s
captures withseparateflash/inputreadiness, eachxstop/threefrozenframes. Keepmouse
wired/OFFbetweenimages; leaveeachdiagstoppedpendingnextflash. RestoreEXACTnormal
v14 ONCE afterbothruns (orimmediatelyifbatchaborted), verifyUSB/useroperation.
Thisbatchavoidsunnecessaryintermediate normalflash; normalisinstalledNOW.
Branchfeature/usb-phase-poll-comparison. Useruntilsaystop; nofixedcutoff.
Physicalpopupnext v19B2 NICENANOreadywired/OFF; verifyexactimage/board beforecopy.

NORMAL V14 RESTORED AFTER V19 B1 / USB VERIFIED - 2026-11-18 18:05 Manila:
UserNICENANOreadywired/OFF. UniqueBoardID/S140/exacttestednormal93ebSHA/181block
UF2verifiedthenrestored. ProductDIY Mouse Dongle v14 uniqueUSB/CDC/HID verified
WITHOUTserial. Private normal-v14-restored-after-v19-b1.json. InstalledNOWnormal
v14autoactive/noarm/notimedstop; NOTv19/stopped. No capture/reader/buildlive.
NEXT briefuserautomaticwirelessrestorationcheck thenpushB1result/restoration;
optionalindependentB2/A2replication perplan, exactimagespreserved/no newbuild.
B1control/inputpass butphaseweightedtotalgainonly12.807us/uncontrolled100usbins;
nofastpollpromotion. Useruntilsaystop. Branchfeature/usb-phase-poll-comparison.

V19 B1 PASS / RESTORE NEXT - 2026-11-18 18:03 Manila:
Session37089 completed100s,read_errors0/failed0,xonce,d50silent,all3frozenframes.
Calibrated1,invalidphase1 explicitlyinbin10, completeaccounting. Accepted19376=
host19376 ALLforeground;dx=-217/dy=-14/wheelnet2,all5buttons3down/3up, aggregate
content/countdifferences0. User smooth/cleanstopped/noissue. Privatev19-latency-
20261118-175953-8719f360.summary.json. RFreaction22.830us/RFsubmit33.196us vs
A1v18 41.672/50.774us. RFUSBcallbackmean427.624us/max1119/p99upper1119us vsA1
565.837us/max1150/p99upper1000us. Busy1.134% vs0.592% A1; no consistenttailgain.
A1referenceweights across10commonvalidphasebins B1mean553.031us vsA1 565.837us,
only12.807usdifference afterphaseweighting (B1invalid1excluded). Coarse100usbins/
manualmotion/onebootpercondition preclude causalgainclaim. Privatephase-comparison-
a1-b1.json. No fastpollpromotion. NEXT freshNICENANOwired/OFF restoreEXACTnormalv14
93eb, verifyUSBwithoutserial/userwireless. Installedv19 STOPPEDbyx; nolivereader/
capture/build. Thenpushresult; potentialindependent B2/A2 replication perplan.

V19 B1 FOREGROUND CAPTURE RUNNING - 2026-11-18 18:00 Manila:
UserOFF/unplugged/samesetup readiness confirmed. Session37089 base
build-pairing-analysis/v19-latency-20261118-175953-8719f360, oneCDCreader+100sraw
foreground. CaptureON/controlsmotioncuepopup sent. Noarm,d50silent,x70once,three
frozenfooters. Expectedfinish~18:01:40. Abortbase.stop-request. NEXTpoll37089,
validatephase/input/timing,collectuserfeel/stop, restoreexactnormalv14 viafresh
NICENANOreadiness. Useruntilsaystop; internal30minguard. Installedv19 untilrestore.

V19 FLASHED / USB VERIFIED - 2026-11-18 17:59 Manila:
UserNICENANOreadywired/OFFconfirmed; uniqueBoardID/S140/exact8ad6dd03SHA/199block
UF2verifiedthenflashed. ProductDIY Mouse Phase Fast v19 withuniqueUSB/CDC/HID
verifiedWITHOUTserial. Privatev19-flash-verification.json. InstalledNOWv19 auto
listening; NOTnormalv14. Calibration/hardwareinputpending. No livecapture/reader/
build. Softwarebdf6322 pushed. NEXT separateOFF USBunpluggedsameDPI/surface/
otherpointerreadiness then100sforegroundtrialExpectedProductv19,now+30mininternal
guard,noarm,d50silent/x70/stopped3frames. Matchrequestedcontrols/motionA1v18.
Analyzephase/input/globalstage metrics,userfeel/stop, thenrestoreexactnormalv14
93eb viafreshphysicalreadiness. Useruntilsaystop; nofastpromotionassumed.

V19 SOFTWARE VERIFIED / FLASH READINESS NEXT - 2026-11-18 17:56 Manila:
Branchfeature/usb-phase-poll-comparison. UsercontinueUNTILTHEYSAYSTOP,no fixedcutoff.
Normalv14 93eb987a installed/automatic/userconfirmed; nohardware/serial/capture/
buildsession. Build57412 Appphase-fast completed. Privatephase-fast-v19-8ad6dd03.uf2
SHA2568ad6dd031a595b4558112c47f915f52b4dc5ebd0754213cd89ff46b0aff9b17e,
199blocks0x26000..0x32700exclusive,50812flash/17280RAM. ProductDIY Mouse Phase Fast
v19. ALLdiagflagsON includingFAST; SAMECsource/privateRFparameters asqualifiedv18.
.configdiffONLYUSBproduct; TIMER3disabled/6CC/SOFIRQOFF/quiet1ms config retained.
All11native/11researchtests +controllerSelfTest/syntaxPASS. Exactv18baseline05eaf
andtestednormal93eb hashesverified. Privatev19-software-validation.json.
V19 NOTflashed/hardwareunproven. NEXT pushsoftware thenphysicalNICENANOready
mousewired/OFF;verifyboard/exactimageflash+USBproduct. SeparateinputreadyOFFUSB
unplugged,sameDPI/surface/otherpointer. Trial100s noarm,d50silent/x70/stopped3frames,
ExpectedProductDIY Mouse Phase Fast v19, internalnow+30minfinishguard. Controls
first(each5buttons3pairs,wheelup5/down3),thensteadymotion. Validatephase/input/
timing+userfeel/stop. RestoreEXACTtestednormalv14 viafreshphysicalreadiness after
trial;verifyUSB/usernormaloperation. Preserveallruns; potentialB1/B2/A2comparison
withv18A1 alreadyrecorded ifpractical, no assumedpromotion/physicalgain.
100usbuckets/manualmotion/workload/lowearlyphaseNlimitcausal20usinferences.
V18baselinehardwared249645 pushed; A1calibrated1/invalid0/exact21818match,user
smooth/cleanstopped; tableinresearch. Companionapp/settingswritesdeferred.

V19 COMPARISON BUILD RUNNING - 2026-11-18 17:57 Manila:
Session57412 Appphase-fast only; no repeatedv18/normalbuild needed becauseCcode
unchanged. ControllerExpectedProductv19 andthreefootergatesSelfTest/syntaxPASS,
11researchanalysischecksPASS. Need native/buildfinish/config/source/UF2/hash
verification,preserveimage andpushsoftwarecheckpoint. NewconfigproductDIY Mouse
Phase Fast v19,onlyexistingFASTflagON; SAMECfilesasqualifiedv18. No hardware/
serial/capture. Installed normalv14,userconfirmed. Useruntilsaystop; nofixedcutoff.
Planphase-poll-comparison.md predeclaresphase-awarecomparisonlimitations incl
100usbucketresolution vs~20usreactionchange. No causal/promotionclaim assumed.

PHASE-AWARE FAST POLL COMPARISON SOFTWARE - 2026-11-18 17:55 Manila:
V18hardware/restorationd249645 pushed,usernormaloperationconfirmed. Newbranch
feature/usb-phase-poll-comparison. PrepareAppphase-fast v19productDIY Mouse Phase
Fast v19, SAME Ccode/instrumentation asqualifiedv18, onlyFASTflagON (10vs50us).
Build/config/controllerproduct changesonly. Need native/build/config/stagedsources/
UF2/hash checks,preservev19 andpushBEFOREphysicalpopup. Normalv14remainsinstalled;
nohardware/serial/capture/buildyet. Planphase-poll-comparison.md. Useruntilstop.
Proposed A1(v18qualified)/B1/B2/A2 independentboots ifuseful; coarse100usbins,
manualmotion/workloadlimits; no assumednormalpromotion orpacket-levelCIs.

V18 QUALIFIED / NORMAL RESTORATION CONFIRMED - 2026-11-18 17:52 Manila:
User confirmed restored normal wireless works. Installed normalv14exact93eb987a,
autoactive/no serial/arm/limit; no livecapture/reader/build/helper. Privateafterv18
restoreJSON updated. V18calibrated1/invalid0, all21818foregroundreports+inputtotals
match, all5buttons3down/up, user smooth/cleanstopped. Perphase table savedresearch.
Withinthisrun RFcallbackphasebinsmean1.058ms(n28 early) down0.207ms(late), global
0.566ms/max1.150ms; RFsubmit~36..59us. ObservationalUSBphaseassociation, notgain.
Wholemixedactivityhostgapsmedian1010us/p95=2147us/p99=5797us include deliberate
controlpauses/zeromotion; NOTsustainedpolling orRFloss qualification. Busy309/52184
candidates,retries34; no inferredend-to-endloss. HistoricalA2causeunresolved.
NEXT isolatedfastpollcomparison usingSAMEphaseinstrumentation asv18: addphase-fast
v19 product/config (10us vs50us), nootherC/runtimechange. Build/test/verify/preserve
andpush BEFOREphysicalreadiness. V18A1 preserved; compareB1/B2/A2onlyifappropriate,
phase-aware/repeatedboots/manualmotionlimits; noautomaticpromotion. Restoretested
normalaftereachdiagnostic. Userwindowuntilsaystop, oldcutoffsrevoked. Companionapp
andDPIwritesdeferred. Keepprivatecaptures/addresses/binariesignored.

NORMAL V14 RESTORED AFTER V18 / USB VERIFIED - 2026-11-18 17:50 Manila:
UserNICENANOreadywired/OFF. BoardID/S140 and exacttestednormal93eb hash/181block
UF2verifiedbeforeflash. ProductDIY Mouse Dongle v14, uniqueUSB/CDC/HID verified
WITHOUTserial. Private normal-v14-restored-after-v18.json. InstalledNOWnormalv14
automatic/noarm/unboundeduntilunplug; NOTv18/stopped. No livecapture/serial/build.
NEXT briefautomaticwireless usercheck, thenpushv18hardwarephase/input result and
restorationhandoff. V18passcalibrated1,invalid0,21818exactcount/contentmatch,user
smooth/cleanstopped; allphasebucketsreconcile. No fastpollpromotion orphysical
latencyclaim. Userwindowuntiltheysaystop. Timingcandidatefuturecomparisonsmust
conditiononUSBphase/repeatboots; diagnosticshaveobserveroverhead.

V18 HARDWARE PHASE/INPUT PASS / RESTORE NEXT - 2026-11-18 17:48 Manila:
Session96530 completed100s, read_errors0/failed0; xonce/active d silent/all3frames
complete. Startuphardwarecalibrated1, invalidphase0; everyphasecount/sum/hist
reconcilesglobal accepted21818=host21818 ALLforeground. Inputdx=-94,dy=-82,
wheelnet1/120Windowsunits, eachfivebuttonsdown3/up3; allaggregatedifferences0.
User allworked smoothly/stoppedcleanly. Privatev18-latency-20261118-174506-f45a9f45
.summary.json. RF->USBcallbackmean565.837us,max1150us,p99histupper1ms. Phasebin
meansrange1058.250us(0..99us,n28) to206.744us(900..999us,n2765); RFsubmitmeans
~36..59us. Strongphaseassociation inONEinstrumentedrun, notoptimizationproof or
physicalend-to-endlatency. HistoricalA2causeunknown; nofastpollpromotion.
Installedv18 STOPPEDbyx; no reader/capture/build live. NEXT physicalNICENANO
readinessmousewired/OFF,restoreEXACTtestednormalv14 93eb987a, USBidentityverify
withoutserial thenbriefautomaticwirelessusercheck. Save/pushhardwaremilestone.
Userwindowopenuntiltheysaystop; noolddeadline. Keepnormalrollbacknotcontrolcb8bbb.

V18 FOREGROUND CAPTURE RUNNING - 2026-11-18 17:45 Manila:
UserOFF/unplugged/otherpointer readiness confirmed. Session96530 base
build-pairing-analysis/v18-latency-20261118-174506-f45a9f45, oneCDCreader +100s
foregroundraw. Autoactive,noarm. Capture-running ON/activatewindow/controlsmotion
cuepopup sent. d50mustsilent,x70once,requireLATENCY/INPUT/PHASEfrozenfooters.
Expectedfinish~17:46:50. Operational30minfinishguard,useruntilsaystop. Abortpath
base.stop-request. NEXTpoll96530, analyzeallframes+CSV, collectuserfeel/stop then
restoreexactnormal93eb viafreshNICENANOreadiness. Installedv18;hardwarecalibpending.

V18 FLASHED / USB VERIFIED - 2026-11-18 17:36 Manila:
User NICENANO wired/OFF readiness confirmed. Unique bootloaderBoardID/S140 and
exact05eaf02a SHA/199-block application verified thenflashed. ProductDIY Mouse
Phase v18 uniqueUSB/CDC/HID verified WITHOUTserial. Privatev18-flash-verification.
Installed NOWv18 automatic/listening, NOTnormalv14. Calibration notyetverified by
frozenframe/hardwaretrial; no capture/serial/build session. Software681183d pushed.
NEXT separateinputreadinessOFF USBunplugged, sameDPI/surface/otherpointer. Then
100sforeground trial expectedv18, operationalnow+30minfinishguard (useruntilstop).
Prioritizeeach5buttons3pairs+wheelUP5DOWN3 thensteadymotionuntilx70. Requireall3
frames andphasecalibration/coverage/inputmatch; collectuserfeel/stop and restore
exactnormalv14 viafreshphysicalreadiness. No fastpollpromotion.

V18 SOFTWARE BUILT/VERIFIED / PHYSICAL FLASH NEXT - 2026-11-18 17:32 Manila:
Branch feature/usb-frame-phase-diagnostics. User says continue UNTIL THEY SAY STOP;
no fixed cutoff. Normalv14 93eb987a STILL INSTALLED/automatic, nohardware/serial/
capture/build session now. Build40533 completedphase+normalcontrol successfully.
V18 private phase-v18-05eaf02a.uf2 SHA256
05eaf02aebbbb49163a2a8f9b95c1e5914ae06afd73a4793eba43ab48e2549cd,
199blocks0x26000..0x32700exclusive,50792flash/17280RAM. Appphase productDIY Mouse
Phase v18, LATENCY/INPUT/USB_PHASE ON,FASTOFF; quiet1msUSB. Stagedsourcesmatch,
TIMER3DTdisabled/6CC andNRFXdriverOFF,SOFIRQOFF verified. Normalcontrol diagnostics
OFF,46188flash/16064RAM,hashcb8bbb60; NOThardwarequalifiedrollback. Exacttested
normal93eb hash verified. Private v18-software-validation.json, imageignored.
All11native +11research testsPASS; controllerSelfTest/syntaxPASS; actual Coutput
cross-parses/reconciles withPython. Calib3consecutive950..1050usSOF intervals,
bounded300x50uspollstepsplusIRQoverhead. Phase0..9 +invalid10; acceptedcompletion
counts/hist/sums reconcileglobal; inputtotalsretained; no activeoutput/perfchange.
V18 NOTFLASHED/HARDWAREUNPROVEN. NEXT pushsoftware thenphysicalpopup Glorious
wired/OFF,receiverNICENANO; verifyboard+exactimage,flash,USBidentity. SeparateOFF/
USBunplugged readiness then100sforegroundcapture ExpectedProductDIY Mouse Phase
v18. Noarm;d50silence/x70/stoppedthreeframes. Use internalnow+30min finishguard
for open-endeduserwindow; notusercutoff. Trialcontrolsfirstthensteadycircles.
Analyzephase/input/timing, collectfeel/stop, restoreEXACTtestednormal93eb viafresh
physicalreadiness andbriefautomaticwirelesscheck. No fastpollpromotion orphysical
latencyclaim. HistoricalA2tworecordcauseunresolved. App/DPI/lightingwritesdeferred.

V18 BUILD RUNNING / OPEN-ENDED USER WINDOW - 2026-11-18 17:26 Manila:
User explicitly says continue UNTIL THEY SAY SO; no fixed cutoff. Build session40533
Appphase thenAppdonglecontrol. All11native checks pass; added actual C-frame->Python
analyzer cross-check passes, all10research checks pass, controllergates/syntaxpass.
Native tests cover TIMER3ownership/SOFIRQcompileguards, all9PPIalloc/assign/enable
failurepoints, timerstall/missing/irregularSOF cleanup, phaseboundary/stale/wrap,
earlycompletion/failedwrites, acceptedinput totals. No firmware edits since staged
build; only test/host/docs work since. Normalv14 stillinstalled, nohardware/serial.
Startup calibration bound is300x50us polling steps plusinterruptoverhead, observing
threeconsecutive950..1050us intervals. Phaseframe11rows includesinvalidbin10.
NEXT finishbuild40533 andconfig/source/UF2/hash verification; preserveimage,
updateconcisehandoff andpushsoftwarebeforephysicalflashreadiness.

V18 NATIVE/ANALYZER CHECKS PASS / BUILD NEXT - 2026-11-18 17:24 Manila:
User updated workwindow: UNTIL THEY SAY STOP; old15:00cutoff expired/revoked.
No new fixeddeadline. Normalv14 remains installed, no hardware/serial/capture.
Branch feature/usb-frame-phase-diagnostics. Opt-in Appphase productDIY Mouse
Phase v18, TIMER3unownedasserts, thirdPPIUSBD SOF->CC4, startup3consecutive950..
1050usintervals within15ms; no queuedSOFIRQ. Failedresource/cadencefailclosed.
11 native testsPASS (inclphasebridge/writer/resourcevariants);10 research tests
PASS. ControllerSelfTest/syntaxPASS, v18requiresLATENCY+INPUT+PHASEfooters. Phase
0..9 100usacceptedsubmissionbuckets +10invalid(stale/changing/uncalibrated), perbin
RFUSBhist/RFsubmit sums reconcileaggregate. No normal fastpoll/performancechange.
Native/resource/collector/analyzer testsfailedbeforeimplementationthenpassed.
NEXT buildphase andnormalcontrol;verifystagedsources,flags/DT,UF2/hash/privatepreserve,
exactnormal93ebrollback;pushsoftwarecheckpoint BEFOREphysicalflashreadiness.
After readiness trial100s noarm/x70/dquiet50/stopped-onlydump, collectfeel and
restoreexactnormalv14. For useropenendedsession use internal30minrollbackdeadline
per100srun (operational guard, NOT a userrequestedstop time). Keepcontextcurrent.

RESUMED / USB FRAME PHASE IMPLEMENTATION - 2026-11-18 17:18 Manila:
User resumed; earlier15:00break revoked by explicitresume. New stop time popup
pending; do software work whilewaiting and do not infer a hardware deadline.
Installed normalv14 93eb987a unchanged, no serial/capture/build yet. Latest prior
S3 milestone c8e1882 pushed. Newbranch feature/usb-frame-phase-diagnostics.
Implement opt-in v18 hardwareSOF/TIMER3+PPI phase measurement retainingv17 input
accounting and stopped-only guards. FAST remainsOFF. Plan docs/plans/2026-11-18-
usb-frame-phase-implementation.md. No app/DPI/lightingwrites. Need native tests,
analyzer/controllerchecks, builds/config/UF2 validation and software push BEFORE
physicalflashreadiness. Restore exact normalv14 after any later trial.

NORMAL V14 WINDOWS S3 RECOVERY PASS / SESSION CHECKPOINT - 2026-11-18 14:54 Manila:
User-operated Windows sleep/resume with ALL buttons released completed. User
reported ALL controls recovered automatically/smoothly, no issues/reconnect/reset/
mousepowercycle. Power-Troubleshooter1 TargetState/EffectiveState4=S3, SleepTime
06:47:25.3535Z/WakeTime06:50:30.6608Z (~185.307s); KernelPower42/107 also present.
Do not interpret event header107 time as actualwake or SleepDuration3189 as total
sleepseconds. Receiver unique normalv14 USB identity OK afterwake withoutserial.
Passive300s raw session4802 finished1776receiverrecords,read_errors0/failed0;
observed movement,left15/right2/middle2 balancedpairs,wheel. Thumbedges0: all5
postresume success is USER-REPORTED, not independently captured. Background counts
are not polling/latency qualification. Private v14-sleep-20261118-144655.summary.json.
Installed tested normalv14 93eb987a automatic untilunplug; user can keepusing it.
NO live serial/capture/build/helper. No diagnostic timing image remains installed.
Milestones: v17acceptedcontent/count9585 exactmatch and five-minute inactivity
recovery passed earlier today; normalrestoration confirmed. Historical A2two-record
cause unresolved; no fastpollpromotion/end-to-endlatency/sustained1kHz claim.
NEXT on resume: refreshworkwindow (today's15:00Manila cutoff expires afterthisday),
implement isolated USBframehardwaretimestamp candidate per
 docs/plans/2026-11-18-usb-frame-timing-review.md on feature-basedbranch. SOFTWARE
REVIEW ONLY sofar; TIMER2all4CCoccupied; queuedSOF/INcallbacksinclude dispatchdelay.
Preserve exactnormalv14; no repeatedbaselines/builds/flashes withoutclearquestion.
Long4h/8h/multiday soak,repeated sleep/hibernate,mouse deep-sleep,broaderRF checks
remain. Companionapp/DPI/lightingwritesdeferreduntilgamingoptimization. Captures,
addresses,binariesignored; .vscode/ andINFO_UF2.TXT unrelated/untouched.

WINDOWS SLEEP/RESUME CAPTURE RUNNING - 2026-11-18 14:47 Manila:
User ready/worksaved/GloriousON USBunplugged/keyboard confirmed. Unique normalv14
USB product and rawlogger hash verified. Passive300s raw session4802, base
build-pairing-analysis/v14-sleep-20261118-144655; no serial/arm/reset. Sleep cue
popup sent: releaseallbuttons, userStart/Power/Sleep30s, keyboardwake/signin, then
movement/all5buttons/wheel without reconnect. Parent waits max300s (sleep may
consume walltime); expected14:51:56 end. Need poll4802, collect useractualrecovery,
query recent System42/107/1 evidence and current USB identity. No tool-initiated
sleep/UAC/driverchange. Normalv14 remains active. Deadline15:00Manila today.

PAUSED FOR WINDOWS SLEEP/RESUME READINESS - 2026-11-18 14:45 Manila:
Idle milestone7710247 pushed. Readonly powercfg /a reports StandbyS3 andHibernate
available, S0lowpoweridle unavailable. Physical readiness popup pending: savework,
normalv14receiverconnected,GloriousON USBunplugged/allbuttonsreleased,keyboardwake.
No sleep/capture triggered yet. Ifready with enough margin before15:00, start a
bounded~300s passive raw capture, cue Start/Power/Sleep30s, keyboardwake/signin,
then movement/clicks/wheel without receiverreconnect. Verify actual sleep/resume
via recent Kernel-Power42/107 and Power-Troubleshooter1 ifavailable; record exact
observed evidence, not assumptions. No forcedsystemsleep/tool, UAC or driverchange.
Ifreplydelayed/insufficienttime, pause and leave testednormalv14 installed.

NORMAL V14 FIVE-MINUTE IDLE RECOVERY PASS - 2026-11-18 14:45 Manila:
User confirmed recovered promptly, controls smooth, no reconnect/powercycle. Idle
explicitstart14:36:35; wake cue AFTER14:41:35 threshold. Segmented passive captures
completed cleanly: pre2286/wake2381 receiver records, bothread_errors0/failed0.
Wake raw contains movement,left1/right2/middle2 balancedpairs,wheelUP29/DOWN40;
thumb edges0 and activity reaches capture end: do NOT claim post-idle thumb test.
Pre/post recordedinputgap613.099s includes uncaptured idle; only conservative
user-confirmed5min inactivity claimed, not uninterrupted10min/deep-sleep proof or
measured wake latency. Private v14-idle-20261118-143124.summary.json. Journal cue
UTC recorded after popup, not exact cue delivery. No continuous full-idle coverage.
Installed normal v14 automatic and user-confirmed; no recording/serial/build live.
NEXT released-button Windows sleep/resume reliability check on unchangedv14 if
physical readiness/time permits, otherwise future USBphase diagnostic per review.
Pause15:00Manila today. No further flash planned. No fast-poll promotion/appwrites.

FIVE-MINUTE WAKE CUE SENT / RECOVERY RECORDING ACTIVE - 2026-11-18 14:42 Manila:
Wake cue sent AFTER14:41:35 threshold; conservative idle5min completed. Separate
100s passive wake capture session61676, base original-idle-base + -wake. User
asked movement/all5buttons/wheel20s, no resets/powercycle. Normalv14 active; no
serial/arm/flash. NEXT poll61676 (~14:43end), analyze pre/post CSV and user recovery
reply. Explicit segmentation; no deep-sleep or true wake-latency measurement claim.

IDLE INITIAL RECORDING COMPLETE / WAITING FOR WAKE EPOCH - 2026-11-18 14:39 Manila:
Session45325 finished420s, raw2286records, read_errors0/failed0. Receiver input
pre-summary2286, balanced observedbuttons: left12/right7/middle2 pairs,470wheel
records. Last recorded movement~14:32:32; conservative explicit idle-start remains
14:36:35. No live recording/serial/build now. Do NOT cue wake before14:41:35.
Need start separate100s passive capture then popupcontrols (normal v14 unchanged).

FIVE-MINUTE IDLE START CONFIRMED - 2026-11-18 14:36:35 Manila:
User pre-idle controls tried; mouse ON and untouched. Idle start UTC06:36:34.9707,
wake NO EARLIER THAN06:41:34.9707 (14:41:35Manila). Normalv14 autoactive unchanged.
Private current-idle-base.txt identifies v14-idle-20261118-143124; .idle.json holds
explicit epoch. Initial420s capture session45325 ends14:38:25 BEFORE wake because
user initial reply was delayed. Do not claim continuous host coverage of full5min.
NEXT atwake start separate bounded100s passive Raw Input capture (no serial),
then popupmove/click/wheel. Analyze pre/post separately; idle observation/user
inactivity can still span5min. Do not auto reset/arm or flash. Keep initialcapture
until its existing limit; poll45325 completion. No mouse deep-sleep proof claimed.
Pause15:00Manila today. Record segmentation transparently in final results.

NORMAL V14 IDLE RECOVERY CAPTURE RUNNING - 2026-11-18 14:31 Manila:
User readiness document/other pointer/Glorious OFF-unplugged confirmed. Unique
normal v14 USB identity and known raw logger hash verified. No serial opened.
Passive background Raw Input420s session45325 started; private base
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\build-pairing-analysis\v14-idle-20261118-143124. Cue ON/controls15s then ON/untouched popup sent.
NEXT await user's idle-start answer; record UTC, time5min, then popupwake/control
cue. Capture hard duration420s from14:31:24 (~14:38:24 end); if user's initial
answer is delayed >~60s, do not falsely claim5min fully captured; analyze what
actually occurred. No firmware stop; normalv14 remains active after capture.
Pause15:00Manila today. Background counts for recovery only, not rate qualification.

NORMAL V14 RESTORATION USER-CONFIRMED / IDLE RECOVERY NEXT - 2026-11-18 14:32 Manila:
User confirmed restored normal wireless movement/click works. Installed tested
normal v14 93eb987a, automatic until unplug; no serial/capture/build running.
V17 accounting milestone4b99ed2 pushed, exact9585report/content match, no issues.
Reviewed SDK USB SOF path: usb_dc_nrfx.c queues USBD_EVT_SOF then delivers status
in usbd_work_handler; callback timestamp is NOT hardware SOF time. TIMER2 already
uses all4CC slots. Future phase instrumentation needs isolated diagnostic timer/
PPI hardware capture and native resource/race/wrap tests; not implemented/flashed.
Next useful unchanged-v14 reliability check: bounded five-minute mouse inactivity
then movement/button/wheel recovery with no receiver reset or arm, followed by
continued normal use. This only demonstrates chosen idle duration; not proof of
mouse deep sleep, OS suspend or overnight endurance. Need physical readiness popup.
Pause15:00Manila today; no long trial that overruns this deadline. User ordinary
wireless use no issues, duration unknown. Companion app remains deferred.

NORMAL V14 RESTORED AFTER V17 / USB VERIFIED - 2026-11-18 14:26 Manila:
User NICENANO readiness wired/OFF confirmed. Unique bootloader board/S140 and
exact tested normal93eb987a SHA/181-block application UF2 verified then restored.
Windows product DIY Mouse Dongle v14, unique root/CDC/HID verified WITHOUT serial.
Private normal-v14-restored-after-v17.json. Installed NOW normal v14, automatic
plug-and-play until unplug; not v17/stopped/passive. No live serial/capture/build.
NEXT brief user automatic movement/click restoration check, then save/push v17
hardware results and concise handoff. Today pause15:00Manila (UTC07:00Nov18).
V17 one trial exact9585report/content/5button match; user smooth/cleanstop. Earlier
A2 two-record cause remains unknown; no fast-poll promotion/end-to-end latency claim.

V17 HARDWARE ACCOUNTING PASS / NORMAL RESTORE NEXT - 2026-11-18 14:24 Manila:
Controller34679 completed100s, raw read_errors0/failed0, x once, active d silent,
both complete frozen frames, no failed/capped/tracking fault. User all worked
smoothly and stopped cleanly. Private v17-latency-20261118-142055-af9a2982 summary.
Device accepted9585 = host9585, all foreground. Exact aggregate dx=-179 dy=85,
wheel2steps/240Windowsunits, each five buttons down3/up3, neutral0, unchanged0;
all content/count differences0. RF->USBcallbackmean756.490us/max1155us; not true
end-to-end latency and no controlled optimization comparison. Aggregate equality
is not event-order/individual-preservation proof; historical A2 gap unresolved.
Installed v17 stopped by x (latches until restart/p); no serial/capture/build live.
NEXT physical popup NICENANO wired/OFF, restore EXACT tested normal v14 93eb987a,
verify USB identity without serial and user brief automatic wireless check. Then
commit/push results/handoff. Deadline15:00Manila today. No fast-poll promotion.

V17 ACCOUNTING CAPTURE RUNNING - 2026-11-18 14:21 Manila:
User OFF/unplugged readiness confirmed; controller session34679 started, base
build-pairing-analysis/v17-latency-20261118-142055-af9a2982. One CDC reader and
100s foreground Raw Input active; automatic startup, no arm command. Power-on/
button-wheel-motion cue popup sent. Active d~50s silent; x~70s, then both frozen
frames. Expected finish ~14:22:38. Stop-request path is base.stop-request if abort.
NEXT poll session34679; analyze both summaries/CSV, collect user observations.
Installed v17; restore exact tested normal v14 93eb987a afterward before15:00.

V17 FLASHED / USB VERIFIED - 2026-11-18 14:19 Manila:
User confirmed NICENANO visible and Glorious wired/OFF. Unique NICENANO board-ID/
S1406.1.1, exact aea67abc SHA and 196-block UF2 verified before copy. USB product
DIY Mouse Input v17, unique root/CDC and HID verified without serial. Private
v17-flash-verification.json. Installed NOW v17, automatic/listening; NOT normal
v14. No serial/capture/build session running. NEXT separate mouse OFF/unplugged
input readiness, 100s foreground accounting capture, then restore preserved normal
v14 93eb987a before today's 15:00 Manila deadline. Software milestone 9ecc2bf pushed.

V17 SOFTWARE VERIFIED / PHYSICAL FLASH READINESS NEXT - 2026-11-18 14:16 Manila:
Branch feature/input-conservation-diagnostics. Pause today at 15:00 Manila
(2026-11-18T07:00:00Z). Normal v14 remains installed and automatic; no hardware
access, serial reader, capture or build session is running. User ordinary wireless
use since last session: no issues noticed; duration unknown, not measured soak.
App input v17 built successfully. Preserved private input-v17-aea67abc.uf2,
SHA256 aea67abc7c748d90c3fde1cc2931343d3e81fcf5b2492c741de82cebfb81b01d.
196 application blocks 0x26000..0x32400 exclusive. Staged sources, flags and quiet
1ms USB configuration verified; normal control diagnostics OFF; tested rollback
93eb987a verified. All 10 native checks, 7 research tests, controller SelfTest and
syntax passed. Private v17-software-validation.json. V17 NOT flashed or hardware
qualified. Adds stopped-only accepted dx/dy/wheel and five-button edge accounting;
no performance change or normal fast-poll promotion. Aggregate equality cannot
prove individual-event preservation/order or historical A2 two-record cause.
NEXT physical popup: Glorious wired/OFF, receiver NICENANO. Verify board/exact
image, flash, verify USB product DIY Mouse Input v17 without serial. Separate OFF/
USB-unplugged readiness, then 100s foreground capture ExpectedProduct v17 with
FinishBeforeUtc 2026-11-18T07:00:00Z. No arm command; automatic startup. Cue ON,
activate capture with other pointer, each of five buttons 3 press/releases, wheel
UP5/DOWN3, then controlled motion until x at ~70s. Collect user feel/stop; analyze
both complete frames and all receiver rows. Restore EXACT tested normal v14
93eb987a after trial via fresh physical readiness, verify ordinary operation.
Prefer no diagnostic flash after 14:45; no capture at/after14:52. Companion app,
DPI writes and USB frame-phase instrumentation deferred. Private captures/binaries
ignored; unrelated .vscode/ and INFO_UF2.TXT untouched.

V17 INPUT ACCOUNTING SOFTWARE / BUILD IN PROGRESS - 2026-11-18 14:10 Manila:
Branchfeature/input-conservation-diagnostics. Userpause15:00Manila(UTC07:00today).
Normalv14STILLinstalled/autoactive;no hardwareaccess/serial/capture yet thissession.
Userordinarywirelessuse:usedit,noissuesnoticed;durationunknown,NOTmeasuredsoak.
Newopt-inAppinputproductDIY Mouse Input v17;NORMAL/LATENCY/INPUT flagsON,FASTOFF.
Main-threadaccepteduniqueUSBpayloadtotals(dx/dy/wheelsigned64,fivebuttonedges,
unchangedreports),separateforcedneutralup,capflags/reset. No perpackettext ornormal
behaviorchange. LATENCY_V1retained,INPUT_V1/SUM/EDGES/ENDappendedpoststoponly.
ControllerExpectedProductv17+bothfootergatesSelfTest/syntaxPASS. Analyzerincludes
allreceivercode0/1rows,wheel120conversion,excludeswired;validatesdevicecoverage/
neutraledges/fields;reportsdifferences andlimitedaggregateequality,notlossless
order/timingproof. Historical2recordcauseunresolved. Nativeinitialcounttestfailed
beforeimplementation;ALL10nativecheckspass(withbridge/writerconservationmodes).
Research7analysischeckspass. LIVEBUILDexec15840 Appinput thennormaldonglecontrol;
firstfreshCMakeconfigured184s,compiling. Needpollandverifyfinalsource/config/UF2/
hashpreserve,pushsoftwarecheckpointbeforephysicalflashpopup. No morefirmwareedits
sincebuildstaging;hostparser/controllereditsdo notrequirefirmwarerebuild.
Next physicalNICENANOreadywired/OFF,thenv17identityandseparateOFF USBunplugged
inputreadiness. Existing100sCDC+foregroundcapture,noarm,dquiet50/x70/poststopdump.
Prioritizeallfivebuttons3press/releases,UP5/DOWN3wheel,thenmotion;keepbuttons
released. RestoreEXACTtestednormal93ebaftertestbefore15:00. Prefernoflashafter
14:45;nostartat/after14:52(8minmargin). No SOFinstrumentation/app/DPIwritesnow.
Plan docs/plans/2026-11-18-input-conservation.md. Copy-pastehandoffneedsfinal
consolidationoncebuildready. No subagents or newdependencies.

RESUMED NOVEMBER18 / RESTORATION CONFIRMATION RECONCILED:
User's final Nov17 reply confirmed restored normal v14 wireless operation works
normally. Prior pending restoration popup is resolved; no additional flash needed.
Last verified installed image remains normal v14 SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052, automatic
plug-and-play until unplug. No live reader/capture/build/helper from prior session.
Today's work window: until15:00 (3PM) Manila November18,2026, UTC07:00. Old03:30
Nov17 deadline expired. Physical connections/power not reverified since break.
Next investigate the two host-record difference with opt-in diagnostic accepted
motion/wheel/button-edge totals before further latency optimization. Preserve
working v14 and frozen v15/v16 images. No normal fast-poll promotion. Companion app
remains deferred until AFTER gaming optimization; no mouse setting writes.
Current branch before new work feature/receiver-fast-poll-latency; prior restored
state was local uncommitted context, now being saved. Only .vscode/INFO_UF2.TXT
are unrelated untracked;keep them untouched. Continue until physical readiness is
needed,then question popup/pause. Keep context and concise resume prompt current.

NORMAL V14 RESTORED AFTER ABBA / USB VERIFIED - 2026-11-17 03:15 Manila:
Userv15run2Smooth/cleanstopped/noissueandNICENANOreadywired/OFF. Board/S140/exact
normal93eb987a hash/181blockUF2verifiedthenrestored. ProductDIY Mouse Dongle v14,
HID/CDCverifiedWITHOUTserial. Private normal-v14-restored-after-replication.json.
InstalledNOWnormalv14automatic/listening;NOTtiming/disarmed/passive/expired.
No livecaptures/readers/builds/helpers/UAC. BothreplicationuserfeelreportsPASS.
Fourruncomparison1b38571pushed;corefindingno consistenttotalreceiverlatencybenefit
fromfastpolldespiterepeatedquickerRFhandling. Do notpromotefasttonormal.
Briefrestorationwirelesschecknext;normalv14wasalreadyhardwaretestedandearlier
restorecheckpassed. No morediagnosticruns/firmwarechanges. Deadline03:30AMNov17.
NEXT savefinalhand-off/fourruntableandplanningforUSBframephase+acceptedinput
motion/buttonconservationtoinvestigatetworecordhostdifference. No newinstrumentation
implementedthissession. Refreshdeadlineonlaterresume;companionappdeferred.
Currentstatesupersedesoldertiming-installed/pendingrestorehistory.

V15 RUN2 CAPTURE VALID / ABBA CONCLUSION - 2026-11-17 03:10 Manila:
exec27549complete100srawread_errors0. Internaldeviceframeall6hist/count/sum/ACK/
busychecksPASS,failed/capped/trackingfault0. Deviceaccepted/completed27594;
hostRawInput27592,ALLreceiverforegroundcode0. Unexplained2recorddifference;NOT
proofRFpacketloss,butblocksclaimedzeroend-toenddrop. No device-motion/checksum
accumulationexistsyet;hostcapturecoalescing/suppressiononlyhypothesis,notproven.
Privatebasev15-latency-20261117-030634-b8ad37cb .summary.json saved,userreportpending.
V15run2candidateage44.646us,RFsubmit54.928us,RFUSBcallback503.489usmean/max1131,
p99UPPER1000us;candidate37190,ACK36906,busy284(.764%),retries17. No liveprocess;
installedv15TIMINGDISARMED,userinstructedwired. Deadline03:30AM.
FourrunABBAprivate latency-abba-20261117.json: RFUSBmeans v15run1=.609ms,
v16run1=.525,v16run2=.583,v15run2=.503. Fastreactioncandidateage21.6-24.8usvs
42.9-44.6us repeats,butwholelatencyrangesoverlapandpairedbenefitreverses. Busy
fast.303%-6.18%,baseline.764%-2.03%. NOconsistentwhole-latency/backpressurebenefit;
KEEPnormalv14stable. Do notpromotefastfromthesedata or treatpacketcountasbootN.
NEXT collectpendinguserfeel/stopreport,thenfreshNICENANOreadyRESTOREEXACTtested
normal93ebbefore03:30. No morediagnosticruns/codechanges thissession. Save/push
fourrunresultandrestoration/concisehandoff. FuturemeasureUSBframephaseanddevice
motion/button-edgeconservationbeforemoreoptimization;4h/8hsoaksstillpending.

V15 RUN2 LIVE - 2026-11-17 03:06 Manila:
FreshOFF USBunplugged/samesetupready. LIVEexec27549;privatebase
v15-latency-20261117-030634-b8ad37cb. OneCDC+100sforegroundraw,NOarm;cueONprovided.
Same d50squietprobe,x70s/poststopd. Do notopenanotherreader/rearm/reset.
Exactbase.stop-requestifabort. NEXTpoll27549/analyze+userreport;fourper-runcomparison;
thenphysicalNICENANOreadyRESTOREtestednormalv14before03:30AM. No morediagnostic
runsplanned. Installedv15autoactiveuntilx,thenDISARMED. Deadline19:30ZNov16.

V15 RUN2 FLASH VERIFIED - 2026-11-17 03:05 Manila:
FreshNICENANOreadywired/OFF;board/S140/exactd7516017UF2hash/rangeverifiedthenflashed.
ProductDIY Mouse Timing v15verifiedwithoutserial. Privatetiming-v15-replication-flash.json.
InstalledNOWv15timingautoactive/listening;v16notinstalled. No liveprocesses.
NEXT separateOFF USBunplugged/samesetupreadiness,then100scaptureExpectedProductv15
FinishBeforeUtc2026-11-16T19:30Z. No newbuild/code. Afterresult/userreportcompare
fourper-runmetricsandRESTOREtestednormalv14-93ebbefore03:30AM. No normalfast
promotion. Currentstate supersedesv16-installedhistory. Deadline/restoreunchanged.

V16 RUN2 PASS / V15 RUN2 FLASH NEXT - 2026-11-17 03:03 Manila:
UserSmooth/cleanstopped/noissue. exec2275complete;40272acceptedHID/USBsamplesmatch
40272receiver-onlyforegroundrecords/errors0. Active d silent,onex,frameallchecks
PASS;failed/capped/trackingfault0. Privatebasev16-latency-20261117-030043-9903f36e
finalsummarysaved. No liveprocess;installedv16TIMINGDISARMED,userinstructedwired.
V16run2means:candidateage24.787us,RFsubmit34.988us,RFUSBcallback583.143us;
RFUSBmax1098us,p99UPPER1098us (notexactpercentile). ACK46945,candidate50039,
retries10,busy3094=6.18% ofcandidates. Run1v16busy.303% andmean525.125us;substantial
USB-sidevariation confirmsneedrepeatbaselinebeforeclaimingoverallbenefit. Fast
CPUreactionseemslowerthanv15run1butnolongrun/wholemousecausalclaim.
NEXT freshNICENANOreadywired/OFF flashEXACTv15d7516017baselineimage,verifyproduct,
separateinputOFF USBunpluggedreadythen100scaptureExpectedProductv15Finish19:30Z.
Reuseexistingimages/tools,nobuilds. ThencomparePER-RUNfourresultsandRESTOREtested
normalv14-93eb987abefore03:30AM. No newdiagnosticflashesat/after03:15ifdelayed;
controllerlateststart03:22. Companionappstilldeferred. Contextupdated.

V16 RUN2 LIVE - 2026-11-17 03:00 Manila:
FreshOFF USBunplugged/samesetupready. LIVEexec2275,privatebase
v16-latency-20261117-030043-9903f36e. OneCDC+100sforegroundraw,NOarm;cueONprovided.
Sameactive d50squietprobe,x70s/poststopd. Do notopenanotherreader/rearm/reset.
Exactbase.stop-requestifabort. NEXTpoll2275/analyzeframe+userreport;thenv15run2
viafreshNICENANOready;restoretestednormalv14before03:30. No builds/codechanges.
Installedv16autoactiveuntilx;willbedisarmedaftercapture. Deadline19:30ZNov16.

V16 REPLICATION FLASH VERIFIED - 2026-11-17 03:00 Manila:
Freshphysicalreadinesswired/OFF;uniqueNICENANO/S140/hash/UF2checksPASS;exactfrozen
v1655a371eaflashed. USBproductDIY Mouse Timing v16verifiedwithoutserial. Private
timing-v16-replication-flash.jsonsaved. InstalledNOWv16timingautoactive/listening;
normalv14notinstalled. No livecapture/serial/build/helper. Deadline03:30AM.
NEXT separateOFF USBunpluggedreadiness,samesettings/otherpointer/window;100scapture
ExpectedProductv16,FinishBeforeUtc2026-11-16T19:30Z. Thenvalidate+saveuserreport,
freshNICENANOreadyflashfrozenv15forsecondbaseline,thenrestoretestednormal93eb.
No code/buildchanges. Don'tstartcaptureat/after03:22;skipdiagnosticflashesafter
03:15ifdelayedandrestore. Currentstate supersedesnormal-installedhistory.

RESUMED UNTIL03:30 / REVERSE-ORDER REPLICATION NEXT - 2026-11-17 02:57 Manila:
User extends sessionto03:30AMManilaNov17 (UTCNov16T19:30Z),superseding03:00pause.
Installednormalv14restoredanduserwirelessconfirmationPASS(dfca95f). No live serial/
raw/build/helper/UAC. Gitcleanexceptunrelated.vscode/INFO_UF2.TXT. Branch
feature/receiver-fast-poll-latency. Exactpreservedv16/v15/normalhashesreverified.
NEXT reuse SAME frozen diagnostic images in reverseorder:v16(10us) secondrun,
thenv15(50us)secondrun,samesurface/DPI/foregroundmotion/100scapture. Reason:
replicatefirstA/B andcheckorder/USB-phasevariation;notunnecessaryfunctionalbaseline.
No newcode/build needed. Eachflash requiresfreshNICENANOreadyGloriouswired/OFF;
image/board/USBproductverify. Eachinputtrial separateOFF USBunpluggedreadiness,
oneCDCreader/noarm,cueON,activewindowsame45smotion,x70s/poststopd. Analyzecounts/
timecomponents;capturemetadata+userfeel separately. ExpectedProductv16thenv15.
FinishBeforeUtc2026-11-16T19:30Z;controller8minmargin meansnostartat/after03:22.
Conservativelystopnewdiagnosticflashesat03:15ifdelayed;restoretestednormal93eb
instead. MUSTrestoreexacttestednormalv14afterreplicationbefore03:30break;no
normalfastpromotionyet. ComparePER-RUNmetrics/ranges ratherthan treatingthousands
ofpacketsasindependentboottrials. Two runsperconditionstillnotlong-runproof.
V16image55a371eacf874e9d2aa8bb47bdedc586ca67d4ae08128bf229bf51e75b828951;
v15d751601753a9be286dc806ecee902f0a3d4a1170b9891cc56680f37519dd06b7;
normal93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
Preservedfilenamesandbaselinecapturepathsinfollowingcheckpoint. Appdeferreduntil
AFTERgamingoptimization. Keepcontext/resumecurrentandpushtestedmilestones.

RESTORED NORMAL V14 WIRELESS CHECK PASS - 2026-11-17, final break checkpoint:
User confirms restored wireless operation works normally after USBunplug/mouseON,
movement/click on harmless document,without arm command. This resolves the prior
pending restoration-check popup. Installedtestednormalv14 (93eb987a fullhash in
following checkpoint) remains automatic/independent;user may keepusing wirelessly.
No live capture/serial/build/helper/UAC and no further tests tonight. Timingv15/v16
remain saved diagnostic candidates,NOTinstalled;fastpollnotpromoted. All results/
copy-pastehandoffsaved;pausebefore03:00AMManilaNov17. Refresh workwindow onresume.
Next:repeatability/soak/gamingqualification,appafteroptimizations. Currentstate
supersedesolderpendingrestorationconfirmation/history.

NORMAL V14 RESTORED / READY FOR 03:00 BREAK - 2026-11-17 02:54 Manila:
Exact hardware-tested normalv14 image restored at02:50 after fresh userNICENANO
readiness (Gloriouswired/OFF),board/S140/hash/UF2checks. USBproductDIY Mouse Dongle
v14,HID/CDCverified WITHOUTserial/arm. Private normal-v14-restored-after-timing.json.
InstallednormalSHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
normalautomaticoperationuntilunplug. NOTdiagnostic/disarmed/passive/expired.
Brief ordinary-use restoration-check popup remains pending;USB restore verification
passed,mouse's current cable/power not independently verified. No need reflash or
repeatlongbaseline. User may keep using normalwireless operation after projectpause.
No live serial/raw/build/UAC/helper sessions. Untracked.vscode/INFO_UF2.TXTuntouched.

Currentbranchfeature/receiver-fast-poll-latency. Tested comparisonaac3941pushed;
softwareb7c9ceb;v15baseline8a3c63d. V15/v16 diagnostic images preservedNOTinstalled.
V15(50us)37597accepted/device/foregroundhostrecordsallmatch,RF->USBcallbackmean
608.802us,max1153us,p99UPPER1000us,candidateage42.949us,busy2.03%,retries15.
V16(10us)36065matchingrecords,RF->USBcallbackmean525.125us,max1107us,p99UPPER1000us,
candidateage21.612us,busy.303%,retries2. Bothquietactive/nativecounts/reconciliation
checksPASS;usersmooth/cleanstopinboth. One manualA/B:USBphase/workloadvariation
prevents causal/longrunclaim. These instrumented receiver components exclude sensor,
earlierqueue/retries,OS/game/display;percentilesarehistogrambounds. Fastpoll NOT
promotedtonormal. Normal50uspathunchanged. Private summaries/comparison retained.

Pause work before03:00AMManilaNov17,asrequested;normaldonglemayremainordinaryuse.
NEXT explicitresume:refreshworkwindow;collectpendingbriefrestorationconfirmation
ifstillneeded;readgaming-qualification/latency-diagnosticsplans. RepeatA/Bunder
controlledmotion/load/USBphase;assessmeans/tails,busy/retries andhostcadence. Only
thenprepareoptimizedNORMALcandidateandqualifylongerbeforepromoting. 4h/8h/multi-day
soaks,Windows sleep/wake/RFrobustness andsustained1kHzqualification stillpending.
No needless extraction/build/flash/baseline repeats;no newexperiment thissession.
Userlowlatency/reliability/1kHz priorities persist. Companionapp DPI/lighting/buttons
explicitlyDEFERREDuntilAFTERgamingoptimization;trueDPI/lightingneedprotocol/hardware
support. No mouse-settingwrites attempted. Keepprivatecaptures/addresses/binaries
ignored and context/resumecurrent,pushtestedmilestones,featurebranches. Usepopups
forphysicalintervention. Refreshdeadlineonlaterday;03:00authorizationexpires.
This restoredstate supersedes all older LIVE/timing-installed/pendingrestorehistory.

NORMAL V14 RESTORED / USB VERIFIED - 2026-11-17 02:50 Manila:
UserNICENANOreadyGloriouswired/OFF. Verifiedboard/S140/exactTESTEDnormal93eb987a
fullhashand181blockUF2thenrestored. USBproductDIY Mouse Dongle v14,HID/CDCstatus
verifiedWITHOUTserial/DTR/arm. Private normal-v14-restored-after-timing.jsonsaved.
InstalledNOWnormalv14autoactive/listening,NOTtimingv16/disarmed. Diagnosticimages
v15/v16preservedbutnotinstalled;fastpollNOTpromoted. No livecapture/serial/build/
UAC/helper. Comparisonaac3941pushed;v15baseline8a3c63d;v16softwareb7c9ceb.
NEXT shortordinary-useconfirmationpopup(optionaltestrestoredoperation),savefinal
context/resumeandpushthenpauseBEFORE03:00AMManilaNov17. Usermayusewirelessnormally
withoutCodex/serial/arm;releasebuttonsfirstifUSBunplugging. No newexperiments.
Measurementnextresume:repeatA/Bundercontrolledload/USBphase,preservecoverage/tails/
busy;thenqualifyfasterNORMALcandidatewithlongerreliabilitybeforepromoting. Device
RF->USBcallbackcomponentmeans0.609ms(v15)vs0.525ms(v16)singlemanualA/B;notfullmouse
latency/causalproof. CompanionappDPI/lighting/buttonsdeferreduntilAFTERgamingwork.
Long-termsoaks/sleeprecovery/gaming1000Hzqualificationremainpending. Deadline3AM
expiresafterthissession;asknewworkwindowonlaterresume. Currentstate supersedes
olderLIVE/timing-installed/pendingrestorehistory.

V16 FIRST COMPARISON PASS / NORMAL RESTORE READINESS PENDING - 2026-11-17 02:48 Manila:
UserSmooth/cleanstopped/noissue. exec84480complete,100sraw36065receiverrecords,
ALLforegroundcode0/errors0;matchesacceptedHID/USBsamples36065. OneCDC,noarm,onex,
active d silent,poststopframeallintegritychecksPASS;failed/capped/trackingfault0.
Privatebasev16-latency-20261117-024522-caa71a4b finalsummarysaved;comparisonJSON
v15-v16-latency-comparison.jsonignored. No livecapture/serial/build/helper.
V16mean candidateage21.612us(v15=42.949),RFsubmit30.914us(v15=52.458),
RFUSBcallback525.125us(v15=608.802),max1107us(v15=1153),p99UPPER1000usboth.
V16reports36065,retries2,busy150/49516candidates(.303%);v15busy1089/53635(2.03%).
ACKready187.540us/ACKend231.789usmeans. Counts/componentsreconcileall6metrics.
One manualA/Bonly:USBphase/workloadvariationcanexplainpartoftotalmeanchange;
NOcausal/longrun/fullsensor-to-screenlatencyclaim. Do notpromotefasttonormalyet.
InstalledNOWv16TIMINGDISARMEDafterx;normalv14NOTyetrestored. RestorepopupPENDING:
Gloriouswired/OFF,double-resetreceiverNICENANO. OnReady verifyboardandEXACTtested
normalimagebuild-pairing-analysis/dongle-v14-93eb987a.uf2 SHA
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;copythenUSBproduct
DIY Mouse Dongle v14,HID/CDCverifyWITHOUTserial. Normalautoactivatesuntilunplug.
RestoreBEFORE03:00AMNov17 UTCNov16T19:00Z;no new experiments. Thenoptionalshort
ordinary-useconfirmation,pushresult/context/conciseresume,andpauseprojectwork.
Currentbranchfeature/receiver-fast-poll-latency;softwareb7c9cebpushed,measurement
stillneedscommit. Native9/parser2/controllergatespassedbeforelive,h/wnowpassed.
Workingnormalv14preserved;companionappdeferreduntilAFTERgaming/reliability.

V16 COMPARISON LIVE - 2026-11-17 02:45 Manila:
UserfreshOFF USBunplugged/samesettings/otherpointerready. OneCDC+100sforeground
capturestarted,NOp. LIVEexec84480;privatebasev16-latency-20261117-024522-caa71a4b.
CueONprovided;d50squietprobe,x70s,poststopd. Do notopenanotherreader/rearm/reset.
Exactbase.stop-requestifabort. Expectedend~02:47:10. Deadline03:00. NEXTpoll84480,
analyzeframeanduserfeel,compareagainstv15baselinewithoutcausal/longrunclaim.
ThenphysicalNICENANOreadinessrestoreTESTEDnormalv14UF293eb987aBEFORE03:00.
Installedv16timingautoactiveuntilx;captureleavesdisarmed. No normalfastpromotion.

V16 TIMING FLASHED / USB IDENTITY PASS - 2026-11-17 02:44 Manila:
FreshNICENANOreadywired/OFF;timebefore02:48verified. Board/S140/UF2exact55a371ea
fullhashverifiedthenflashed. ProductDIY Mouse Timing v16 readbackPASS;no serial
opened/armcommand. Private timing-v16-flash-validation.jsonsaved. InstalledNOW
v16timingautoactive/listening;v15notinstalled. No liveprocess. Deadline03:00AM.
NEXT separateinputreadiness(mouseOFF USBunplugged,otherpointer,sameDPI/surface),
thenexisting100scapture -ExpectedProduct 'DIY Mouse Timing v16' withfinish19:00Z.
Do notstartat/after02:52. Comparewithv15baselineprivatev15-latency-20261117-023337-d3bd3453.
ThenRESTOREtestednormalv14-93eb987aviafreshphysicalNICENANOreadyBEFORE03:00.
No extra build/flashofcompletedcandidate;no normalfastpollpromotionfromoneA/B.

V16 FAST-POLL SOFTWARE READY / FLASH NEXT IF TIME FITS - 2026-11-17 02:42 Manila:
Branchfeature/receiver-fast-poll-latency. App latency-fast changesONLYdiagnostic
pollwait50->10us;FASTflagrequiresLATENCY;normal/defaultpathstill50us. Timingcore,
RFbytes/ACKramp/mappingunchanged. All9nativecheckspass;build/config/UF2/stagingpass.
Finalimage55a371eacf874e9d2aa8bb47bdedc586ca67d4ae08128bf229bf51e75b828951,
49316flash/16448RAM,193blocks0x26000..0x32100exclusive. Privatepreserved
build-pairing-analysis/timing-v16-55a371ea.uf2;softwaremeta v16-software-validation.json.
ProductDIY Mouse Timing v16,all5featureflagsON,PRINTK/UARTconsole/bootbannerOFF.
ParentcaptureExpectedProductv16supported,versionedprefix;SelfTest/syntaxPASS.
Build99344complete;NOliveprocesses. InstalledSTILLtimingv15DISARMED,notnormalv14.
Baselinev15hardwarepass8a3c63dpushed;RFUSBmean608.802us/max1153/p99upper1000,
candidateage42.949us,busy1089/53635candidates,reports37597exactlyraw/samples.
NEXT physicalNICENANOreadinesswired/OFF,verifyexactv16image/boardbeforeflashONLY
ifbefore02:48. Thenproductverify/separateOFF USBunpluggedinputreadiness,100squiet
capture -ExpectedProduct 'DIY Mouse Timing v16' -FinishBeforeUtc2026-11-16T19:00Z.
OneCDC/noarm;x70s/poststopd. Do notstartat/after02:52. Comparemeans/tails/busyand
hostcadence;manualmotion/USBphasedifferencesmeanoneA/BisNOTcausalorlongrunproof.
RESTOREtestednormalv14UF293eb987a viafreshphysicalreadinessBEFORE03:00break.
Do notpromotefastpolltonormalfromonebench. Ifdelayedskipcandidateandrestorev14.
No morebuildsneeded;keepprivatecaptures/addresses/imagesignored/contextcurrent.

V16 FAST-POLL DIAGNOSTIC BUILD LIVE - 2026-11-17 02:39 Manila:
V15baselinehardwaremeasurementPASSEDandsaved/pushed8a3c63donfeature/receiver-latency-diagnostics.
Currentbranchfeature/receiver-fast-poll-latency. Installedtimingv15DISARMEDafterx;
userreportsSmoothlikenormalv14/cleanstopped;Gloriousinstructedwired. NoCDC/rawlive.
NewcandidatechangesONLYdiagnosticRXpollpause50->10us (FAST_POLL_DIAGNOSTICScompile
flagrequiresLATENCY_DIAGNOSTICS). Normal/defaultpathsretain50us. SameRFbytes,
ACKramp,USBmapping,timingcollector/dumpprotocol. App latency-fast productDIY Mouse
Timing v16;newseparatebuildfolder. ParentcaptureacceptsexplicitExpectedProductv15/
v16only,versionedfileprefix;SelfTest/syntaxPASS. Nativechecksrunbybuildscript.
LIVEBUILDexec99344 latency-fast. No v16flash/measurementyet;normalv14rollback93eb
andbaselinev15d751preserved. Deadline03:00AMManilaNov17 UTCNov16T19:00Z.
NEXT finishbuild/config/staging/UF2validation/preserve,pushsoftwarethenphysical
NICENANOreadinessifclockfits. Do NOTflashv16at/after02:48;restorev14instead.
Capturecontrollerrequires8minmargin(no startat/after02:52). Need100sforemotion
trialwithv16expectedproduct,compareallmeans/tails/busy/retries,notjustCPUmean.
ThenfreshphysicalNICENANOreadinessRESTORETESTEDnormalv14before03:00. Do notpromote
fastpolltonormalwithoutrepeatability/longerreliabilityevidence. Ifdelayedskipnew
trialandrestorev14. CompanionappdeferreduntilAFTERgamingwork. Contextcurrent.

V15 FIRST LATENCY MEASUREMENT PASS - 2026-11-17 02:35 Manila:
UsermotionSmoothlikenormalv14;stoppedcleanly. exec63319complete;100sforegroundraw
37597receiverrecords/errors0,ALLcode0;oneCDCreader,NOp,onex,dactiveprobeSILENT,
frozenpoststopsummaryvalid. AcceptedHID37597 matchesraw37597 andallUSBsamples.
Parserallhist/count/sum/component/ACK/busychecksPASS;failed/capped/trackingfault0.
RFEND->USBcompletionCALLBACKmean608.802us,max1153us,p99UPPER1000us(135of37597>1ms).
RFEND->submitSTARTmean52.458us,max185us. SubmitSTART->completionmean556.344us,
max1085us,p99UPPER1000us. CandidateageallACKcandidateframesmean42.949us,max179us.
ACKREADYmean207.954us/max362;ACKENDmean252.202us/max406. SuccessfulACK52546,
candidate53635,retries15,busy1089;reconciliation53635=52546+1089. Rawall37597code0.
Privatebasev15-latency-20261117-023337-d3bd3453 finalsummarysaved. No liveprocess.
Installedtimingv15NOWDISARMEDbyx;Gloriousinstructedwired. NOTnormalcontinuousstate.
Measurementsincludeinstrumentation+USBcallbackdispatch,andlastacceptedRFattempt;
exclude sensor/priorqueue/retries/OS/game/display. PercentilesbucketUPPERbounds.
No claim of totalmouse1mslatency or1000Hzcertification. Rollbacknormal93ebpreserved.
NEXT smallA/Bdiagnosticcandidate:reduceONLYdiagnosticpollpause50->10us,keepnormal
codepath50/RFbytes/ACKramp/mappingunchanged. ObserveCPUreaction/busy/tailtradeoffs;
notassumebenefitfrommeanalone. Separatebuildproductv16;newflashreadinessandshort
sameforemotioncapture. Needrestoretestednormalv14BEFORE03:00AMbreak regardless.
Do notstartnewcaptureat/after02:52;deadlineUTC2026-11-16T19:00Z. Ifdoesnotfit,
restorev14withoutnewcandidate. Apps/DPI/lightingremainexplicitlydeferred.

V15 TIMING TRIAL LIVE - 2026-11-17 02:33 Manila:
FreshreadinessconfirmedmouseOFF USBunplugged/otherpointer/samesettings. Controller
runningoneCDCreaderDTR+100sforegroundraw,NOp. LIVEexec63319;privatebase
v15-latency-20261117-023337-d3bd3453. CapturecuebeforemouseON. Active d at50s
expectsnoserialtext;x70s;poststopdquery. Usersteadycircles/strokes~45swindowactive.
Do notopenanotherreader/rearm/reset. Exactbase.stop-requestifuserrequestsabort.
Expectedend~02:35:20;resultpending. NEXT poll63319,analyzecompleteframevia
research/analyze_latency.py,collectfeel/stopreport,save/push. ThenphysicalNICENANO
readinessrestoretestednormalv14before03:00. Installedtimingv15autoactiveuntilx;
parentcapturewillleavedisarmedafterstop. No normaloperationclaimafterdiagnostic.

V15 TIMING FLASHED / ENUMERATION PASS - 2026-11-17 02:32 Manila:
UserphysicalNICENANOready/GloriouswiredOFF. VerifieduniqueNICENANOboard/S140and
exactfinald751601753a9be286dc806ecee902f0a3d4a1170b9891cc56680f37519dd06b7UF2,
valid193blocks0x26000..0x32100then copied. USBproductDIY Mouse Timing v15 plus
HID/CDCverified. Private timing-v15-flash-validation.jsonsaved. NOserialopened/
armcommand. InstalledNOWtimingv15autoactive/listening,NOTnormalv14/passive.
No livebuild/serial/raw/helper. HardwaretimingdataNOTyetcaptured. Software70dc90b
pushed. Deadline03:00AMManilaNov17. MustRESTOREtestednormalv14beforebreak.
NEXT separateinputpopup:GloriousOFF USBunplugged,harmlessdoc/otherpointer,same
surface/DPI/settings. Runresearch/capture_latency_session.ps1 -FinishBeforeUtc
2026-11-16T19:00Z. OneCDCreader+100sforegroundRawInput,noarmcommand. CueONthen
activatewindowwithOTHERpointer,motion~45s. Active d50s silent;x70s;frozendumpafterx.
Analyzevalidframeonly;collectuserfeel/stoppopup;thenfreshNICENANOreadinessrestore
build-pairing-analysis/dongle-v14-93eb987a.uf2 (fullhashinpriorcheckpoint),verify
USBproductDIY Mouse Dongle v14. No extra builds/flashes needed for completedsteps.

V15 QUIET TIMING SOFTWARE READY / FLASH NEXT - 2026-11-17 02:28 Manila:
Deadlineextended03:00AMManilaNov17 (UTCNov16T19:00Z). Branchfeature/receiver-latency-diagnostics.
InstalledhardwareSTILLworkingnormalv14autoactive;NOdiagnosticflashorlivetrialyet.
No livebuild/serial/raw/UAC/helper. Normalrollback93eb987a fullhashpreserved.

Finaltimingv15UF2d751601753a9be286dc806ecee902f0a3d4a1170b9891cc56680f37519dd06b7,
49304flash/16448RAM,193blocks,0x26000..0x32100exclusive. Privatepreserved
build-pairing-analysis/timing-v15-d7516017.uf2. Finalstagedsourcesmatchall6headers/
main;NORMAL_DONGLE/LATENCY_DIAGNOSTICS/WIRELESS_HID/DISCOVERY_PROBE ON. Product
DIY Mouse Timing v15;PRINTK/UARTconsole/bootbannerOFF,HID/REBOOT/UARTinterruptON.
NormalcontrolbuildpassedLATENCYOFF,hashf594d2bf988896be858d3885e1ffb20e3a51a1fe6d4edc0077a2f7bb32f511b3;
notinstalled;restoreTESTED93ebimageafterdiagnostic. Builds67436/30244complete.
All9nativechecksPASS(protocol3modes,bridgewith/withoutdiag,collector/writer).
Researchparser2tests+invalidcasesPASS;controllerSelfTest/syntaxPASS,liverununtested.
Coreinitialemptyhistfailedcountassertbeforeimplementation. Softwaremeta
v15-software-validation.jsonprivate. Detailsdocs/plans/2026-11-17-latency-diagnostics.md.

Fixed6histograms/9bins/1Msamplecap,IRQlockedinflightstate,async+earlyUSBcallback,
failed/shortwrite exclusion,clockwrap,neutralexclusion,stop/drain tested. No text
whileactive. xstopscollectionafterneutral;dONLYpostdisarm+nopending outputsframe,
nonblockingFIFO+DTR+1.5sdeadline. Footerreports/retries/busyvalidatecoverage.
Metricsareinstrumented RF END->submissionSTART/USBcompletionCALLBACK andACKready/
end;NOTsensor/debounce/earlierqueue/retry/OS/game/displaylatency. Quantilesbucket
UPPERbounds,mean/maxmeasured;observeroverheadincluded. No performancefixyet.

NEXTphysicalpopup:Gloriouswired/OFF,double-resetONLYreceiverNICENANO. Verifyboard
andEXACTfinaltimingimagebeforecopy;diagnosticAUTOACTIVATES,nopassive/unarmedclaim.
Verifyproduct.Then separateinputreadiness(mouseOFF USBunplugged,harmlessdoc/other
pointer,sameDPI/surface). Use research/capture_latency_session.ps1 withfinishUTC
2026-11-16T19:00Z;oneCDCreader,100sforegroundraw,NOarmcommand. Actualcapturecue
beforemouseON;useractivateswindow/moves~45s. d at50sactiveexpectsSILENCE;x70s;
frozendumpafter250ms,up to3drequests. Perjob.stop-request sendsxonetimeearly.
Do notopenanotherreader,rearmorresetduringcapture. Analyzeviaanalyze_latency.py;
requirevalidfooter/hist/count/component reconciliation anduserreport. Restore
preservednormalv14aftermeasurementviafreshphysicalreadinessBEFORE03:00break.
AppforDPI/lighting/buttonsrequestedBUTexplicitlydeferreduntilAFTERgamingwork.
GloriousCOREdoesnotgetofficialreceiverinterfacefromourstandardHID;wiredmousewas
previouslydetected. No proprietarymouse-settingwritesorappimplementationdone.
Keepprivatecaptures/addresses/imagesignored;context/resumecurrent/pushtestedsteps.
Currentreadystateabove supersedesolderLIVE/buildhistory. No morebuildsneeded.

QUIET LATENCY SOFTWARE / BUILDS LIVE - 2026-11-17 02:15 Manila:
All9nativechecksPASS,protocolprobe/receiver/dongle andbridgebothdiagnosticsmodes.
New latency_metrics.h/coretests:fixed6histograms/9bins,1Msamplecap,sum64,IRQlocked
singleinflighttimestampownership,callbackbeforereturn/async/duplicates/failedshort
writes/clockwrap/stopdrain/overlapfault tested. latency_dump.h outputsframedstats
ONLYaftercollectstoppedandpendingfalse;DTR+1.5swhole-dumpdeadline,nonblockingFIFO,
partialwrites/DTRloss/stall/activeguardtestsPASS. Coretestinitialemptycollector
failedexpectedcountassertbeforeimplementation. No perpackettext/alloc.
Integrated RFcandidateagehookfromTIMER2PPIEND,CC3CPUclock,USBsubmitstart/completion
beforesemgive,andACKREADY/ENDhardwaretimestamps. Neutralnotmeasured;newsession
cannotresetpending. Markdisarmstopscollection;dcommandonlywhenneutraldisarmhandled.
App latency NORMAL_DONGLE+LATENCY_DIAGNOSTICS ON,printk/UARTconsole/bootbannerOFF,
productDIY Mouse Timing v15. Normal Appdongle explicitLATENCYOFF stays independent.
LIVEBUILDexec67436: building latency then normal dongle control. Nohardwarechange:
installedworkingnormalv14 stillautoactive,UF293eb987a preserved. No live serial/
raw/UAChelper. Deadline03:00AMManilaNov17 UTCNov16T19:00Z.
NEXT finishbuild/config/staging/UF2hashpreserve/push,thenflashreadinesspopup with
Gloriouswired/OFF (diagnosticimagealsoautoactivates). Prepareone-reader100sraw+
CDCcapture,actualimage/productverified,noarmcommand,quietactivewindow,xthenpoststop
dframe. Recordinstrumentationoverhead/scope:RFEND->USBDcompletionisnotphysical
sensor->displaylatency. CompanionappexplicitlydeferreduntilAFTERgamingwork.

LATER COMPANION APP REQUEST - 2026-11-17: user wants a receiver app for DPI,
lighting and buttons,but explicitly defers it until AFTER gaming optimizations.
Do not implement app now. Receiver button remapping is feasible;true sensorDPI/
lighting depend on actual mouse hardware/proprietary command support. Keep focus
on quiet latency measurement/reliability. No new mouse-setting writes authorized
or attempted in this step.

RESUMED UNTIL03:00 / QUIET LATENCY DIAGNOSTICS NEXT - 2026-11-17 01:59 Manila:
User extends work to03:00AMManilaNov17 (UTCNov16T19:00Z),superseding02:00cutoff.
Prioritylowlatency/reliability/consistent1000Hz. Branchfeature/receiver-latency-diagnostics.
Installednormalv14 remainsworking/autoactive;NOlivehardwaretool,buildorcapture.
Planopt-inApp latency (NORMAL_DONGLE+LATENCY_DIAGNOSTICS),preservev14normalbinary.
UseexistingTIMER2PPI RFEND/READY capture;CC3free for synchronized CPU timestamps.
Fixed6histograms:candidateRFage,RF->submitstart,submitstart->USBcompletion,
RF->USBcompletion,RF->ACKready,RF->ACKend. No perpackettext/alloc/hotpathdump.
MeasureUSBcompletioncallbackbeforesemgive;handlecallbackbeforereturnfromwrite,
asynccompletion,failed/shortwrite,busy,neutralexcluded,32bclockwrap andstatscap.
Smallirq_lock sections protectinflightstats;dumpONLYafterxneutralandnoinflight.
KeepPRINTK/UARTconsole/bootbannerdisabled;explicitpoststopd command usesbounded
nonblockinguart_fifo_fill withDTR/deadlinechecks. No output whileactive. Normal
v14imageandv13rollbackpreserved;no speculativeoptimizationsbeforemeasurements.
Neednativebehaviorregressions/core+bridgehooks,hardwaretimingadapter,buildflag/
App latency/productv15,SDKbuild/config/UF2verify,pushsoftwarethenphysicalflashpopup.
Newdiagnosticimageautoactivates,soGloriouswired/OFFforflash;laterharmlessdocmotion,
xstop,dumpandrawhostcapturewithoneCDCreader. Do notuseoldv13controller.
Coremetricsdonotprovephysicalsensor->screenlatency;observeroverheadmustbeacknowledged.
Usermaykeepusingnormalmouseuntilphysicalflashneeded. Keepcontext/resumecurrent.

GAMING/RELIABILITY PRIORITY SAVED - 2026-11-17, before02:00AM pause:
User wants long-term reliability and especially gaming capability/1000Hz if
possible,and asks which gaming features can be added. Preserve working normal
v14;no firmware/settings/flash/serial/capture changes in this planning step.
Next plan docs/plans/2026-11-17-gaming-qualification.md:verifyactual USB1ms
configuration and controlledforeground consistency,then quiet device-side timing
histograms/counters before targeted IRQ/hardware-timing changes. Distinguish fresh
report delivery from polling and end-to-end latency. Validate fast motion/input
edges,USB/mouse/system sleep recovery and practical RF robustness. Proposed4hour,
then8+hour/multi-day soaks NOTyetperformed. DPI/debounce/lift-off/batteryfeatures
need supported mouse-side protocol;receiver scaling is not true DPI. Userordinary
wireless use may continue. Currentnormalv14 remainsinstalledautoactive/listening,
no live tools/readers/helpers/builds. Latestnormalhardwarepass704c7c7pushed.
NEXT resumeafterbreak:refreshworkwindow,readqualificationplan,and implement
measurement step conservatively. Do not repeat extraction/build/flash/baselines
without reason. Existing02:00AMNov17 deadline expires;do not reuse in laterday.

V14 PLUG-AND-PLAY HARDWARE PASS / BREAK HANDOFF - 2026-11-17 01:51 Manila:
User confirms: all worked automatically and smoothly, including receiver USB
unplug/replug with mouse left ON, both mouse-only OFF/ON cycles, and controls at
16/18min beyond old limit. User additionally reports mouse STILL WORKING after
capture ended. This is intended: normal firmware runs independently until unplug,
without Codex, serial reader or arm command. User may keep using it wirelessly.
Do not stop/reflash/reset it merely because recording or the work session ended.

First normal trial:1160s mouse-only recording exit0,44814 receiver-only background
records,read_errors0/failed0. Receiver input span1131.448104s (18m51.448s);
11522 records after15min,8365 after16min,2710 after18min fromfirst receiver event.
LEFT213down/213up,RIGHT9/9,wheelUP107/DOWN132. Middle/thumb events absent in this
run;their deliberate v13 coverage and unchanged mapping remain separate evidence.
No CDC/DTR/arm command or Windows/DPI setting change. No active serial/raw/build/
UAC/helper processes. Normalv14 installed and autoactive/listening while connected;
NOT unarmed/passive/expired. Mouse is currently being used wirelessly per user.
Private base v14-normal-20261117-012742-226176cb with complete CSV/metadata/summary.
USB replug/cycles are user-confirmed; LastArrivalDate predates capture and does
not independently establish their timing. Background arrivals do not certify
physical polling/loss/latency. No claim of long-term reliability from one normal run.

Current branch feature/receiver-plug-and-play. Software3f5934c,flash/enumeration
withoutseriala05de2f,recordingfd4c32bpushed;nowrecordconfirmedhardwarepass. Normal
image SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
private preserved dongle-v14-93eb987a.uf2,46124flash/16064RAM,181UF2blocks,
0x26000..0x31500exclusive. V13rollback396d7dd8 remains preserved. Do not flash
bounded receiver-v14 image by mistake; normal uses build.ps1 -App dongle.

CORE NORMAL-DONGLE BEHAVIOR NOW USER-CONFIRMED. Automatic startup follows USB
configuration, no DTR/manualarm/30sdiscovery/15min/ACK/discoverycap;mouse link gaps
neutralize and rediscover. USB restart can automatically start normalmode again,
per user's latest request overriding earlier passive/finite preference. Optional
x stop/p restart is native-tested but NOT separately hardware-validated in normal
mode; use physical mouseOFF or receiver unplug for immediate stop when needed.
Abrupt unplug while held can leave Windows held state (also wired baseline);
release buttons before unplug. No speculative startup-neutral fixes needed.

Pause project work before02:00AMManilaNov17,as requested;dongle may remain in
ordinary use. NEXT on explicit project resume: refresh available work window,
review user experience from ordinary use;investigate only observed problems or
plan measured gaming latency/long-term/RF robustness qualification. Do not repeat
completed extraction/build/flash/functional baselines without reason. Old bounded
v13 serial-controller MUST NOT be run against silent normalv14. Use raw-only
capture for further normal-use observation unless deliberately changing mode.
Keep private captures/addresses/images ignored,context and copy-paste handoff
current,and push tested milestones. Unrelated .vscode/ and INFO_UF2.TXT untracked.
This current completed state supersedes earlier LIVE/pending/passive history.

V14 NORMAL RAW CAPTURE COMPLETE / USER REPORT PENDING - 2026-11-17 01:47 Manila:
exec74926exited0;1160srawonly,noCDC/DTR/armcommand.44814records,allreceiver/all
backgroundcode1,read_errors0/failed0. CompleteCSVvalidated;monotonictimestamps.
Receiverfirst28.314609s,last1159.762713s,span1131.448104s(18min51.448s).
11522recordsafter15minfromfirst;8365after16min;2710after18min. LEFT213down/up,
RIGHT9down/up,wheelUP107/DOWN132. No middle/sideevents inthisrun (v13all5baseline
preserved;donotclaimindependentv14rawcoverageforthose). No captureerrors.
Privatebasev14-normal-20261117-012742-226176cb summarypendinguserreport. USBsnapshot
LastArrival17:26:32 predatescapture;cannotindependentlyproveplanned1minreplug.
Usercompletion/observationspopupsstillpending;needautomaticinitialstartup,
receiverreplugwithmouseON,bothmouseOFF/ONcycles,controlsafteroldlimit/stalls.
No live serial/raw/build/helper/UAC. NORMALv14 remainsautoactive/listening while
USBconnected;captureendDIDNOTstopit. UserinstructedmouseOFForUSBwiredfornow;actual
posttrialmousepower/cableNOTverified. Do notassumeexpired/unarmed/passive.
Deadline02:00AMManilaNov17. NEXT collectreportthenpushconfirmedresult;iftheUSB
replugwasnotperformed,recordthatscopeandaskseparateshortreplugreadinessratherthan
repeatlongtrial. No newbuild/flashneeded. V13rollbackpreserved;normalimage93eb987a.

V14 NORMAL LIVE UPDATE ~01:33 Manila: raw-only exec74926 still active, file growing, stderr empty. NO CDC/DTR/arm commands. Read-only USB snapshot saved; LastArrivalDate predates capture and must NOT independently prove timed replug. User replug/powercycle/feel report still pending.

V14 NORMAL LIVE UPDATE ~01:38 Manila: raw-only exec74926 running, receiver-labelled movement verified (firstevent28.314609s afterrawstart), stderr empty/file growing. No serial/DTR/arm command. Ten-minute mouse cycle due; user observations/replug confirmation still pending. Normal firmware stays active after capture.

V14 NORMAL LIVE UPDATE ~01:44 Manila: exec74926 still recording; receiver event timestamp984.334777s observed in growing file versusfirst28.314609s (>15min span). Streaminglastrowcanbepartial;parseonlyafterclose. No serial/arm command. User16/18minchecks/replug/cyclereportsstillpending. Captureend~01:47:02;normaldonglecontinuesbeyondcapture.

V14 NORMAL 18-MINUTE TRIAL LIVE - 2026-11-17 01:27 Manila:
Userconfirmedharmlessdoc/otherpointer/GloriousOFF USBunplugged/unchangedsettings.
Raw-onlycapture1160sstarted;NOCDCreader,NO DTR,NO armcommand. LIVEexec74926,
privatebasev14-normal-20261117-012742-226176cb. Expectedcaptureend~01:47:02Manila.
Loggerhashverified09d2e29c90ff0e5e363381c7412b389d11251c862a2a30ac3972ce143eeeab6d.
UsercueON/start18mintimer;allcontrols;releaseALLbuttonsandreceiver-onlynormalUSB
replugaround1minwithmouseON;mouse-onlyOFF3s/ONaround5/10min;controlsat16/18min.
At18minmouseOFF/reconnectUSB. NORMALFIRMWAREDOESNOTTIMEDSTOP,andcaptureclosure
DOESNOTstopreceiver. Do not openserial/arm/reflash/resetduringtrial. IfuserStop,
confirmphysicalmouseOFForreceiverunplugged;capturemayfinishorbeclosedpartial.
Metadata.capture.jsoninitialsuccessfalseuntilrawcompletion;dataallignored.
NEXT poll74926/read-onlyfiles,collectautomaticinitialstartup/replug/bothmousecycles/
continuedafterold15minlimit andnountestedcontrols. Nohardwarepassyet. Deadline
02:00AMManilaNov17 (UTCNov16T18:00Z). Normalv14installedautoactive/listening;
previousunarmed/passivestateshistorysuperseded. Flashmilestonea05de2fpushed.

V14 NORMAL FLASHED / USB ENUMERATION PASS - 2026-11-17 01:24 Manila:
PhysicalreadyGloriouswired/OFF. VerifieduniqueNICENANOnRF52840-nicenano/S1406.1.1
andnormalUF293eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
beforecopy. USBrootproductreadback DIY Mouse Dongle v14,oneHID+oneCDC,statusOK.
Private dongle-v14-flash-validation.json saved. NO serial reader opened,NO DTR/
armcommand. InstalledhardwareNOWNORMALv14;automaticreceiverlisteningexpected,
actualRFinput NOTyettested. Notunarmed/passive/expired. V13rollbackpreserved.
No live build/serial/raw/helper/UAC. Branchfeature/receiver-plug-and-play,software
3f5934cpushed. Deadline02:00AMManilaNov17 (UTCNov16T18:00Z).
NEXT separateinputreadinessfor18minnormaltrial:harmlessscrollabledoc/otherpointer,
GloriousOFF/USBunplugged,unchangedDPI/settings. Receiverleaveconnected. Raw-only
capture1160s(hidden,mousereadonly,NOCDC/arm). CueONthenallfivebuttons/wheel/motion.
At~1min RELEASEallbuttons thenunplug/replugONLYreceivernormally(no double reset),
mouseON;verifyautomaticcontrolrecoverywithoutserial. Mouse-onlyOFF3s/ONat5/10min.
Checkcontrolsagainat16and18min,thus>15minafterreceiverreplug. At18minmouseOFFand
reconnectUSB;normaldongledoesNOTautostopwhencaptureends. NoheldUSBunplugtrial.
Needstartby01:37tofit1160s+wrapby02:00;otherwisepausewithoutnewtrial. Usemanual18min
timer;oldfive-minuteHTMLtimerunsuitable. Collectusercontrols/automaticstartup/
USBreplug/bothmousecycles/continuedactivityafteroldlimit/nounexpectedstops.
Rawcapturefilehostcountsarenotphysicalpolling/latencycertification;normalmodehas
noserialdiagnosticsbydesign. Do not useboundedv13serialcontrolleronthisimage.

V14 PLUG-AND-PLAY SOFTWARE READY / FLASH NEXT - 2026-11-17 01:15 Manila:
User explicitly requests normal dongle operation: USB plug automatically activates
until unplug. This supersedes previous finite-only/manual-start and passive USB
reconnect preferences FOR THE NEW NORMAL BUILD. Current branch:
feature/receiver-plug-and-play. Installed hardware STILL v13, disarmed/neutral;
no v14 flash/arming/hardware trial yet. No live build/serial/raw/UAC/helper session.

New App dongle / NORMAL_DONGLE=ON starts after USB configuration without DTR or
host command. No discovery-arm timeout, session duration, ACK or discovery cap.
One-second successful-ACK gap neutralizes then keeps rediscovering. Optional x
stays stopped until explicit p or reboot. USB reset/reconnect can autoactivate
again; USB/radio/HID errors fail closed. Telemetry wrapping does not affect link.
Normal overlay disables UART console, printk and boot banner to avoid CDC output
blocking with no reader. CDC input is optional. RF bytes/timing, mapping and idle
ACK hold are unchanged. Bounded App receiver remains available and now permits
explicit fresh p only after completed neutral stop; no active-session renewal.

Tests: new expiry rearm regression failed on v13; normal-mode regression failed
old 30s guard before implementation. All8 tests PASS, protocol subtests cover
probe/receiver/dongle, no-command startup, offline/fault gates, old limits,
telemetry wrap, gap recovery, x latch, fatal TX and neutral-failure cases. All four
firmware builds PASS. Final staged sources/config match; NORMAL only ON for dongle.
Normal PRINTK/UART_CONSOLE/BOOT_BANNER disabled, HID+REBOOT enabled, interval1ms.
Normal UF2 SHA256 93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052
46124 flash/16064 RAM,181 blocks,0x26000..0x31500 end exclusive. Preserved ignored
build-pairing-analysis/dongle-v14-93eb987a.uf2. Earlier ddd815ea image was superseded
by final USB-reset-during-init fix; DO NOT flash that stale image.
Bounded v14 receiver SHA256
5aadca1a29a17d8fc25f022d4309e24159fd12d7aa25a1bd06882b25b42b3696,
62268 flash/16128 RAM,244 blocks,0x26000..0x35400; preserved separately. Probe
SHA e51374e433090fadbecf3092a3f76e002bef742279d062cd535dac5965d0b26f;
radio SHA014953ed153a2cc50ca0577278be7fbd0154961faedb86233c57230b2c169438.
Default HID hash unchanged. V13 rollback396d7dd8 full hash verified preserved.
Private software metadata v14-software-validation.json ignored. Build15557 and
final normal rebuild25478 finished; no more builds needed for current sources.

NEXT physical flash popup: Glorious wired/OFF, double-reset ONLY receiver to
NICENANO. Verify unique bootloader/board + exact normal image before copy. The new
image AUTOACTIVATES; do not claim passive/unarmed startup. Verify USB enumeration
and product DIY Mouse Dongle v14 without opening CDC. Then separate harmless-doc/
other-pointer readiness, mouse OFF/USB unplugged; raw-only host capture, no serial
reader or arm command. Test automatic controls/powercycle then safe USB replug,
all buttons released first. Longer normal trial must exceed old15min window.
Old capture_receiver_session.ps1 targets bounded v13 and MUST NOT be used for
normal v14; normal has intentionally no serial diagnostics. Save/push every tested
milestone and keep private images/captures ignored. Deadline02:00AM ManilaNov17,
UTC2026-11-16T18:00Z; refresh if later. Abrupt unplug held-state known from both
DIY and wired reference; no physical polling/latency or long-term certification.

V14 PLUG-AND-PLAY BUILDS IN PROGRESS - 2026-11-17 ~01:07 Manila:
User explicitly requests USBplug->automaticactivationuntilunplug;newnormalmode
supersedespriorfinite/passivepreference. Branchfeature/receiver-plug-and-play.
NORMAL_DONGLE added as separateAppdongle;boundedreceiver/probe/radio retained.
Normalmain waitsUSBconfiguredinsteadDTR,startsreceiverwithoutCDCcommand,hasno
30sdiscovery/15min/ACK/discoverycaps;gapneutralizesandrediscoverycontinues. Optional
x stops/latches;explicitp/rebootrestarts. USBresetrestartcanactivateagain normalmode.
USB/radio/HIDerrorsfailclosed. Unsignedtelemetrywrapdoesnotstopnormalmode.
Normaloverlay UART_CONSOLE=n/PRINTK=n/BOOT_BANNER=n preventsCDCoutputblocking.
SameRFbytes/timing/mapping/quietactive. Sharedv14explicitrearmresetsper-session
counters/dedupaftercompletedneutralstop;pending/failedstopandUSBfaultsblockp.
TDD: rearmexpiryregressionfailedonv13;continuousmodefailedold30sarmguard;nowall8
nativeunittestsPASSwithprobe/receiver/donglesubtests. Newarmexpiry/failuregatesalso
passedbybuildtest. Bridgefault/resetchecksPASS. HardwareNOTchanged:stillv13disarmed.
LIVEBUILDexec15557:Appdongle thenreceiver/probe/radio,verifycompletion/artifacts.
A one-line mainnormalinitialization reset-requestservice fix was added AFTER first
donglestaging began;must ensurefinalbuildstagingmatchescurrentmain/rebuilddongleif
needed (do notflasholdimage). No live serial/raw/UAChelper. Deadline02:00AMManila.
Need completebuild/config/staging/UF2hash/privatepreservevalidation,README/context/
resumeupdateandpushsoftwarecheckpointbeforephysicalflashpopup. Newnormalimage
AUTOACTIVATES,soGloriouswired/OFFbeforeverifiedNICENANOcopy. Testnormalstartupwith
NOCDCreader/NOarmcommand,harmlessdoc/otherpointer;collectallcontrols/powercycle/
USBreplugresults. Do not callnormalstartupunarmed/passive. V13rollbackpreserved.
Oldcapture_receiver_session.ps1 expectsboundedv13 andmustnotbeusedagainstnormal
image(no serialdiagnosticsbydesign). Raw-onlyhostcapturecanmonitornewimage.

USER CHANGES TARGET TO PLUG-AND-PLAY - 2026-11-17 ~01:04 Manila:
User asks receiver automaticallyactive fromUSBpluguntilunplug. This supersedes
priorfinite-only/passivestartselectionfornewNORMALbuild. Agentacknowledgedautomatic
startup,noCDCwindowrequired,continuousoperation/mousepowercycle recovery,emergencyx.
Currentbranchfeature/receiver-plug-and-play;priorfeature/receiver-explicit-rearm
createdbutNOcommit/build/image/flashthere. Workingdiffcontainsv14explicitrearm
sharedlogic+tests:originalexpiryregressionfailedonv13;all8nativecheckscurrentlyPASS.
NOTfullyvalidatedv14;nohardwarechange. InstalledimageSTILLv13DISARMEDneutral,
no live process. Deadline02:00AMManilaNov17 (UTCNov16T18:00Z).
NEXT separateAppdongle normalbuildusingexistingreceiverlogic/compileflag;retain
boundedreceiver diagnostic variant andv13rollback. MustremoveDTRstartupdependency
andavoidCDCconsoleblockingwithoutreader. ProposednormaloverlayPRINTK=n,
UART_CONSOLE=n,BOOT_BANNER=n;CDCcommandsstilloptional. InspectSDKdriver/config.
NormalmodeautoarmsafterUSBconfigured;discoverywait/session/ACK/discoverylimits
mustnotstopnormaluse;gapneutralizesandkeepsrediscovering;USBresetmayrestartnormal
modeautomatically(usernewpreference),faultsfailclosed. xlatcheduntilfreshp/replug.
Testactualguards/autoarmoffline/USBfault/noautoresumeafterx/numericalbudgetwrap;
keepboundedreceiver/probechecks. No giantfakebudgetstostandforcontinuousmode.
Needfinishplan,testsfirst,builddongle+receiver/probe/radio/config/UF2validation,
preservetestedimage,pushbeforephysicalflashpopup. Gloriouswired/OFFbeforeflash
becausenormalimagewillautoactivate. LaterverifyNOserialreader/armcommandrequired,
allcontrols/mousecycles,andUSBreplugwithsafephysicalreadiness. Neverflashblindly.

V13 INTENDED DESK POSITION PASS / NEXT MILESTONE CHOICE - 2026-11-17 00:52 Manila:
User confirms receiverinintendednormaldeskposition,normalmousegrip/spot,smoothmotion
andcleanstop;distance/obstaclesunknown. Full100sserial/rawexit0,39356receiverrecords
ALLforeground,readerrors0/failed0. Onep,onelink,onex,neutralstop/disarm,noERROR.
Hostmedian1003us,p90=1222us,p99=4893us,max418994us;peak1000events/fullsecond,
23of55interiorbins950..1050events. Manualmotionnotinstrumented;hosttimingisnot
physicalpolling/latencyorinterference/rangecertification. Final58610ACKs,
39360HIDreports,8retries,1471busy. Raw/HIDtotalsdiffer4;donotcallthatRFpacketloss.
Privatebasev13-motion-20261117-004932-493d1a24 summarywithuserreportignored.
exec1572completed;no live serial/raw/UAC/helper/build. Installedv13DISARMEDneutral,
onearmused;normalrebootbeforefuturearm. GloriousinstructedreconnectUSB,cablenot
independentlyverified. No firmware/settings/DPI changes. Deadline02:00AMManila
November17 (UTC2026-11-16T18:00Z). Branchfeature/receiver-foreground-timing.
Completedbaseline:all5buttons/wheelvalidated,two15minrecoverytrialsandforeground
motion/intendedplacementpassed. Preservev13image/guards;no needrepeatthosechecks.
NEXT userpreferencepopup: prioritize convenient REPEATED EXPLICIT Start/Stop,
keepingfiniteboundsandpassiveUSBreconnect (wouldrequirecarefullytestedrearmstate
changefromonearmperboot),or device-sidetiminginstrumentationforperformancework.
No newrearmbehavior/firmwareimplemented or armed. Untilpreferencearrives,do not
silentlyremoveonearmguardoraddautoarm/unboundedmode. Hardwareinterventionwill
stillneedpopupforanyfutureflash/inputtrial. CurrentstatesupersedesLIVEhistory.

V13 DESK-POSITION CAPTURE LIVE - 2026-11-17 00:49 Manila:
Usernormaldeskreceiverposition/freshnormalreboot/GloriousOFF USBunpluggedconfirmed.
Distance/obstaclesnotprovided. Onep/newactualarmconfirmedbeforeONcue. LIVEexec1572;
privatebasev13-motion-20261117-004932-493d1a24. Existing100sforeground/autox60slink.
Useractivateswindowwithotherpointer,motion45-60snormalspot/grip. No firmware/settings
change. Do not reopenserial/rearm/reset. Stopfileexactbase.stop-requestifabort.
NEXT poll1572,collectstalls/overshoot/cleanstoppopupanddistanceifknown,save/push.
Deadline02:00AMManilaNovember17. Earlierdisarmedstatehistorysupersededwhilelive.

V13 ALL FIVE BUTTONS VALIDATED - 2026-11-17 00:47 Manila:
User confirms requestedmiddle/frontthumb/rearthumborder,cleanstop,no stuckbuttons.
Full100s raw/serial exited0:25819 receiverrecords,allforeground(code0),read_errors0/
failed0. Middle5down/5up,side2=5/5,side1=5/5;oneextraLEFTdown/up. Temporalorder
plususerconfirmation mapsfrontthumb->Windowsbutton5(side2),rearthumb->button4(side1).
Onep,onelink,onex,neutralstop+disarm,noERROR. Final59898ACKs,25821HIDreports,
23retries,919busy. Raw/HID totals differ2;do not equate reportcounts with physical
packetloss. Privatebasev13-motion-20261117-004417-1a641303 summarywithmappingignored.
Togetherwithpriorleft/right/wheeltests,allfivebuttons+wheelup/downnowdeliberately
validated. No firmware/build/flash/Windows/DPI changes. exec13642finished;no live
serial/raw/UAC/helper/build. Installedv13DISARMEDneutral;normalrebootfornewarm.
UserinstructedreconnectUSB. Branchfeature/receiver-foreground-timing;deadline02:00
AMManilaNovember17. Preservev13UF2396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc.
NEXT intendeddesk-placementRFcheck onunchangedv13:receiverinintendeddailyposition,
sameDPI/surface/settings,otherpointer/harmlesscapturewindow. Normalreceiverreboot,
GloriousOFF USBunplugged;onearmactualconfirmbeforeON. Existing100sforeground
controller/autox60s. Usersteadycircles/strokes45-60snormalgripfromnormaldeskspot,
reportstalls/overshoot/cleanstoptogetherwithapproxreceiverdistance/obstacles.
This checksonepracticalplacement,notcertifiedrange/interference/gaminglatency.
No need repeatcompleted wired/extraction/build/flash or longrunbaseline.
CurrentcompletedstatesupersedesearlierLIVEhistory. Save/pushthenphysicalpopup.

MIDDLE/THUMB BUTTON CAPTURE LIVE - 2026-11-17 00:44 Manila:
Fresh readinessconfirmed;unchangedv13;onep/newactualarmingconfirmedbeforeONcue.
LIVEexec13642,privatebasev13-motion-20261117-004417-1a641303. Existing100s foreground
controllerusedforbuttoncoverage;autox60sfromfirstlink. Do not reopenserial/rearm.
Useractivateblankcapturewindowwithotherpointer;middle5/frontthumb5/rearthumb5.
No firmware/settingschanges. Resultpending. Abortexactbase.stop-request ifneeded.
NEXT poll13642,analyzemiddle/side1/side2 down/up and temporalorder;collectuser
performed/cleanstopreport,save/push. Deadline02:00AMManilaNovember17.

SECOND V13 FIFTEEN-MINUTE RELIABILITY PASS - 2026-11-17 00:41 Manila:
User confirms all controls smooth after BOTH mouse-only powercycles;original15min
cleanstop. Full935s serial/raw exited0,75552 receiverrecords/allbackground,errors0.
Onep/zerox,three links/three neutralstops,noERROR;hostobserved899945ms (~900s).
Final879210/983040 ACKs;480531HIDreports,784retries,30952busy;idle397891/status4/
type1=481315. LEFT658down/658up,RIGHT68/68,wheelUP217/DOWN548. No middle/sideflags.
Privatebasev13-session-20261117-002340-2dc17d49 with finalsummary/userreportignored.
Across two separate fifteen-minute trials:four planned mouse-only recoveries,
all userconfirmed smooth and originaldeadlinecleanstop;30min total boundeduse,
NOT one continuous30min session. Foregroundmeasurement4d9a89f passed separately.
No live readers/logger/UAC/helper/build. Installedv13 now expiredneutral,onearm
used;normalreboot beforefuturearm. No firmware/Windows/DPI changes. Userinstructed
reconnectUSB,cablenotindependentlyverified. Deadline02:00AMManilaNovember17.
Branchfeature/receiver-foreground-timing. NEXT focusedmiddle+thumbbuttonphysical
check usingexisting100s -ForegroundMotion controller (recordsbuttons,onep,one x
at60slink,full100sraw). Reason:fivebitmappingnativeverified,onlyleft/right/wheel
physicallytested. Normalreceiverreboot,GloriousOFF/USBunplugged,otherpointerready.
ActualarmcuebeforeON;activateblankRawInputwindowwithOTHERpointer andkeepactive.
Press/release middle5times,frontthumb5,rearthumb5 separately,thenleft/right/wheel
ifcomfortable;noDPI. CheckRawInputmiddle/side1/side2 balancedtransitions and user
stuckstate separately. No browsernavigationtest/no new firmware/build/flash.
CurrentcompletedstatesupersedesearlierLIVE/pendingentries. Save/pushandpopup.

SECOND V13 CAPTURE COMPLETE / USER REPORT PENDING - 2026-11-17 00:39 Manila:
exec79121 exited0;full935s serial/raw,75552 receiver-only backgroundrecords,zero
read errors/failed0. Onep/zerox,three links/three neutralstops/noERROR;hostobserved
session899945ms (~900s). Final879210ACKs/480531HIDreports/784retries/30952busy.
LEFT658down/up,RIGHT68down/up,wheelUP217/DOWN548. No middle/sideflags. Private base
v13-session-20261117-002340-2dc17d49 with savedsummary pendinguserobservation.
User selected completed;posttrial smoothness/recovery/stop reportpopup pending.
No live serial/raw/helper/build. Installedv13 sessionexpiredneutral;rebootrequired
beforeanotherarm. Current state supersedes LIVE history. Deadline02:00Manila.
NEXT collectpendingreportthenpushrepeatabilityresult. Remaining deliberatebutton
coverage:middle(wheelpress),two thumbbuttons;nativefivebitmapping tested,but not
physicallyprompted. Use existing100s ForegroundMotion controller for focused
buttoncapture,x60s;activateblankRawInputwindowwithotherpointer beforetesting,
avoidbrowsernavigation. Need normalreboot/OFF USBunplugged readiness first.
No newfirmware/build/flash or extra capturefeature needed.

SECOND RUN LIVE UPDATE ~00:30 Manila: exec79121 active,six minutes recorded,two links/one neutral stop/no ERROR after first planned mouse-only cycle. Onep/zerox;usercontrol/feel observations pending.

SECOND RUN LIVE UPDATE ~00:35 Manila: eleven minutes recorded,three links/two neutral stops/no ERROR after second planned mouse-only cycle. exec79121 active;onep/zerox. Finaldeadline/userreportstillpending.

SECOND V13 RELIABILITY TRIAL LIVE - 2026-11-17 00:23 Manila:
Fresh physical readiness confirmed normal reboot/mouseOFF USBunplugged/document.
Default935s controller started,one p/new actual arming confirmation before ON cue.
LIVE exec79121;private base v13-session-20261117-002340-2dc17d49. Do not open another
reader/rearm/reset. Expectedcaptureend~00:39:18;original15min fromfirstlink.
User normalcontrols+mouse-onlyOFF3s/ON at5/10min. No firmware/settings changes.
No passed result yet. Abort exactbase.stop-request triggerssinglex/partialcleanup.
NEXT poll79121,collectuserresults,compare fullcounters/stop timing,save/push.
Deadline02:00 AM Manila November17 UTC2026-11-16T18:00Z. Prior foreground4d9a89f
andfirstlongrunfb26dad passed/pushed. This live state supersedesdisarmedhistory.

V13 FOREGROUND MOTION PASS - 2026-11-17 00:21 Manila:
User:Smooth like wired;stopped cleanly. Full100s raw/serial exit0,39210 receiver
records,ALL code0 foreground,read_errors0/failed0. One p,one link,one x,one neutral
linkstop+disarm,no ERROR. x sent60027ms after host-observed link;disarm chunk25ms
later (host timestamps,not device precision). Native controller mode hardware PASS.
Foreground median gap1007us,p90=2016us,p99=4926us,max939042us.56 interior one-second
bins;peak988events/s,4 bins950..1050.39208 motion records,maxabsX36/Y22. Firmware
58809 ACKs,39210 HIDreports,39 retries,535 busy;idle19559/status1/type139249.
User confirmed capture window active. Reused older wired/v8 foreground reference;
manual motion not matched,so do NOT rank latency/loss or diagnose maxgap as RF stall.
Host data supports near1000event/s capability in parts of run,not certified physical
1000Hz or end-to-end latency. No firmware/settings/DPI changes or injected input.
Private base v13-motion-20261117-001840-d854159f (csv,serial,journal,stdout/stderr,
capture.json,summary.json),ignored. exec23691 complete;no live serial/raw/helper/
UAC/build. Installedv13 now DISARMED/neutral;normal reboot required for new arm.
Glorious instructed reconnectUSB;cable not independently verified. Deadline02:00 AM
Manila November17 (UTC2026-11-16T18:00Z). Branch feature/receiver-foreground-timing.
NEXT repeatability: second bounded15min normal-use/recovery trial on samev13,
using existing default935s controller;reason is repeated reliability beyond single
long-run pass,not a new build/baseline. Need new normal reboot/input readiness pop-up.
No flash. Keep same surface/DPI/settings and harmless doc/other pointer. Mouse-only
OFF3s/ON around5/10min;all controls/smoothness/original15min cleanstop. Existing
foreground test establishes host capacity;defaultlongrun backgroundcannotcertifypolling.
No15min arm at/after01:42. Keep private evidence ignored and push tested result.
EarlierLIVE entries are history;currentcompletedstate here supersedes them.

V13 FOREGROUND CAPTURE LIVE - 2026-11-17 00:18 Manila:
User confirmed fresh normal receiver reboot/GloriousOFF USBunplugged/other pointer.
Controller -ForegroundMotion started;fresh v13 verified,one p,actual arming
confirmed before powerON popup. LIVE exec23691,private base
v13-motion-20261117-001840-d854159f.100s raw+serial;auto x at60s from first link.
Do NOT open another reader/rearm/reset. User activates visible capture window,
continuous circles/strokes45-60s;no clicks/wheel/DPI/powercycle. Await completion
and user observations;not yet a passed measurement. Deadline02:00 Manila.
If user requests stop,create exact private base.stop-request (one x,partial capture).
NEXT poll23691,analyze code0 host timing with manual-motion limitations,save/push.

V13 FOREGROUND CONTROLLER READY / PHYSICAL SETUP NEXT - 2026-11-17:
Branch feature/receiver-foreground-timing. Current deadline02:00 AM Manila today
(UTC2026-11-16T18:00:00Z). No hardware access,build,flash or arming this session.
Existing capture_receiver_session.ps1 now accepts -ForegroundMotion:100s serial/
raw,existing visible Raw Input window,one x after60s from first link,confirmed
neutral/disarm and full raw completion. Default935s fifteen-minute mode unchanged;
conservative18min deadline margin retained in both modes. SelfTest passes original
deadline/single-stop plus exact60s motion deadline/no-link/disabled cases;syntax
and diff checks PASS. New live mode NOT hardware-tested. No firmware changes.
Reuses prior wired foreground reference;do not repeat extraction/build/flash/wired
baseline. NEXT pop-up: receiver normal unplug/replug (not double-reset),Glorious
OFF/USBunplugged,unchanged DPI/surface/settings,other pointer. Keep OFF until new
actual arming confirmation. Then ON,use other pointer to activate capture window,
move continuous circles/strokes45-60s without buttons/wheel/powercycle. Keep window
active until automatic close;expect x stop around60s after link. Capture lasts100s.
Analyze code0 only,report manual motion/focus limitations;no physical1000Hz/latency
claim from host arrival alone. Old v13 fifteen-minute result preserved.

RESUMED / V13 FOREGROUND TIMING NEXT - 2026-11-17 00:12 Manila:
User resumes and sets pause time2 (interpreted/announced02:00 AM Manila today,
UTC2026-11-16T18:00:00Z). Yesterday18:00 deadline expired. Git clean except
unrelated untracked .vscode/ and INFO_UF2.TXT; latestfb26dad pushed. No hardware
access,arming,build,flash or live capture has occurred in resumed session.
Installed image last verifiedv13; last session expired/neutral,one arm used.
Current physical connection/state not reverified since break.
Reviewed context,README,radio analysis,original wireless HID plan,15min plan,
foreground capture plan,existing C logger/controller and prior timing summaries.
Reuse prior wired foreground reference raw-wired-foreground-20261116-002746;
no repeat of completed wired baseline. V8 foreground capture50748records/allcode0,
median hostgap1001us and several~1000event/s seconds. This is host delivery,
NOT physical polling/latency certification. V13 full935s recording was entirely
background(code1),so cannot establish current foreground delivery rate.
NEXT focused v13 motion-only foreground timing,unchanged firmware: normal receiver
reboot;GloriousOFF/USBunplugged,same surface/DPI/settings,other pointer available.
One serial reader/freshv13 banner/onep/actualconfirm before mouseON. Show existing
Raw Input window;user activates with other pointer and makes steady circles/strokes
~45-60s,no clicks/wheel/DPI/powercycle. Explicit x around60s after first link;
full100s serial/raw captures with confirmed neutral/disarm,not full15min validation.
Need prepare bounded controller by reusing existing session and xstop patterns,
with deadline/one-stop checks;no firmware build or flash needed. Pause via readiness
pop-up now;do not arm based on yesterday's readiness. Save controls/feel separately
from host statistics;foreground(code0) only for rates/gaps;manual motion gaps are
not proof of RF loss. Finish/save/push by02:00. Existing v13 pass remains baseline.

V13 FIFTEEN-MINUTE TRIAL PASS / READY FOR BREAK — 2026-11-16 17:50 Manila:
User confirms all controls worked smoothly after BOTH mouse-only OFF3s/ON cycles,
receiver untouched, and clean original15min stop. Full935s serial AND Raw Input
completed exit0;92400 receiver-only records/read_errors0/failed0 (all background).
One p,zero x,three link starts/three neutral stops,no ERROR. Host-observed first
link09:34:06.773UTC -> final stop09:49:06.705UTC (~900s;host chunk timestamps are
not firmware-level timing). Final883050/983040 ACKs;658536 HID reports,438 retries,
47107 busy;idle=224074/status=2/type1=658974. LEFT381down/381up,RIGHT86/86,wheelUP135/
DOWN262. Raw coalescing means these counts do NOT certify1000Hz or physical loss.
Private base v13-session-20261116-173338-5a62c157 (.txt/.csv/.host.jsonl/.capture.json/
.raw.stdout.txt/.raw.stderr.txt/.summary.json),all ignored. No private addresses
or binaries committed. All live readers/logger/build/UAC/helper sessions CLOSED;
exec55451 completed. Installed v13 remains connected,session EXPIRED/neutral,
one-arm-per-boot budget used;do NOT rearm without normal reboot and fresh setup.
Mouse instructed reconnect USB;user report does not independently verify cable.
Image396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc.
Branch feature/receiver-fifteen-minute-session. Previous milestones fa9936c software,
5e1ca7e flash/passive startup pushed. Context/resume updated with full result.
No further15min arm fits current18:00 stop (cutoff17:42 already passed). Pause
hardware work here;user may use Glorious wired. NEXT on explicit resume: refresh
user deadline, review this result and plan repeated bounded normal-use validation
or measured gaming-performance work;do not rebuild/flash/repeat completed baseline
without reason. Preserve v13 as tested finite-session baseline. No unlimited mode.
Remaining: repeated longer reliability,passive USB restart regression for this exact
v13 image if needed,physical polling/latency/channel/IRQ work. V11 recovery,x stop
and held-timeout previously tested;v13 long session does not independently repeat
every safety case. Abrupt physical unplug held state matched wired reference on
this PC/app;wired PnP restart not compared. Do not revive startup-neutral guesses.
Current state above supersedes earlier LIVE/IN PROGRESS/history entries below.

LIVE UPDATE ~17:40 Manila: session55451 still running; six minutes captured, two link starts/one neutral link stop/no ERROR after first planned mouse-only cycle. User controls/feel not yet collected. No extra p/x sent.

LIVE UPDATE ~17:45 Manila: eleven minutes captured; three link starts/two neutral stops/no ERROR after second planned mouse-only cycle. Session55451 continues; no extra p/x. User controls and final deadline observations pending.

V13 LIVE TRIAL IN PROGRESS — 2026-11-16 17:33 Manila:
User confirmed normal receiver reboot, Glorious OFF/USB unplugged and harmless
scrollable doc/other pointer. Controller started one serial reader/DTR,drained
fresh v13 banner,started935s raw capture and sent exactly one p. Actual arming
response confirmed BEFORE mouse-ON pop-up. LIVE exec session55451; private base
v13-session-20261116-173338-5a62c157. Do NOT open another serial reader or rearm.
Expected capture end~17:49:20 Manila; final active deadline15min from first link.
User asked mouse-only OFF3s/ON around5/10min, controls/consistency/finalstop.
To abort: create exact private base.stop-request; controller sends x ONCE then
closes partial capture. No UAC/helper. Deadline remains18:00; pause afterward.
NEXT monitor session55451 with bounded polls, read private logs if needed without
opening device; collect user results after capture. If context is lost,inspect
process/artifacts before acting;never blindly restart controller. Trial NOT passed
yet. Previous flashed/passive milestone5e1ca7e pushed; this live state local only.

V13 FLASHED / PASSIVE STARTUP PASS — 2026-11-16 17:27 Manila:
Verified unique NICENANO board nRF52840-nicenano/S1406.1.1 and exact preserved
v13 UF2 SHA256396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc
before copying. Fresh v13 banner on COM12;8s passive capture PASS, no ERROR,
no arm/link/disarm. No serial command written. Reader closed. Private artifact
receiver-v13-passive-20261116-172732.txt/.json. Installed hardware NOW v13,
UNARMED with unused budgets (983040 ACK attempts/900000ms/8 discoveries).
No live serial/raw/build/UAC/helper session. Glorious user-confirmed wired/OFF.
NEXT separate15min trial readiness: normal receiver unplug/replug (fresh banner
needed after passive reader); harmless scrollable document/other pointer; mouse
OFF/USB unplugged, unchanged DPI/surface/settings. Keep OFF until actual arm cue.
Use research/capture_receiver_session.ps1 -FinishBeforeUtc 2026-11-16T10:00:00Z.
Full935s serial/raw; one p, confirm before mouse ON. Controls and mouse-only
OFF3s/ON around5 and10min; observe original15min stop. Stop-request=x once.
No arm at/after17:42;save/push and pause by18:00. Full v13 trial still UNTESTED.

V13 SOFTWARE CHECKPOINT / FLASH READINESS NEXT — 2026-11-16:
User authorizes work until18:00 Manila; do not arm15min trial at/after17:42.
Branch feature/receiver-fifteen-minute-session. V13 restores v11 bridge/main/HID
harness (e106a79), removes ineffective v12 startup-neutral experiment and changes
only receiver cap983040/900000ms/banner. All8 native tests PASS; receiver/probe/
radio builds complete. Staged receiver sources match. CONFIG_REBOOT=y and both
receiver features ON. Receiver UF2 SHA256
396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc;
61816 flash/16128 RAM,242 blocks,0x26000..0x35200 end exclusive.
Preserved ignored receiver-v13-396d7dd8.uf2. Probe/radio/default HID hashes match
v11 controls. Installed hardware STILL v12, physically unplugged; no live build,
serial/raw/UAC/helper session. No v13 flashing/arming occurred.
Raw logger max now1200s; warnings-as-errors compile/native self-test PASS.
1200 boundary accepted then safely refused existing output;0/1201 rejected;
marker unchanged. One-second hidden lifecycle PASS, raw_exit0 (private smoke
wired-usbloss-smoke-20261116-171316-d29d03e5). New executable SHA256
09d2e29c90ff0e5e363381c7412b389d11251c862a2a30ac3972ce143eeeab6d.
Sandbox native launch was blocked; same self-test passed outside sandbox.
New research/capture_receiver_session.ps1: full935s serial+raw, fresh v13 passive
banner, one p/actual arming confirmation before mouse ON; explicit finish deadline
and18min margin; stop-request sends one x; early abort partial; cleanup on error.
Deadline/single-stop self-check and syntax PASS. Full935s capture NOT yet tested.
NEXT physical pop-up: Glorious wired/OFF, reconnect receiver then double-reset
NICENANO. Verify board/image before copy, passive startup no arm. Then separate
15min input readiness: harmless scrollable doc/other pointer, Glorious OFF/USB
unplugged, same DPI/settings. Trial controls and mouse-only OFF3s/ON around5/10min;
observe original900s stop, note overshoot/stalls. Controller deadline UTC10:00
November16. If time no longer fits, pause without arm. Save/push result and stop
by18:00. Unexpected USB removal held state matched wired physical-unplug baseline;
wired PnP restart not compared. Connected x/timeout release remains validated.

WIRED USB-LOSS CONTROL MATCHED / PAUSED AHEAD17:10 BREAK 2026-11-16:
User confirms wired Glorious also kept selecting after physical USB unplug with
LEFT held;cleared afterward,other pointer normal. DIY receiver unplugged throughout.
Private base wired-usbloss-20261116-165945-b57d350d:60s read-only capture1458
wired-only raw records/errors0,one LEFTdown/no LEFTup. Host LEFTFalse initially,
True~24.527s,False~53.924s after cleanup;one later click transition. No RF/serial/
PnP/UAC,input injection or Windows setting change. All processes closed.
INTERPRETATION:physical receiver-removal behavior MATCHES wired reference on this
PC/app. The expected automatic release on abrupt unplug was not established by
baseline;do not call it a DIY-specific physical-removal bug. This is a scoped
local observation,not universal Windows behavior. Wired PnP restart was NOT
compared,so don't generalize that exact scenario. V12 startup-neutral did not
change the observed PnP held state;no demonstrated benefit for that symptom.
Known connected x/timeout neutral-release checks still passed;keep those guards.
Pause now ahead of user17:10 break;resume ONLY on user request. No more firmware
changes/live trials this session. Current branch feature/physical-usb-loss-validation.
Installed/source/staged image remains v12 SHA256
1e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30;receiver physically
unplugged. Glorious USB may be reconnected for ordinary wired use;not independently
checked after instruction. No live serial/raw/helper/build/UAC sessions.
NEXT WHEN RESUMED:consolidate findings and choose smallest stable baseline before
longer normal-use testing. Do not add more firmware release guesses for physical
unplug behavior. Review whether to drop v12 startup-neutral experiment (no benefit
shown for target symptom) in favor of preserved v11;no rollback/build/flash done.
A planned host restart can be made graceful by x+confirmed neutral before PnP,
but that is not unexpected-loss release and not newly hardware-tested. Keep the
remaining PnP-specific limitation scoped. Then plan finite longer-use validation
with explicit stop/time/reconnect limits and fresh physical readiness. No unlimited
mode or daily-use/gaming latency certification. Refresh user deadline after break;
old17:06/17:10 UTC guards must not be blindly reused in a later session.

WIRED USB-LOSS REFERENCE PREPARED 2026-11-16: physical receiver unplug
also retained LEFT per user;host cleared,other pointer normal. No firmware
changes. Added research/capture_wired_usb_loss.ps1:60s native raw+read-only host
LEFT state,NO serial/RF/PnP/UAC. One-second wrapper/native lifecycle selfcheck
PASS(records0/errors0,not movement evidence);expired break deadline refusal
PASS before capture. Script requires FinishBeforeUtc and60s wrap margin.
NEXT physical readiness:leave DIY receiver UNPLUGGED,connect Glorious by USB in
WIRED mode,same selectable text/other pointer,mouse cable reachable while LEFT
held. No arming. Start60s capture;user holds LEFT/drag then physically unplugs
Glorious USB,waits5s and tests other pointer without clicks while stillheld.
Then release/clear with Esc/click other pointer ifneeded. This reference tests
same Windows/app behavior without DIY radio/firmware. Record outcome before
attributing held state to dongle. Do not assume generic Windows behavior yet.
Use -FinishBeforeUtc 2026-11-16T09:10:00Z for17:10 Manila break;controller refuses
if60s capture+60s wrap won't fit. No new armed trial after17:06;wired hasnoarm.
All capture/serial/helper/build sessions currently closed;only1s software smoke
ran. Current installed v12 remains unplugged. Save/push before break.

PHYSICAL RECEIVER REMOVAL ALSO RETAINS HELD STATE 2026-11-16:
User confirms physical unplug with LEFT held;selection kept following OTHER
pointer,cleared afterward,normal now. Job v12-physicalusb-20261116-165002-295ec075:
one actual arm/link,physical serial loss,user confirmed,no x sent. Raw2050
records/errors0,LEFTdown2/up1. Serial loss~58.387s;host LEFTTrue across portloss,
false only~81.586s after cleanup. No rearm/reopen/PnP/UAC. Firmware unchanged.
Receiver remains PHYSICALLY UNPLUGGED;all capture/serial tasks ended. Do not
claim release PASS. This is not exclusively a PnP-restart symptom.
NEXT CONTROL:run the same held-left physical USB-unplug test on Glorious in
WIRED mode,DIY receiver unplugged. Read-only raw+host button capture,no RF arm.
Compare observed host behavior before any further firmware changes. If same,
retain scoped host-lifecycle limitation and use explicit x before planned
restart;do not claim firmware can transmit a release after power is removed.
Current break17:10,armed cutoff17:06;wired reference must also finish+save before
break. No rebuild/flash/repeat of v12 PnP/removal trials now.

PHYSICAL USB-REMOVAL CAPTURE SOFTWARE READY 2026-11-16:
branch feature/physical-usb-loss-validation. V12 startup-neutral PnP held-state
fix FAILED (recorded1e1b1e5). User cleared host via Esc/releases/receiver unplug;
other pointer normal. Installed v12 remains physically unplugged,no live tasks.
No firmware changes/build/flash. New research/capture_physical_usb_loss.ps1
captures one explicit arm,raw120s/serial loss,global LEFT state1Hz. User physically
unplugs receiver while LEFT held;no UAC/PnP/automatic restart/reopen/rearm. If
cancelled/no link35s/no unplug60s,one x fallback while connected. Capturer130s
bound. Serial loss alone needs user physical confirmation,not assumed proof.
Self-checks PASS for live/cancel/timeout/no-link/disconnected action gate and
actual expired-cutoff CLI rejection before hardware access. Syntax/diff checks
PASS,firmware diff0. No physical test run. No native firmware test repeat needed.
Current user break17:10 Manila;script requires explicit future UTC arm cutoff,
check before hardware and again before p. Use -ArmBeforeUtc 2026-11-16T09:06:00Z
for this session (17:06 Manila). Stop/save/close by17:10. Refuse if cutoff passed.
NEXT physical readiness popup:reconnect receiver normally(no double-reset),
Glorious OFF/USB unplugged,selectable text/other pointer,receiver cable reachable
while holding LEFT. Actual arm cue before mouse ON;move5s,hold LEFT/drag,physically
unplug ONLY receiver around15s afterON. While still held move OTHER pointer with
no clicks,observe whether selection fixes. Then release LEFT,leave receiver
unplugged. Ifstuck Esc/release/click other pointer,confirm clear. Do not replug/
rearm until observation recorded. This separates physical removal from failed
PnP restart;not a firmware fix or generalized USB-recovery certification.

NEW TIME BOUNDARY 2026-11-16: user asks break at17:10 Manila. Stop/save/
close by17:10;do not start short physical-removal armed trial after17:06.
No5min trial planned. Check actual UTC time before arming. Hardware remains
v12 physically unplugged after failed PnP held-release retest;host cleared.
Preparing separate physical USB-removal diagnostic without firmware changes.

V12 ACTIVE PNP-RESET HELD-RELEASE FAILURE 2026-11-16:
Job v12-activeusb-20261116-162436-bb444906 finished transport capture:2353 raw
receiver records,0 read errors,one arm/targeted restart,fresh passive v12 boot
and startup-neutral-complete marker,no second arm. Host LEFT probe True before
AND after reset. User waited/checks confirmed selection still followed OTHER
pointer. Startup-zero hypothesis FAILED for this Windows PnP restart case.
User cleared via Esc/releases/receiver unplug and confirms other pointer normal.
All serial/raw/helper processes finished;receiver PHYSICALLY UNPLUGGED,installed
v12. No Windows settings/input injection changed. Do not claim overall PASS.
NEXT DIAGNOSTIC: distinguish PHYSICAL USB removal from host PnP restart,using
unchanged installed v12. No more guessed firmware release behavior or synthetic
press. Prepare bounded one-arm/raw capture for user physically unplugging only
receiver while holding Glorious LEFT;probe host LEFT before/after removal,then
observe selection before clearing. If physical removal clears it,scope remaining
problem to PnP scenario;otherwise investigate host/class behavior further. No
new rebuild/flash or automatic rearm needed. Separate physical readiness required.

V12 DEFERRED HELPER PREPARED 2026-11-16: user approved UAC with Glorious
wired/OFF. Job v12-activeusb-20261116-162436-bb444906 ready;expires
2026-11-16T08:27:41Z (16:27:41 Manila). NO restart/arm/capture yet. Separate
normal receiver reboot/OFF-unplugged/text/other-pointer popup pending. Capture
requires75s remaining plus prearm checks;refuse/cancel if late. On confirmed
readiness run controller -Mode Capture -RunName v12-activeusb-20261116-162436-bb444906.
To cancel write exact private job.cancel;helper expires without action. Never
reuse v11 jobs. Arm cue must precede mouse ON;hold through real reset before
selection check. Old v11 host-held failure now cleared,not assumed fixed by v12.

V12 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user reconnected receiver
and confirmed NICENANO/Glorious wired-OFF. Bootloader0.6.0/nRF52840-nicenano/
S1406.1.1 verified;exact v12 hash1e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30;
UF2 PASS243 blocks0x26000..0x352FF,copy complete. COM12 expected USB identity
confirmed.6.5s passive capture receiver-v12-passive-20261116-162126.txt shows
v12 banner AND HID startup neutral complete,no RF arming,zero traffic,no errors.
Board NOW v12 UNARMED,unused327680 ACK/5min/8 discovery budgets. COM12 closed,
no live serial/raw/helper/build. Flash metadata receiver-v12-flash-validation.json.
This verifies startup neutral completion,NOT Windows retained-held-button fix.
NEXT physical/UAC readiness:Glorious wired/OFF,receiver normally connected.
Controller -Mode Prepare gets UAC/deferred180s helper,then separate normal receiver
reboot/OFF-unplugged/selectable-text/other-pointer readiness. Capture v12-only,
75s helper lifetime required,one p/actual arm cue,reset at link+25s. Hold LEFT
through actual reset and wait about40s from mouse ON (pointer inactive) BEFORE
checking other-pointer selection without clicks;then release LEFT/moveGlorious
and verify no auto-resume. Capture samples host LEFT state before/after reset.
Do not reuse oldv11 job or invoke immediate restart helper. If still dragging,
Esc/release all/unplugreceiver and confirm host clear. No press-pulse workaround.

V12 STARTUP-NEUTRAL SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-startup-neutral. V12 BUILT/VALIDATED,NOT FLASHED.
V11 active USB-loss held-release failed;host cleared after Esc/released buttons/
physical receiver unplug. Read-only host left/right=false,ClickLock=false.
V12 sends exactly one six-byte all-zero neutral after first USB configuration,
main-thread wait for completion BEFORE DTR/RF arming. Pending blocks forwarding;
EAGAIN/I/O/timeout keeps output blocked. Duplicate config/service idempotent.
Reuses link-end neutral helper;no synthetic press,motion,scaling/protocol/budget
change. Passive startup now includes this zero HID report (not RF input).
Runtime startup-gate regression failed on v11;all8 native tests PASS including
zero bytes,no RF report/counter/sequence consumption,error/completion/offline
cases and prior bounds/stop/reset behavior. Receiver/probe/radio builds+UF2 PASS.
Staged source/config/HID separation/v12 banner+startup completion marker verified.
61988 flash/16128 RAM,243 UF2 blocks0x26000..0x352FF. Exact receiver image:
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA2561e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30.
Private receiver-v12-1e543890.uf2,priorv11/v10/v9/v8 preserved. Validation JSON
receiver-v12-build-validation.json. Probe/radio rebuilt;byte identity not claimed;
default HID existing image unchanged,not rebuilt. Host controller now expects
v12+neutral completion marker,records read-only global LEFT state before/after
reset;selfchecks/syntax PASS,no host input injection. No live helper/serial/raw/build.
Hardware STILL v11,receiver PHYSICALLY UNPLUGGED;no v12 flash or hardware result.
Windows may ignore initially zero button state after re-enumeration;this remains
an UNPROVEN experiment,not a guaranteed fix. Do not claim release success until
same active reset physical test. If it fails,clear host state via Esc/release/
receiver unplug and reassess;no synthesized press/release workaround.
NEXT PHYSICAL PAUSE:keep Glorious wired/OFF,reconnect only receiver and double-
reset into NICENANO. Verify bootloader identity/exact v12 hash/UF2,copy,then
passive startup showing startup neutral complete/no RF arm. Separate UAC Prepare
and normal-reboot/OFF held-test readiness for controller's v12 job. No new timing
cutoff supplied on this resumed afternoon session. Do not reuse old v11 job.

V11 ACTIVE USB-LOSS HELD-RELEASE FAILURE CONFIRMED 2026-11-16:
Job v11-activeusb-20261116-155201-22120a8b completed capture with0 read errors,
1795 receiver records. One arm/link,one receiver PnP restart(helper exit0),one
fresh passive v11 boot,no second p;post-reset ACKs0/hid_reports0 while2851 RF
packets observed. Software transport recovery succeeds,but overall hardware
held-release test FAIL:other-pointer movement extended selected text while
Glorious LEFT remained held. Other-pointer click/release did not clear;Esc,
release all buttons and physical receiver unplug cleared it. User confirms normal
pointer now. Read-only Windows left/right state false;ClickLock disabled.
Raw LEFT events:down23.717s/up23.772s/down28.174s,NO matching final up. Last
raw47.810s;PnP began07:54:25.315Z,postboot07:54:28Z. Physical no-resume assertion
confounded by held state;only log-based no rearm established. No injection/settings
changes. All capture/helper/serial processes finished. Receiver PHYSICALLY UNPLUGGED
per clearing instruction;last installed v11,postreset unarmed before poweroff.
NEXT SOFTWARE: test an isolated startup-neutral report hypothesis. Current v11
fresh boot has last_buttons0 and emits no neutral until armed/held;add exactly
one all-zero HID report after configuration,outside callback,before RF output/
arming,wait completion,gate output on success. Native zero/no-motion/error/
idempotence checks,build/validate,preservev11. This is unproven on Windows after
PnP reset (HID class may ignore unchanged zero state);no synthetic button press.
Do not claim fix until same active-reset physical test. No repeat v11 test now.

ACTIVE USB-LOSS HELD-RELEASE FAILURE OBSERVED 2026-11-16: during job
v11-activeusb-20261116-155201-22120a8b,user reports selected text followed OTHER
pointer while Glorious LEFT remained held after receiver restart. Thus Windows
held-state release FAILED,despite fresh passive boot/no second arm. Do not mark
whole trial PASS. User instructed release Glorious LEFT,click/release other
pointer LEFT on harmless text,reconnect Glorious USB or leave OFF. Clearing-state
popup pending. No new arm/serial writes. Existing capture finishing read-only;
helper restart already completed. Need inspect logs/physical no-resume separately.
Likely follow-up investigate sending neutral after USB reconfiguration before
allowing input;not implemented,not assumed proven. Keep hardware passive.

ACTIVE USB-LOSS CAPTURE IN PROGRESS 2026-11-16: job
v11-activeusb-20261116-155201-22120a8b. User confirmed normal receiver reboot,
Glorious OFF/unplugged,text/other pointer. Helper lifetime/fresh v11 boot checks
passed;ONE p sent,actual arm confirmed before mouse-ON popup. Raw150s and serial
running. Controller closes sole serial reader/signals one receiver restart at
link+25s,then opens new receiver CDC and observes passive boot/no second p.
User holding LEFT through restart,checking selection with other pointer,then
release/move Glorious to verify no auto-resume. Leave mouse ON/unplugged for RF
observation. No UAC interaction during held input. If user cancels,write exact
build-pairing-analysis/v11-activeusb-20261116-155201-22120a8b.cancel file.
No second reader/manual restart/rearm. Pending helper/capture/physical results.

ACTIVE USB-RESET HELPER PREPARED 2026-11-16: user confirmed UAC readiness
and approved helper. Job v11-activeusb-20261116-155201-22120a8b in ignored
build-pairing-analysis. Helper READY,expires2026-11-16T07:55:05Z (15:55:05 Manila).
No restart/arming/serial capture yet. Waiting for separate normal receiver reboot,
Glorious OFF/unplugged,text/other-pointer readiness popup. Capture requires75s
remaining helper life;refuse if too late (rough latest start07:53:45Z before
prearm drain). To cancel,write job.cancel;helper exits without PnP restart.
On readiness run research/capture_active_usb_reset.ps1 -Mode Capture -RunName
v11-activeusb-20261116-155201-22120a8b. Never substitute another job or reset
armed state. Fresh v11 startup required;one actual arm cue before mouse ON.

RESUMED / ACTIVE USB-RESET HOST TOOLS READY 2026-11-16:
User resumed later;old02:30 cutoff/pause no longer applies. No new cutoff given.
Branch feature/active-usb-reset-validation. Installed firmware remains v11,
last known disarmed/latched24544 ACKs;COM12 closed,no live sessions. No firmware
edit/build/flash or hardware/serial/admin action on this resumed turn.
New tracked scripts research/restart_receiver_usb_deferred.ps1 and
research/capture_active_usb_reset.ps1 reuse existing exact receiver PnP target.
Deferred helper gets UAC FIRST,waits180s for unique signal,expires/cancels without
restart,revalidates healthy exact USB root and safe ID characters before one
restart. PnP child limited15s;result private. Controller Prepare creates unique
job;Capture requires unused matching job/>75s helper life,fresh v11 startup and
explicit physical reboot/OFF readiness.150s hidden raw,one p/actual arm cue,
link+25s close sole serial reader and signal restart,reopen expected receiver CDC
and observe new passive startup without second p. Abort attempts x if armed and
recovery not verified;helper cancelled on exit. Background raw not cadence proof.
Self-checks PASS:exact/foreign/duplicate/mismatched targets,unsafe ID chars,
cancel/signal/expiry gates,used/wrong/expired/short-lived jobs,and existing-log
protection. JSON DateTime/string deadline conversion bug caught/fixed;fixture
preserves logs and touches no hardware. Syntax/diff checks PASS;firmware diff0.
Recorded private target ID fits strict format;raw executable exists. No helper
prepared or running yet. Runtime active USB loss/held-release remains UNTESTED.
NEXT PHYSICAL PAUSE:mouse wired/OFF,receiver normally connected;user ready for
UAC. Run controller -Mode Prepare,approve helper BEFORE any mouse holding.
Record RUN_NAME/helper expiry. Then promptly separate readiness popup for normal
receiver reboot(no double-reset),Glorious OFF/USB unplugged,selectable text/other
pointer. Capture -Mode Capture -RunName ... only with sufficient helper lifetime;
if expired cancel/defer,new job/UAC needed,no automatic arm retry. After real arm
cue,user briefly moves then holds LEFT through targeted restart,checks other
pointer selection fixed while still held,releases,then moves Glorious to verify
NO input resumes without explicit arm. Keep receiver connected/mouse ON afterward
for passive RF observation. Save scoped outcomes;MCU reset cause not instrumented.

PAUSED BEFORE02:30 / V11 POST-RECOVERY ARM PASS 2026-11-16:
User's02:30 Manila work window is ending;pause after completed test. No further
hardware actions until user resumes. Branch feature/receiver-usb-reset-restart.
User confirms all controls smooth and held-left release after NEW explicit arm
following receiver-only Windows USB restart. Configured-passive restart PASS;
active USB-loss/held-button behavior across reset is NOT yet tested.
Private base build-pairing-analysis/receiver-v11-rearm-20261116-022118:
100s hidden raw1975 receiver-only background records,0 read errors;left down/up
15/15,wheel45. Right click user-confirmed but no independent right transition
in this short capture. Link21.144s,x46.166s,raw up46.176s,neutral/disarmed chunk
46.199s;host timing is not physical latency. One arm/discovery,24544 ACKs,
8065 HID submissions,48 retries,busy75,no ERROR;one disarmed/reboot response.
Board v11 NOW DISARMED/LATCHED,one arm/discovery,24544 ACKs used. COM12 closed,
no live serial/raw/build/admin helper/UAC session. User told release/reconnect USB.
Installed v11 SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and private
receiver-v11-b1d27489.uf2. V10/v9/v8 rollback images preserved. Bounds remain
327680 ACK/5min,8 discoveries,1s gap,30s initial discovery,one p/boot,x stop.
NEXT WHEN USER RESUMES: prepare ACTIVE USB-reset test,including old-session
abort,no automatic resume,fresh explicit arm and held-button behavior. Review
UAC sequencing first:approval must occur before held-input observation,otherwise
consent clicks confound the test. Bounded deferred receiver-only admin helper
may be needed;NOT implemented. Existing helper restarts immediately. Exact
receiver root privately identified;never replace drivers/target other devices.
Configured-passive PnP restart returned one fresh unarmed boot/no loop;MCU reset
cause not separately instrumented. Active-reset release/startup neutral remains
unverified. Do not claim complete USB recovery or daily-use/gaming reliability.
Current used boot needs explicit normal-reboot readiness before another arm.
No repeated v11 build/flash/passive baseline without a new reason.

V11 CONFIGURED-PASSIVE WINDOWS USB RESTART PASS 2026-11-16:
User approved UAC for fixed receiver-only helper. Run private base
build-pairing-analysis/receiver-v11-usbrestart-20261116-021713. Exact receiver USB
root revalidated;pnputil restart exit0/helper exit0. No other device/driver change.
After restart,COM12 reenumerated;8s passive DTR capture has ONE fresh Zephyr boot
and v11 banner,zero traffic,no ERROR/arming/link or restart loop. COM12 closed.
Scope:configured-passive host-restart scenario only;MCU reset cause not separately
instrumented,active USB-loss/held-button behavior NOT tested. No automatic arm.
Board v11 fresh boot UNARMED,unused327680 ACK/300000ms/8 discovery budgets.
Flash metadata references restart validation;raw/result files ignored. No sessions.
NEXT physical readiness:post-recovery explicit-arm controls/x-stop check. Harmless
scrollable/selectable doc,other pointer,Glorious OFF/USB unplugged,receiver stays
connected without reset. Prepared capture-v11-rearm.ps1 syntax to check;100s raw/
95s serial,one p,actual arm cue,x ONCE25s after link. Try motion/clicks/wheel then
hold LEFT through x stop,other pointer checks selection fixed. No second p.
No short arm after02:26;pause/save by02:30. Active USB-reset scenario deferred.

V11 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user confirmed NICENANO
visible/mouse wired-OFF. Bootloader0.6.0/nRF52840-nicenano/S1406.1.1 and exact
v11 hash b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b verified;
242 UF2 blocks0x26000..0x351FF validated,copy complete,COM12 USB identity matched.
6.5s passive capture receiver-v11-passive-20261116-021434.txt:correct v11 passive
USB-restart banner,zero traffic,no ERROR/arming/link/disarm. COM12 closed.
Board NOW v11 UNARMED,unused327680 ACK/300000ms/eight-discovery budgets.
Metadata receiver-v11-flash-validation.json. No host USB-restart test yet.
NEXT physical/UAC readiness:keep Glorious wired/OFF,receiver unchanged. Windows
administrator token absent;launch fixed receiver-only restart-receiver-usb-admin.ps1
via Start-Process -Verb RunAs -WindowStyle Hidden,with validated run-name and
exact target from receiver-usb-restart-target.json. User approves Windows UAC.
Read private result,wait for expected CDC re-enumeration,open sole serial reader
DTR and capture new startup passively without p. Verify fresh v11 boot/unarmed,
no errors or reset loop. If approval/restart fails,no blind retry or driver change.
Configured-passive restart test alone doesn't prove active held-button recovery;
record scope honestly. Then separate explicit-arm readiness/control trial if time.
Pause02:30;no short armed trial after02:26,5min after02:22. No new firmware build.

V11 PASSIVE USB RESTART SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-usb-reset-restart. V11 BUILT/VALIDATED,NOT FLASHED.
User policy:stay passive after USB reconnect,explicit arm required. Configured
RESET/DISCONNECTED requests COLD software reboot via main-thread service;callback
only latches off. Initial enumeration reset doesn't restart;USB_ERROR remains
fatal. Service runs in DTR wait and fatal receive path;new boot uses existing
unarmed/one-arm rule.5min/327680 ACK/8 discoveries/1s gap/x stop unchanged.
New missing-service regression failed compilation on v10;all8 native checks PASS
including reset/disconnect/error split,quick-config output block and deferred
restart. Receiver/probe/radio builds+UF2 PASS;receiver staged sources/config/
v11 banner verified,CONFIG_REBOOT=y. Probe/radio hashes changed with shared
edits,not byte-identical controls;default HID existing image unchanged.
V11 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b.
61812 flash/16128 RAM,242 blocks0x26000..0x351FF. Private image
receiver-v11-b1d27489.uf2;v10/v9/v8 preserved. Metadata receiver-v11-build-validation.json.
Board STILL v10 expired/passive282670 ACKs/3 discoveries;COM12 closed,no sessions.
NEXT physical flash readiness:Glorious wired/OFF,receiver double-reset NICENANO,
verify identity/hash,copy then passive startup(no arm). Then receiver-only
Windows host restart test requires administrator/UAC because current tokennotadmin.
Exact USB root identified privately in receiver-usb-restart-target.json.
Ignored restart-receiver-usb-admin.ps1 prepared/syntax PASS,NOT EXECUTED;it
revalidates unique exact receiver root and runs pnputil /restart-device only on
that instance,writes private result. No driver replacement/other devices touched.
Begin with passive configured receiver;fresh v11 startup after host restart and
no arm/input required before further tests. Power unplug alone not proof. Active
USB-loss/held-button release remains separate hardware evidence if not tested.
Pause by02:30 Manila;no short armed trial after02:26 or5min after02:22.

RESUMED UNTIL02:30 AM MANILA 2026-11-16: user extends work again,revoking
break pause after v10 trial. Five-minute milestone saved/pushed8977f4b.
Current time on resume~02:01. Stable hardware remains v10 expired/passive,
282670 ACKs/three discoveries used,COM12 closed,no live serial/raw/build.
NEXT review USB reset/reconnect guard before implementation. Current bridge
latches failure on reset/disconnect/error after configuration;main exits on
offline. Recovery policy question pending:explicit rearm vs automatic remaining
session resume. Do not implement dependent behavior until preference answered.
Review local Zephyr USB reset/endpoint completion behavior independently.
Stop/save by02:30;no new5min trial after02:22,short trial after02:26.

PAUSED AFTER V10 FIVE-MINUTE HARDWARE PASS 2026-11-16:
User explicitly extended work until this trial finished,then pause;trial COMPLETE
and user confirms "All worked smoothly after both cycles;stopped cleanly".
Pause now;no further trial/build/flash until user resumes. Branch
feature/receiver-five-minute-session. Private base
build-pairing-analysis/receiver-v10-sustained-20261116-015246:full335s serial,
onlyfirst180s hidden/background raw (11004 receiver records,0 read errors).
Raw left down/up84/84,right8/8,wheel410 in initial portion;not full-session raw
or foreground cadence evidence. Serial one arm,three links/two recoveries:
link host-relative22.126s,108.760s(213365ms left),168.307s(153811ms left),
final neutral-complete stop322.095s (~300s after first link). All3 link stops
neutral complete,no ERROR;final282670/327680 ACKs(155757 idle,3 status,
126910 type1),126712 HID submissions,198 retries,busy7101. No x needed/sent.
Board v10 NOW EXPIRED/PASSIVE:one arm,three discoveries,282670 ACKs used.
COM12 closed,no live serial/raw/build. User told release buttons/reconnect USB.
V10 UF2 SHA25632713b13dff337956808746b952bad1fa27d108079fa73ba52584c703ea4c2a9,
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2;
private receiver-v10-32713b13.uf2,priorv9/v8 rollback images preserved.
Limits remain finite5min/327680 ACKs,8 discoveries,1s gap,30s initial arm,
one arm/boot,x stop latches until reboot. V9 x-stop/held release hardware PASS;
V8 held-left expiry/control/motion hardware PASS;no logic changes except v10 bounds.
NEXT AFTER RESUME: review USB reset/reconnect recovery (currently latches HID
output off and requires receiver reboot),design smallest safe change/tests using
v10 as stable baseline. No automatic rearm or unbounded operation/limit increase.
Current boot cannot be reused for p;new trial needs explicit physical readiness
and normal reboot. No repeated v10 baseline/extraction/build/flash without reason.
Five-minute success is not hours-long/daily-use/interference/physical-latency or
confirmed1000Hz gaming certification. Keep DPI/Windows/mapping unchanged.

V10 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: exact v10 hash/UF2 and
NICENANO0.6.0/nRF52840-nicenano/S1406.1.1 verified;copy complete. COM12 expected
USB identity confirmed. Passive6.5s capture receiver-v10-passive-20261116-014706.txt
shows correct bounded-five-minute/x-stop banner,zero traffic,no errors/arm/link.
Board NOW v10 UNARMED,unused327680 ACK/300000ms/eight-discovery budgets.
COM12 closed,no live sessions. Metadata receiver-v10-flash-validation.json.
Prepared capture-v10-sustained.ps1:335s serial+first180s hidden raw;single p/DTR,
2s drain,actual arm confirmation. Per-capture .stop-request file sends x ONCE
through same reader if user reports issue. Arm cutoff01:52AM Manila2026-11-16;
pause/save by02:00. No full-session raw/foreground cadence claim.
NEXT physical trial readiness: harmless scrollable document/other pointer,
Glorious OFF/USB unplugged,same settings,receiver connected unchanged. Trial
movement/clicks/wheel,mouse-only OFF3s/ON around1min and3min,then observe original
5-minute stop. Stop early with x on trouble;never reopen serial/automatically rearm.

V10 FIVE-MINUTE SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-five-minute-session. V10 BUILT/VALIDATED,NOT FLASHED.
Only receiver cap/session constants327680 ACKs/300000ms and banner changed;
8 discoveries,1s gap,30s initial arm,one arm/boot,x latch/neutral,quiet active,
mapping/ACK timing retained from hardware-passed v9. Bounds regression failed
on v9;all8 native checks PASS including original shared deadline/stop errors.
Receiver build/UF2/source/config/banner checks PASS:61560 flash/16128 RAM,
241 blocks0x26000..0x350FF. Existing probe/radio/default HID hashes verified
unchanged vs v9,not rebuilt because non-HID paths not modified.
V10 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA25632713b13dff337956808746b952bad1fa27d108079fa73ba52584c703ea4c2a9.
Private receiver-v10-32713b13.uf2,receiver-v9-ecc725eb.uf2 and v8 preserved;
metadata receiver-v10-build-validation.json. Hardware benefit not tested.
Board STILL v9 DISARMED/latch,25000 ACKs used;COM12 closed,no live sessions.
NEXT physical flash readiness: mouse wired/OFF,receiver double-reset NICENANO;
verify identity/hash/copy then passive startup,no arming. Separate5-minute
trial readiness if time fits. Full335s serial,onlyfirst180s optional hidden raw
(control events,not cadence). Exercise controls and mouse OFF3s/ON around1 and
3minutes,observe original5-minute stop. x early stop available through same reader.
HARD PAUSE02:00 Manila;do not arm5-minute trial after01:52. Recheck clock before
arming;defer if too late. Update handoff/push all tested milestones before pause.

V9 EXPLICIT X STOP HARDWARE PASS 2026-11-16: user confirms selection fixed
while LEFT physically held after x stop;circles smooth like v8/wired.
Private base build-pairing-analysis/receiver-v9-xstop-20261116-013634.
100s hidden raw2393 receiver records,0 read errors. Link observed22.335s;
host x sent47.365s,raw left-up47.372s,serial neutral/disarmed chunk47.397s.
Host/USB/serial chunk times are not physical stop latency. One arm/discovery,
25000 ACKs(11151 idle,0 status,13849 type1),13840 HID submissions,9 retries,
busy4011,no ERROR;one disarmed/reboot-required response. No second p sent.
Board v9 DISARMED/LATCHED,one arm/discovery,25000 ACKs used. COM12 closed,
no live raw/serial/build. User instructed reconnect mouse USB. V9 preserved.
NEXT SOFTWARE: v10 bounded5-minute experiment,327680 ACK cap/300000ms,keep
8 discoveries/1s gap/30s initial arm/one arm per boot/x stop/neutral/quiet logic.
Only constants/banner and dependent tests change. All normal checks/builds.
No automatic flash/arm. Physical readiness later explains5-minute bounds.
Hard stop02:00 Manila;no new5-minute arm after01:52 (full335s capture+wrap).
Host raw logger stays1..180s;first180s optional raw is partial,full335s serial
covers5-minute session. Do not claim full-session raw coverage or gaming latency.

V9 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user confirmed NICENANO
visible/Glorious wired-OFF. Bootloader0.6.0,nRF52840-nicenano,S1406.1.1 verified;
exact v9 hash ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e,
UF2 partition validation241 blocks0x26000..0x350FF PASS;copy completed.
COM12 expected USB identity verified.6.5s passive capture privately saved as
receiver-v9-passive-20261116-013345.txt:correct v9 explicit-x/quiet banner,
zero traffic,no errors/arm/link/disarm. COM12 closed,no live serial/raw/build.
Board NOW v9 UNARMED with unused65536 ACK/60s/eight-discovery budgets.
V8 rollback preserved. Metadata receiver-v9-flash-validation.json:armed=false.
NEXT PHYSICAL PAUSE: Glorious OFF/USB unplugged,selectable harmless text and
other pointer,same DPI/settings;leave receiver connected without reset.
Prepared ignored capture-v9-xstop.ps1 syntax PASS,not executed. Single p/DTR,
2s drain,95s serial+100s hidden raw;enforces01:56AM arm cutoff. Actual arm
confirmation BEFORE mouse ON. User circles5-10s,then hold LEFT dragging text.
Script sends x ONCE25s after observed link start,logs command host UTC/elapsed,
expects disarmed/neutral/reboot-required response. While LEFT physically held,
other pointer without clicks should leave selection fixed,then release.
No second p or automatic retry. Background raw for release events only,no
cadence claim. Verify motion stays v8-like by user report. End/save by02:00.

RESUMED UNTIL02:00 AM MANILA 2026-11-16: user authorizes work until02:00.
Current time on resume01:29. Prior break pause revoked for this interval.
Stop/save/close by02:00;no new armed60s trial after01:56,and start any longer
trial only if its full capture+wrap-up fits before02:00. Always recheck UTC time
before arming. V9 exact image hash reverified ecc725eb...;NOT flashed. NICENANO
not currently visible;physical readiness popup is next. Board last known v8
expired/passive59958 ACKs,COM12 closed,no live sessions. No build repeated.

EXPLICIT USER PAUSE 2026-11-16: user selected "Pause here — resume flashing
later" after v9 software milestone2f86867. Stop project work until user resumes.
V9 built/tested/pushed,NOT FLASHED;board remains v8 expired/passive59958 ACKs.
COM12 closed,no live serial/raw/build. Next after resume is physical NICENANO
flash readiness,not automatic copy/arming. See full v9 checkpoint below.

V9 EXPLICIT-STOP SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-explicit-stop. User resumed and requested break after
next step;software preparation complete,now at physical flash/break boundary.
V9 BUILT/VALIDATED,NOT FLASHED. Adds receiver-only lowercase x cancellation:
10ms paced CDC checks during scan/active loops,latches off ACKs/discovery/p
arming,disables RF before neutral,prints disarmed success only after release.
Reboot required to rearm. Poll interval is not a physical stop-latency guarantee.
Limits remain65536 ACKs/60s/8 discoveries/1s gap;no sustained limit increase yet.
Native active-stop regression failed on v8;all8 tests now PASS (prearm/active/
rediscovery cancellation,disable/neutral errors,no ACK after latch,no p rearm,
repeated handling and paced polling). Receiver/probe/radio builds/UF2 checks PASS.
Staged sources/config/HID separation/v9 banner+disarmed response verified.
Probe/radio controls not byte-identical after shared-source edits;native probe
behavior checked,new stop code receiver-only;default HID existing image unchanged.
V9 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e.
61536 flash/16128 RAM;241 blocks,0x26000..0x350FF. Private rollback/candidate
receiver-v8-5df75330.uf2 and receiver-v9-ecc725eb.uf2;metadata
build-pairing-analysis/receiver-v9-build-validation.json. No v9 hardware claim.
Board STILL validated v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used.
COM12 closed,no live serial/raw/build. No new flash/arming on this resumed turn.
NEXT PHYSICAL PAUSE when user continues: Glorious wired/OFF,double-reset only
RECEIVER into NICENANO;verify bootloader identity/exact v9 image before copy,
then passive startup without arming. Separate explicit motion/held-left x-stop
readiness and one arm;capture x command timestamp,neutral/reboot-required response
and Windows release while physically held. Also check motion remains v8-like.
Do not extend limits until explicit-stop hardware pass;preserve v8 as baseline.
Do not repeat completed extraction/v8 tests/builds without a new reason.

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
