# First radio capture experiment

Date: 2026-11-14 (Asia/Manila).

## Input and method

Input is the user's successfully read mouse application, not a substitute firmware:
`build-pairing-analysis/mouse-app-6000-1cc9f.bin` (ignored).
SHA256: `965c8f8cc17e912d0ede5f0ed82b4c12a15d21fae931aeaf59921c94794e1738`.
GNU ARM objdump from Zephyr SDK 0.17.2 disassembled it as Thumb code at base 0x6000.
Literal pools and tables were read as data, not interpreted as instructions.

Register meanings were cross-checked against Nordic's nRF52840 product specification
and the installed Nordic MDK headers/SVD:
https://docs.nordicsemi.com/r/bundle/ps_nrf52840/page/radio.html

## Static evidence

The routine at `0x15A1C` initializes RADIO. A call at `0x19B84` occurs on the path
next to the `USB power removed` string. This supports testing it for battery-mode
radio traffic; it does not prove every later state keeps the same settings.

| Register | Observed value | Code/evidence |
| --- | --- | --- |
| MODE | 4 (BLE 2 Mbit/s modulation) | 0x15A50..0x15A52 |
| PCNF0 | 0x00000008 | 0x15AA2..0x15AA6 |
| PCNF1 | 0x03040040 | store 0x15AAC, literal 0x15B68 |
| PREFIX0 | 0xB3B2B1B0 | store 0x15A78, literal 0x15B5C |
| PREFIX1 | 0xB7B6B5B4 | store 0x15A7E, literal 0x15B60 |
| BASE0 | 0x01234567 | store 0x15A86, literal 0x15B64 |
| BASE1 | FICR DEVICEID[0] initially | 0x15A88..0x15A8E |
| TXADDRESS | 1 | 0x15A94 |
| RXADDRESSES | 2 | 0x15A9A |
| CRCCNF | 2 | 0x15AB2 |
| CRCINIT | 0xFFFF for two-byte CRC | 0x15AD0..0x15AD4 |
| CRCPOLY | 0x00011021 | 0x15AD6..0x15AE2, literal 0x15B6C |

PCNF1 specifies maximum payload 64, four base-address bytes plus a prefix,
MSB-first packet fields, and whitening enabled. PCNF0 specifies an 8-bit length
field and no S0/S1 field. The observed PCNF0 leaves PLEN at zero even though MODE
is 4; the first logger deliberately reproduces the observed raw value rather
than replacing it with a conventional BLE packet format. BLE modulation here
does not establish a standard Bluetooth connection or packet protocol.

The routine at `0x983C` copies 16 channel bytes from `0x1A966` into a RAM table.
The path at `0x15F4C..0x15F5A` uses the RAM table for RADIO.FREQUENCY:

```text
51 27 76  4
48 24 73  8
52 26 77  2
47 22 72  7
```

These are offsets from 2400 MHz. Group selection and hop timing are not yet
implemented or fully recovered. Scanning all entries is a reception experiment.
`0x134B2..0x134B6` also writes `0x89ABCDEF` to BASE1 on another state path.

## Logger choices and hypotheses

Source: `firmware/radio_logger/`. It is separate from the working HID prototype.

- Receive-only: starts TASKS_RXEN; no TXEN, ACK, pairing, or HID movement code.
- Uses MODE/PCNF/CRC values above and scans each listed channel for 40 ms.
  Logger v2 prepends discovery channels 3, 49, 78 to the 16-channel table.
- DATAWHITEIV is set to the Nordic reset value 0x40; no alternative seed has
  been established for this path. This assumption needs over-air validation.
- Logical address 0 listens on the observed common BASE0/prefix B0.
- Logical address 1 alternates between the queried four-byte mouse address
  interpreted little-endian (prefix B1) and observed BASE1 0x89ABCDEF (prefix B1).
  The first capture validated reception with the mouse-address mapping; no packets
  were recorded with the alternate pairing base.
