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
