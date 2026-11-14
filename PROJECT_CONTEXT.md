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
  Receiver checkpoint is ready for commit/push; use git status/log on resume.
- Added separate `firmware/build.ps1 -App receiver`, preserving hid/radio/probe.
- Receiver product DIY Mouse RX HID Probe, local USB ID2FE3:0008, CDC+HID.
  COM port may differ from COM11. HID endpoint interval1ms is not measured1000Hz.
- Same V8 discovery/ACK bytes/timing, one arm/discovery per boot,256 ACK cap,
  two-second post-discovery window. No output before explicit serial `p`.
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
- Physical board STILL runs non-HID V8. Discovery consumed,118/256 ACK attempts
  used,window expired/passive until reboot. COM11 closed; no live serial session.
- User told to stop scrolling/reconnect mouse cable after V8 capture; actual cable
  reconnection unconfirmed. Do not assume battery/wired state.
- V8 UF2 preserved: ignored build-pairing-analysis/probe-v8-4ad89938.uf2.
  SHA256 4ad89938b68c166ebb3b73fea46878b763cd97974c3f9424acc227f0e45824f4.
- Earlier V4/V5/V6/V7 artifacts are also preserved in ignored evidence.
- Known bootloader:0.6.0,nRF52840-nicenano,S1406.1.1. Last bootloader drive D:;
  verify identity anew before flashing, do not erase/update bootloader.
- Receiver artifact (NOT FLASHED):
  C:\Users\Gabriel\zephyrproject\build-mouse-receiver\zephyr\zephyr.uf2
  SHA256 cfc2eb5820c34969f7299ee43b47a28b20bd1421dd5ac70acce816f566441b4b.
  Build/UF2 pass:flash60,288/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF.
- SDK DMA macro rounds six-byte allocation up to eight; code explicitly copies/
  transmits six bytes. First staged build stopped at obsolete allocation assertion;
  corrected build above passed. Eight tests pass, including the native bridge
  with padded-buffer/asynchronous USB stubs and protocol in probe/receiver modes.
  These do not prove real Windows enumeration or new USB-before-ACK timing.
- All four builds, UF2 checks and config/binary separation checks pass. No live
  build/serial session remains. HID descriptor/forwarding strings exist only in
  receiver; RF arming exists only in receiver/probe. Regression artifacts:
  probe:flash58,776/RAM15,936,230 blocks,0x26000..0x345FF,
  SHA256 fac516813e95b0b1e049079093b2d3acbed278d59cf701d6d4ef597f6e3816ef.
  radio:flash56,760/RAM15,872,222 blocks,0x26000..0x33DFF,
  SHA256 6f0f263f5095a064aa8c782d42bb06973eef9c569eaf6c9ab745f0e3689eab8e.
  default HID:129 blocks,0x26000..0x2E0FF,unchanged SHA256
  058c1ff58130ab07c14f258cde93df331e5345f99b4b673db0ede46d2f4a5953.
  These supersede earlier hashes at the same build paths; V8 preserved separately.

Exact next steps:
1. Save/push tested checkpoint on feature/wireless-hid-prototype. No receiver
   hardware test has occurred; the board still runs the non-HID V8 probe.
2. Await user keeping mouse wired and double-resetting nice!nano into NICENANO. Verify
   bootloader identity and receiver hash/bounds, flash only receiver UF2.
3. Discover new2FE3:0008 COM port, open DTR and collect passive baseline without `p`.
   Check receiver banner,1MHz timing startup,no errors and no spontaneous pointer input.
4. Separate explicit input readiness is required. First test on an empty workspace,
   with another pointer available. Switch mouse OFF, unplug its cable and leave OFF
   until arming is confirmed, to reduce stale queued actions. Then power ON and move
   without clicking/scrolling for the first two-second HID test. Verify cursor motion,
   ACK progression,timing,USB errors and neutral stop before testing deliberate clicks.
5. Save raw log/summary locally; update context/research and push tested milestone.
   Do not claim reliable gaming receiver completion from a bounded HID demonstration.

The firmware snapshot is an application-region read, NOT a full-chip backup.
Do not flash proprietary receiver firmware onto this board or write mouse config.
Do not invent speculative RF decoding beyond established evidence.