- CRC-valid packets are emitted as raw hex; failed CRC and oversize reports are counted.
- It waits for a USB serial connection with DTR enabled before scanning.
- Board LED is initially on, then toggles after each complete scan.
- Polling and text logging intentionally miss traffic. This is not a packet-loss,
  latency, throughput, or 1000 Hz measurement tool.
- No CRC-valid packets does not prove the mouse is silent or incompatible.
  It may need a different address/seed/state or an acknowledgment before sending movement.

`generate_radio_header.py` verifies the exact firmware hash and obtains literal
parameters and channel bytes from the image. It validates the recorded USB identity
and address before emitting a private header only into the staged build source.
No device-specific address or vendor binary is added to tracked firmware source.

## Hardware procedure

Build verified on 2026-11-14: five Python tests passed; 56,688 bytes flash,
15,872 bytes RAM. UF2 contains 222 blocks (113,664 bytes) and writes only
`0x26000..0x33DFF`. SHA256:
`9daaa154dd51db6a6d77d2784ba38c400b6f79a2e3196a07f471cf2bede565fd`.
The radio and USB use the same Zephyr HFXO on/off manager.

1. Build with `firmware/build.ps1 -App radio`; use only its validated UF2.
2. Double-reset the nice!nano into NICENANO and copy the new radio logger UF2.
3. Keep the Glorious mouse wired initially so it remains usable for setup.
4. Have Codex identify/open the logger's serial port with DTR enabled and save its output.
5. During the capture, unplug the Glorious mouse's USB cable, switch it on,
   place it near the board, and move/click it for the requested interval.
6. Reconnect the Glorious mouse afterward. Save capture output locally under an ignored build folder.

The logger product is `DIY Mouse RX Logger`, local test USB ID `2FE3:0001`.
It appears as a serial device, not a HID mouse.

Hardware baseline on 2026-11-14: flashed the hash-verified UF2 after verifying
NICENANO bootloader identity. Windows enumerated COM11 (2FE3:0001, MI_00).
Opening with DTR enabled produced the expected banner and radio register settings,
followed by 12 SCAN lines over eight seconds, all counters zero and no errors.
The mouse was still wired during this baseline.
Evidence: ignored `build-pairing-analysis/radio-baseline-20261114-121111.txt`.
The earlier HID UF2 remains available in `C:\Users\Gabriel\zephyrproject\build-mouse-hid\zephyr\zephyr.uf2`.

## First battery-powered capture

User confirmed mouse unplugged and switched on, and was asked to move/click it.
Saved 30-second capture: ignored
`build-pairing-analysis/radio-wireless-20261114-121312.txt` and `.summary.json`.

- 595 CRC-valid packets, all logical address 1 with the mouse BASE1 candidate.
- All payloads: byte `0x20` followed by the four address bytes read over USB.
  Exact device-specific values remain in ignored evidence.
- Logger frequencies/counts: 4/149, 48/151, 77/147, 2/147, 47/1.
- Packet timestamps span 243430..273198 ms since board startup.
- Counters are cumulative, including scanning while the serial port was closed:
  first SCAN good=804/bad_crc=1/invalid_len=0, last SCAN 1384/1/2.
  These totals are not the capture packet count; final RX lines follow the last SCAN.
- No movement-varying payload or receiver response was captured. No transmission
  was made by the board. This validates reception, not pairing or pointer control.

## Discovery state traced after capture

The DISABLED handler calls `0x15EA8` from `0x6722`. Its state byte is at
`0x20003620`; the table at `0x15EE0` dispatches state 4 to `0x15F66`.
That path selects a frequency from the three-byte table at `0x1A950`:
**3, 49, 78** (2403, 2449, 2478 MHz). It constructs length=5, payload byte
0x20, then copies FICR DEVICEID[0] into the remaining four bytes at
`0x15F70..0x15F84`. This matches the captured payload structure.

The original scan omitted these exact discovery frequencies. Adjacent-channel
reception is a plausible explanation for the frequencies logged above; a new
capture on the exact frequencies is needed before assigning transmit channels.

State 5 (`0x15FA4`) switches PACKETPTR to the receive buffer and starts RX,
with timeout derived from state offset 32 times four. State 6 (`0x15FAE`)
checks CRCSTATUS; when receive length is 3 it reads a channel-group byte and
a little-endian 16-bit value into state offsets 23 and 44. It then regenerates
the channel table and moves toward normal operation. This is static evidence
for a discovery response, not a validated response recipe: timing, accepted
values, and subsequent acknowledgments still require investigation.

Logger v2 only adds the three discovery frequencies. Six Python tests and the
build/UF2 bounds check pass. Flash 56,704 bytes; RAM 15,872 bytes; 222 UF2 blocks,
payload range 0x26000..0x33DFF. SHA256:
`d5122f0ad3b6061c6db82be77b0c661d04c1c333b964595d3e3af83c34417882`.
Built at the same radio UF2 path above. Flashed after verifying the bootloader
identity and UF2 hash. COM11 returned; five-second baseline confirmed the v2
banner, 19 frequencies, and six SCAN lines with all counters zero and no errors.
Evidence: `build-pairing-analysis/radio-v2-baseline-20261114-121813.txt` (ignored).
Battery-powered v2 capture completed:
`build-pairing-analysis/radio-v2-wireless-20261114-122244.txt` and `.summary.json`.
855 valid packets over timestamps 294555..324531 ms, all mouse-base/address 1,
all the same 0x20 plus four address bytes. Channel counts: 3=123, 49=123,
78=119, 4=119, 48=120, 77=125, 2=126. Reception on the three exact discovery
frequencies is confirmed; adjacent-channel reception also occurs. First/last
SCAN totals were 1091/6/1 and 1923/8/1 (good/bad_crc/invalid_len), cumulative
since boot and not per-capture totals. User was told to reconnect the mouse cable.

## One-shot discovery probe v1 (hardware tested)

`firmware/build.ps1 -App probe` builds a separate artifact in `build-mouse-probe`.
The `radio` build remains receive-only; the probe's TX code is compile-time gated.
The probe remains passive until serial ASCII `p` arms a 30-second window, with
one transmission maximum per boot and no rearming. It only responds to a
CRC-valid, exact-length 0x20/address payload, matched on logical address 1,
mouse BASE1, and one of the three exact discovery frequencies.

The test response is length 3, payload `00 F4 01`: channel group 0 and timing
value 500, chosen from the stock channel-group indexing and initial 500 value.
This is an inferred candidate, NOT a captured original-receiver response.
It transmits at 0 dBm after an experimental 250 us software delay, then listens
on group zero's first frequency (51) for two seconds, before resuming passive scans.
There are no subsequent ACKs or HID reports, so a transient transition is the
expected observable result, not a working receiver. Absence of a transition
would not disprove the response layout: turnaround/scheduling may be wrong.

No persistent mouse configuration writes are implemented. A mouse power cycle
is the recovery step if the experiment leaves it in a temporary connection state.
Transmit completion is bounded; a timeout consumes the one-shot budget too.
The native C harness tests the actual packet matcher, explicit arming, inability
to extend/rearm, transmit timeout, receive-buffer restoration, and no retry.
Seven tests pass. Hardware assertions also exercise the packet matcher at startup.

Final probe build passes (2026-11-14): 57,564 bytes flash, 15,936 bytes RAM,
225 UF2 blocks, application payload 0x26000..0x340FF. Path:
`C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`.
SHA256: `6e66aba83badd6014ca26d1dd8a10e374f3df4904983b7e681fe79a96f47d69b`.
The first intermediate probe build is superseded.
Receive-only regression build also passes with DISCOVERY_PROBE=OFF, while the
probe cache is ON. Binary banner/arming-string checks confirm the builds are
distinct. Rebuilt receive-only UF2 SHA256:
`b12e5867f7b4f36a68827e5905a5b0cbce4f824ea468abd43b61045266197559`.
Probe subsequently flashed after verifying bootloader identity and its final hash.
COM11 returned. Passive startup check passed with the expected probe banner,
19 frequencies, and six zero-counter SCAN lines over five seconds; no errors or
assertions. Evidence: ignored
`build-pairing-analysis/probe-passive-baseline-20261114-181929.txt`.
After battery-mode readiness, sent serial `p` once and captured 35 seconds in
`build-pairing-analysis/probe-armed-20261114-182039.txt` and `.summary.json`.
Exactly one ARMED and one discovery-SENT line, no ERROR lines. Of 990 captured
CRC-valid packets, 31 were new eleven-byte packets on channel 51:
`00 00 41 00 00 01 00 00 00 00 00` (timestamps 93111..93352 ms).
The remaining 959 were discovery packets. The first subsequent discovery was
seen at 94074 ms. This supports acceptance of the discovery response and a
temporary transition to data traffic; it does not establish a sustained link.
The probe has consumed its transmission budget and is passive until reboot.
User was told to reconnect the mouse cable.

## Probe v2: one bounded data acknowledgment

Stock `0x18938..0x18974` prefixes normal outgoing data with two sequence bytes.
The tag builder at `0x15BA0` encodes type in bits 0..2, size in bits 3..6,
and continuation in bit 7. Tag 0x41 is type 1, size 8, no continuation.
The type-1 eight-byte report is constructed at `0x14D20..0x14D66` and passed
to the radio queue at `0x14DBE..0x14DC8`; the wired path sends the same eight
bytes at `0x14D7A..0x14D82`. The observed report was repeated unchanged;
movement/click interpretation is not yet empirically validated.

Stock `0x16088..0x160B8` releases the queued outgoing report when the received
first payload byte equals outgoing sequence+1 (eight-bit wrap). A receive
length below 3 skips the incoming application-message branch at `0x16044`.
The next probe therefore sends one length-2 ACK with payload
`[outgoing_sequence + 1, peer_sequence & 0x3F]`. High bits are cleared because
`0x160CC..0x160D2` interprets them as scheduling flags. This ACK is inferred
from the mouse code, not yet tested over air.

Probe v2 keeps the proven discovery response, then permits exactly one data ACK
within two seconds, only for a CRC-valid eleven-byte/type-1 report on channel 51
and the mouse address. At most two transmissions per boot total, with no automatic
rearm or retries. Both use the existing bounded TX path and 250 us delay.
It logs the acknowledged sequence and keeps listening for a changed sequence or
report. No Windows HID output or continuous ACK loop is implemented.
The C harness now also checks report filtering, sequence wrap, cleared scheduling
flags, ACK expiry, pre-discovery rejection, timeout consumption, and no repeat ACK.

V2 build and seven tests pass. Flash 57,960 bytes, RAM 15,936 bytes; 227 blocks,
payload range 0x26000..0x342FF. Current probe UF2 (same path above) SHA256:
`be6109e58a548ed6104f257d4bf35d950ab11ad45db1079b40287935a644c3f1`.
V2 subsequently flashed after verifying bootloader identity and the be6109...
hash. COM11 returned. Five-second passive startup confirmed v2 banner, 19
channels and six zero-counter SCAN lines without errors. No arming command sent.
Evidence: `build-pairing-analysis/probe-v2-passive-20261114-182615.txt` (ignored).
V2 armed/capture completed in ignored
`build-pairing-analysis/probe-v2-armed-20261114-182719.txt` and `.summary.json`.
One ARMED, one discovery SENT, one ACK SENT (seq=0,next=1), no errors.
974 CRC-valid packets: 31 repeated `00 00 41 00 00 01 00 FF FF 00 00` on
channel 51 and 943 discovery packets. No sequence advance or changed data
within this capture, so ACK acceptance was NOT demonstrated. The type-1 payload
differs from v1 at the second 16-bit field (FFFF vs 0000), consistent with a
movement report but not yet a controlled axis/button validation.
User was told to reconnect mouse cable. Board v2 budgets are consumed/passive.

## Probe v3: shorter data-ACK turnaround

V2 applied the successful discovery turnaround delay (250 us) to the data ACK
as well. Normal data uses a shorter timer-driven exchange than discovery; the
extra delay plus TX ramp-up may miss its RX window. Additionally, logging the
discovery transition after enabling RX could leave the first received packet
waiting while USB output completes, making a subsequent ACK stale.

V3 keeps discovery delay 250 us but removes the added data-ACK delay. It logs
the transition before starting channel-51 RX. The same two-transmission limit,
exact packet filters, expiry, and no-retry guards remain. The native test checks
the separate discovery and ACK delay arguments. This is a timing hypothesis to
test; the negative v2 result does not prove the ACK payload is correct or wrong.

V3 build and seven tests pass: flash 57,936 bytes, RAM 15,936 bytes, 227 UF2
blocks, payload 0x26000..0x342FF. Current probe artifact SHA256:
`1df362581ae875af62762513bb6c0022058de179b263c0844586bd496c99966f`.
V3 flashed after verifying bootloader identity and the 1df362... hash. COM11
returned. Five-second passive baseline passed: expected v3 banner, 19 channels,
six zero-counter SCAN lines, no errors. No arming command sent.
Evidence: `build-pairing-analysis/probe-v3-passive-20261114-183203.txt` (ignored).
V3 test completed: ignored `build-pairing-analysis/probe-v3-armed-20261114-183305.txt`
and summary JSON. One discovery SENT, one ACK SENT seq0->1, no errors. 975 valid
packets: 31 identical type-1 reports (`00 00 41 00 00 01 00 FF FF 00 00`),
944 discovery packets. No sequence advance observed. Thus removing the delay
did not demonstrate ACK acceptance. User was told to reconnect the mouse cable.

## V4 hardware timing instrumentation / handoff

V4 retains V3 response bytes, delays and two-transmission limit. New
`probe_timing.h` configures unclaimed TIMER2 at 1 MHz and dynamically allocates
two PPI channels through nrfx (without taking channels from the USB driver).
RADIO.END captures CC0; RADIO.READY captures CC1. Before TX the RX END timestamp
is saved and software age sampled; after TX a TIMING line reports handler age,
TX READY and TX END offsets relative to RX END, plus MODECNF0. Hardware startup
checks timer progress; setup failure stops the app without arming/transmission.
TIMER2 must remain disabled in devicetree and unused by nrfx's timer driver.

All seven tests and the V4 build/UF2 checks pass. Timing hardware is not yet tested;
the native protocol harness stubs the timing helpers. Flash58,680 / RAM15,936
bytes; 230 blocks; payload0x26000..0x345FF. Current probe UF2 SHA256:
`2bef74865e0f59502cdb1d6c7117708ddd5efd6c14ca693c8d3e0dfbed8309e3`.
Path remains `C:\Users\Gabriel\zephyrproject\build-mouse-probe\zephyr\zephyr.uf2`.
V4 flashed after verifying NICENANO bootloader0.6.0, S1406.1.1 and the hash above.
Five-second DTR-enabled COM11 passive baseline passed: V4 banner, `TIMING hardware
capture initialized at 1 MHz`, 19 channels, six zero-counter SCAN lines, no errors.
Evidence: ignored `build-pairing-analysis/probe-v4-passive-20261114-232306.txt`.
No `p` sent; both TX budgets unused. Serial port closed. Mouse last confirmed wired.
This baseline state was superseded by the capture below.

## V4 armed capture: first observed sequence advance

User confirmed battery readiness; one `p` sent and35seconds captured. Evidence:
ignored `build-pairing-analysis/probe-v4-armed-20261114-232436.txt` and summary JSON.
Exactly one arm, discovery TX, and data ACK TX; no errors.991 CRC-valid packets:
960 discovery and31 eleven-byte reports on channel51. Initial report at t119735:
`00 00 41 00 00 01 00 00 00 00 00`. Subsequent30 reports at t119749..119974:
`01 00 41 00 00 00 00 FF FF 00 00`. Sequence0->1 and changed payload support
acceptance of the one-shot ACK. This is the first positive ACK observation;
V2/V3 negative observations remain valid for their captures.

| Response | RX END to handler (us) | RX END to TX READY (us) | RX END to TX END (us) |
| --- | --- | --- | --- |
| Discovery | 19 | 401 | 449 |
| Data ACK | 63 | 194 | 239 |

Both completed=1; MODECNF0=0x200. Hardware timing capture works. These measurements
describe this exchange, not a proven timing window. V4 timing instrumentation
changes execution compared with V3; the reason for different outcomes is not isolated.
No second data ACK was permitted, so repeated sequence1 and eventual discovery
do not establish sustained connectivity or measured mouse report throughput.
Movement/button interpretation remains unverified.

Board V4 budgets consumed/passive until reboot; COM11 closed. User told to
reconnect cable, physical reconnection not confirmed. Next: smallest bounded
multi-ACK diagnostic with explicit arm, finite time/packet budget, preserved
timing/response bytes and native guard tests. Check stock channel/scheduling
behavior before assuming a fixed channel. No HID output yet.
Do not repeat earlier reads/captures on resume.

## V5: bounded multi-ACK diagnostic

V5 retains V4 radio initialization, hardware timing capture, discovery bytes,
250us discovery delay and immediate data ACK bytes. It allows at most16 data ACK
attempts in the existing two-second post-discovery window, in addition to one
discovery TX. One30-second arm per boot; no deadline extension or automatic rearm.
Every attempt consumes budget; any TX failure exhausts the remainder and stops
the app. CRC-valid exact eleven-byte/type1 reports must still match this mouse's
base, logical address1 and channel51. Repeated packets may be acknowledged again
within the budget because an over-air ACK can be lost. No persistent writes or HID.

Static check: `0x983C` copies the fixed16-channel table, ignoring its passed
DEVICEID argument. `0x15FDA..0x15FE8` selects group*4 on discovery transition;
`0x16088..0x160B8` applies sequence+1 release and `0x160CC..0x160D2` copies
scheduling flags from the second ACK byte. V4 observed both sequence0 and1 on51.
These observations support a bounded same-channel follow-up, not a complete hop
schedule. V5 deliberately retains channel51 and cleared scheduling flags.
Polling/USB logging still misses traffic; do not interpret this as a throughput test.

All seven tests pass with native checks for finite count, fixed deadline,
sequence wrap/flag masking, repeated-packet ACKs, timeout exhaustion and no rearm.
V5 build/UF2 checks pass: flash58,716/RAM15,936 bytes,230blocks,
payload0x26000..0x345FF. Current `build-mouse-probe/zephyr/zephyr.uf2` SHA256:
`a1b78c57b5010fb7d7de5e7eb997e1f96e012de0040ee4455b4e1abae49031fe`.
Preserved V4 artifact: ignored `build-pairing-analysis/probe-v4-2bef7486.uf2`.
Receive-only regression build passes: flash56,704/RAM15,872 bytes,222blocks,
payload0x26000..0x33DFF; new radio UF2 SHA256:
`8a19a4c22319ddb93fd8f2e49eabb6dd6548f973a8eed519cda28a0b188f3339`.
CMake flags and binary banner/arming/ACK string checks confirm compile separation.
V5 flashed after user confirmed NICENANO ready/mouse wired, verifying bootloader
0.6.0,board-ID nRF52840-nicenano,S1406.1.1 and a1b78c57... UF2 hash.
Five-second DTR-enabled COM11 passive baseline passed: expected V5 banner,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/probe-v5-passive-20261114-233754.txt`.
No `p` sent; discovery and16 ACK budgets unused. COM11 closed, mouse last wired.
Next: await battery readiness, then one35-second armed capture. Multi-ACK behavior
is not yet hardware tested. Do not repeat flash/baseline on resume.
