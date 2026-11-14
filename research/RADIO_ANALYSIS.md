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

## V5 armed capture: multi-ACK acceptance and compound packet

After user battery readiness, one arm/35-second capture saved in ignored
`build-pairing-analysis/probe-v5-armed-20261114-233953.txt` and summary JSON.
1003 CRC-valid packets:958 discovery and45 data packets on51. First15 data packets
were length11,sequence0..14 once each at139159..139383ms,16ms apart. Fifteen ACKs
sent(seq0->1 through14->15),no errors. This supports repeated ACK acceptance.
Do not infer report throughput from this polling/USB-text experiment.

Next30 packets were length20,sequence15,unchanged payload:
`0f 00 41 00 00 00 00 ff ff 00 00 45 06 01 00 00 00 00 00 00`,
at139397..139623ms. Two sequence bytes plus tag41/eight-byte type1 and
tag45/eight-byte type5 match the tag construction at0x15BA0 and message appending
at0x15E5C..0x15E92. Type5 meaning remains unknown. V5 rejected length20 by design,
so its unused16th ACK and return to discovery are explained by the packet filter.
This is not evidence that the15th ACK failed: sequence15 was observed.

Timing(handler/READY/END relative to RX END,us): discovery42/424/472; data ACK
ranges24..70/155..201/199..245. All16 transmissions completed,MODECNF0=0x200.
Board V5 discovery consumed,15 ACKs attempted,ACK window expired/passive. COM11
closed. User told to reconnect cable; reconnection unconfirmed.

Next: V6 narrowly accepts the observed length20/tag41+45 shape in addition to
length11/tag41, retaining16-attempt/two-second bounds and V4 timing/response bytes.
No general packet parser, type5 command handling, HID output or extra TX budget.

## V6 preparation

The matcher now accepts length 11/tag 0x41 or length 20/tag 0x41+0x45 at the
observed offsets. Address, channel, CRC and all arming/budget/deadline guards are
unchanged. The tag 0x45 block is not interpreted. Native tests use the captured
compound shape and reject every unsupported length 0..64, wrong tags and wrong
channel/base/logical address. All seven tests pass.

V6 probe build and UF2 validation pass: flash 58,740 / RAM 15,936 bytes;
230 blocks, payload 0x26000..0x345FF. Current probe UF2 SHA256:
`dbf0b69cd0e4d61656d3bb6ef61b4782783d97c80b3293222d881a2de8c66020`.
V5 preserved locally at ignored `build-pairing-analysis/probe-v5-a1b78c57.uf2`.
Receive-only regression build and compile flag/binary string
separation checks pass. Radio UF2 hash remains 8a19a4c2... above.
V6 flashed after user confirmed NICENANO ready/mouse wired, rechecking bootloader
0.6.0, nRF52840-nicenano, S140 6.1.1 and dbf0b69c... UF2 hash/bounds.
Five-second DTR-enabled COM11 baseline passed: V6 banner, 1 MHz timing capture
initialization, 19 channels and six zero-counter SCAN lines; no errors, no arming.
Evidence: ignored `build-pairing-analysis/probe-v6-passive-20261114-234710.txt`.
Board V6 passive/unarmed, all TX budgets unused. COM11 closed; mouse last wired.
Next: battery readiness, one armed 35-second capture; check compound ACK and
sequence advance past 15. Do not repeat flash/baseline.

## V6 capture: compound ACK acceptance

After battery readiness, captured once for 35 seconds in ignored
`build-pairing-analysis/probe-v6-armed-20261114-234923.txt` and summary JSON.
1004 valid RX: 958 discovery, 46 data on channel 51. Sequences 0..15 appeared
once each, then sequence 16 repeated 30 times. Exactly 16 ACKs and one discovery
TX completed with no errors. The sole length-20 packet was sequence 15:
`0f00410000ffffffff0000450601000000000000`; all other data were length 11.
Sequence 16 confirms acceptance of the compound packet ACK. The subsequent
repetition is expected after exhausting the ACK budget. Data span 153381..153860 ms.
Final repeated payload `1000410100bdfb4e130000` has changed type-1 bytes, but no
controlled button/axis decoding is claimed.

Timing (handler/READY/END after RX END, us): discovery 18/399/447; data ACK ranges
13..72/144..203/189..248. All 17 completed, MODECNF0=0x200. Board V6 consumed/passive;
COM11 closed, user told to reconnect cable, reconnection unconfirmed.
Next diagnostic: V7 raises ACK cap to 256, retaining the two-second deadline and
observed packet filters. No continuous/unlimited TX, HID, or timing changes.

## V7 preparation and candidate movement layout

V7 raises only the ACK cap to 256 within the unchanged two-second deadline;
banner prints that constant. Native tests check the finite cap, expiry after
partial use, wrap, timeout exhaustion and existing exact packet filters. All
seven tests and probe build/UF2 validation pass: flash 58,732 / RAM 15,936 bytes,
230 blocks, payload 0x26000..0x345FF. Current probe UF2 SHA256:
`e9e79cce535839d72fc4ca338ee1fc85a62f8b9ee68194af6139deaad69c8cee`.
V6 artifact preserved locally at ignored `build-pairing-analysis/probe-v6-dbf0b69c.uf2`.
V7 not flashed/armed; receive-only regression and binary separation checks pass.
Radio: flash 56,704 / RAM 15,872 bytes, 222 blocks, payload 0x26000..0x33DFF;
new UF2 SHA256 `3b459262846fbae3f30ded047c5a768bc9e44ad7bf2490fdbddfee85c6f584d1`.
V7 flashed after user confirmed NICENANO ready/mouse wired, checking bootloader
0.6.0, nRF52840-nicenano, S140 6.1.1 and e9e79cce... UF2 hash/bounds.
Five-second DTR-enabled COM11 passive baseline passed: V7 banner with256-ACK/
two-second limit, 1 MHz timing initialization,19 channels,six zero-counter SCAN
lines,no errors. Evidence: ignored
`build-pairing-analysis/probe-v7-passive-20261114-235645.txt`.
No `p` sent. Board V7 passive/unarmed, all budgets unused. COM11 closed; mouse
last wired. Next: battery readiness, then one35-second armed capture. Check the
ACK window separately from post-window RX traffic. Do not repeat flash/baseline.

The hash-checked snapshot contains a 91-byte HID mouse report descriptor at
0x1A750..0x1A7AA. At 0x1A7AC the length/type/pointer words are
`91, 0x22, 0x1A750`. This supports identifying it as descriptor data, rather than
interpreting another incidental byte-pattern match as a descriptor. The report
builder at 0x14D20..0x14D66 packs the same eight bytes for the wired USB send at
0x14D7A..0x14D82 and type1 RF queue at 0x14DBE..0x14DC8.

| Type1 report byte(s) | Descriptor field | Static encoding |
| --- | --- | --- |
| 0 | Buttons1..5, then padding | Five low bits, three padding bits |
| 1 | Generic Desktop Wheel | Signed8, relative, -127..127 |
| 2..3 | Generic Desktop X | Signed16 little-endian, relative |
| 4..5 | Generic Desktop Y | Signed16 little-endian, relative |
| 6 | Consumer usage0x238 | Signed8, relative, -127..127 |
| 7 | Consumer usage0 | Eight bits, logical0..1, relative; semantic role unresolved |

This table concerns the eight bytes after tag0x41, not the sequence/tag prefix.
There is no Report ID item in this descriptor. This is static evidence; verify
each axis/button/wheel with controlled actions before enabling Windows input.
No type5 meaning is inferred and no decoding/HID code was added.

## V7 capture: bounded two-second link

After user battery readiness, one arm/35-second capture saved as ignored
`build-pairing-analysis/probe-v7-armed-20261114-235814.txt` and summary JSON.
1059 CRC-valid packets:913 discovery,146 data (145 length11,one length20).
124 ACKs were sent(seq0..123),then sequence124 was received. No errors and all
125 transmissions completed. One compound report was successfully ACKed.
ACKed RX timestamps112128..114097 span1969ms, consistent with the two-second
deadline ending below the256 cap. The exact discovery/ACK-window start is not
timestamped; these RX times alone are not an exact deadline measurement.

Channel51 had130 data packets:seq0..123 once each,then seq124 six times.
Another16 copies of seq124 appeared at114135..114326 on logged frequencies
27(4),76(6),4(2),49(2),78(2). The first three match group0's retry channels;
49/78 may be adjacent-channel reception. This supports retry/channel changes
after ACKs stop, not a complete receiver hopping/recovery implementation.
Stock0x1889A..0x188DC increments retry/channel indices after unsuccessful
cycles and writes the table-selected frequency. No hopping code was added.

Hardware timing(handler/READY/END relative to RX END,us): discovery40/421/469;
data ACK ranges12..73/143..204/187..248. MODECNF0=0x200 throughout. ACKed RX gaps
were15..17ms; USB text/polling miss traffic, so this is not a throughput or1000Hz test.

Applying the descriptor layout offline to124 unique ACKed reports gives masks0/1,
wheel0,X range-43..412,Y-142..196,and both consumer fields0. Movement/click actions
were uncontrolled; these ranges support further testing but do not validate axes.
Board V7 discovery consumed,124 ACK attempts used,window expired/passive until
reboot. COM11 closed; user told to reconnect cable, reconnection unconfirmed.
Next: normal board reboot (no new firmware), passive startup, then a distinct
controlled battery capture holding left button and moving only right/lifting
to return. Verify candidate button/X mapping before USB pointer integration.

Controlled-test preparation on 2026-11-15: user confirmed normal V7 reboot/mouse
wired. Five-second DTR-enabled COM11 startup passed with expected V7 banner,
256-ACK/two-second limit,1 MHz timing initialization,19 channels,six zero-counter
SCAN lines,no errors and no arming. Evidence: ignored
`build-pairing-analysis/probe-v7-input-passive-20261115-000319.txt`.
Board V7 unarmed with fresh budgets; COM11 closed. Await battery readiness while
holding left button and moving only right/lifting to return, with no scrolling
or other buttons. This will be a distinct controlled capture, not another generic
link test. No source or firmware change.

## Controlled left-button/rightward test (2026-11-15)

User confirmed holding left button and moving only right, lifting to return,
with no wheel/other buttons. One arm/35-second capture saved as ignored
`build-pairing-analysis/probe-v7-left-right-20261115-000519.txt` and summary JSON.
1070 valid RX:924 discovery,146 data (145 length11,one length20).124 ACKs with
unique sequences0..123 over1968ms,no errors. Only unique ACKed reports are used
for candidate movement interpretation, avoiding duplicate motion from retries.

Button byte0 was0 for sequences0..15,then1 for all108 reports at16..123.
Wheel and consumer fields were0 throughout. All X values from16 onward were
positive, supporting left-click bit0 and the positive X direction. However:

- First16 reports were small negative/zero motion with button0; previously
  queued data is a plausible explanation, not established fact.
- Sequence16 contained X22768,Y7359. It is retained as an outlier/transition
  observation, not silently removed from the evidence.
- Sequences17..123 had X25..58,Y9..37; Y was substantial despite the requested
  right-only motion. Hand orientation/lift effects are possible, so this does
  not fully isolate axes or validate the Y sign.

These observations partly validate the descriptor interpretation; they do not
establish button release, other buttons, Y direction, wheel or latency. No USB
pointer output or firmware change. Board V7 window expired/passive,COM11 closed.
User told to stop/release/reconnect cable; reconnection unconfirmed. Next: normal
board reboot,passive startup,then separate battery readiness for right-button
hold and downward-only motion/lift to return. Preserve initial packets/outliers.

## Controlled right-button/downward test (2026-11-15)

After confirmed normal reboot, passive baseline saved as ignored
`build-pairing-analysis/probe-v7-right-down-passive-20261115-000859.txt` passed
with no arming/errors. User then confirmed right-button hold and downward-only
movement toward them/lifting to return, no wheel/other buttons.
Armed35-second capture: ignored
`build-pairing-analysis/probe-v7-right-down-20261115-001018.txt` and summary JSON.
1071 complete RX packets:925 discovery,146 data (145 length11,one length20).
One final discovery line was cut off at capture end; all124 ACKed reports and
125 TX timing lines were complete. No firmware errors; ACKed span1969ms.

Button byte0 was1 for sequences0..15,then2 for all108 reports at16..123.
The earlier left-button state strengthens the previously queued-report hypothesis.
Wheel/consumer fields0 throughout. Sequence16 X6927,Y-18490 is retained as a
large transition observation; accumulated motion/wrap is possible, not established.
The remaining107 reports(seq17..123) had X -9..21(abs total1042),Y -7..86
(abs total4813),with96 positive and11 negative Y samples. This supports right
button bit1 and downward-positive Y; nonzero X and small negative Y remain recorded.
No automatic prefix/outlier dropping was added to receiver code.

Board V7 window expired/passive; COM11 closed. User told to stop/release/reconnect
cable; reconnection unconfirmed. Next: normal reboot/passive baseline, then
stationary wheel-up-only battery test with no buttons, about one notch/second,
to compare wheel field/sign and released-button state. No new firmware.

## Controlled wheel-up test and idle packets (2026-11-15)

Fresh passive baseline: ignored
`build-pairing-analysis/probe-v7-wheel-up-passive-20261115-001422.txt` passed.
After user confirmed stationary/no buttons/slow wheel-up, one35-second capture:
ignored `build-pairing-analysis/probe-v7-wheel-up-20261115-001538.txt` and summary.
1001 complete valid RX:954 discovery,16 length11,one length20,30 length2.
17 ACKs(seq0..16),256ms ACKed RX span,no errors. First16 reports retained prior
button2/downward motion. Seq16 contained button0,wheel+19,X855,Y121,consumer
fields0. This supports released buttons and positive wheel-up byte1; only one
fresh wheel report was seen and accumulated motion remains, so retain the caveats.

Then30 identical length2 packets `11 00` (sequence17,peer0) arrived. V7 deliberately
rejected them. Stock0x18938..0x18950 constructs length2 with sequence bytes; the
queue-empty branch skips report append to0x18976. Stock0x16094..0x1609A branches
past outgoing-sequence increment when queue is empty. Thus repeated idle sequence
can be legitimate and must not be interpreted as fresh motion or ACK failure alone.
No idle ACK has been tested yet. V8 will accept only this additional observed
length2 form, using the existing sequence+1/cleared-flags ACK,256 cap/two-second
deadline and exact CRC/address/channel guards. No idle HID report is generated.
Rename matcher to is_ack_candidate; add native small-buffer/repeat/expiry tests
and incoming-length ACK logging. Board V7 window expired/passive;COM11 closed.
User told to stop/reconnect cable; reconnection unconfirmed.

## V8 preparation: bounded idle ACKs

Matcher renamed is_ack_candidate and now accepts length2 (no tag reads), or the
existing length11/tag41 and length20/tag41+45 shapes. Length checks precede tag
access. CRC/address/channel filters,ACK bytes/timing,256 cap/two-second deadline,
one arm and one discovery per boot are unchanged. No HID.
ACK log format is now `PROBE ACK sent: count=N/256 len=L seq=S next=T`.
Idle length2 never carries a type1 report; a subsequent real report can use the
same sequence, so future report deduplication must not consume sequences on idle.

Seven tests pass, including a true three-byte radio buffer(length+two payload),
wrong identity/channel,truncated/extended idle,repeated idle ACKs,expiry and
budget/timeout guards. Native assertions now exit noninteractively on Windows;
the new idle fixture failed cleanly before implementing the matcher change.
V8 build/UF2 checks pass:flash58,752/RAM15,936 bytes,230 blocks,
payload0x26000..0x345FF. Current probe UF2 SHA256:
`4ad89938b68c166ebb3b73fea46878b763cd97974c3f9424acc227f0e45824f4`.
V7 preserved locally at ignored `build-pairing-analysis/probe-v7-e9e79cce.uf2`.
Receive-only regression and binary separation pass. Radio UF2 hash remains
3b459262... above;flash56,704/RAM15,872,222 blocks.
V8 flashed after user confirmed NICENANO ready/mouse wired,checking bootloader
0.6.0,nRF52840-nicenano,S1406.1.1 and4ad89938... UF2 hash/bounds. Five-second
DTR-enabled COM11 passive baseline passed: V8 banner,256 cap/two-second window,
1 MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/probe-v8-passive-20261115-002350.txt`.
No `p` sent;board V8 passive/unarmed,all budgets unused. COM11 closed;mouse last
wired. Next: controlled wheel/idle capture after battery readiness; do not repeat
flash/baseline or earlier generic tests.

## V8 capture: idle ACK acceptance and fresh wheel reports

After readiness, one35-second controlled stationary/no-buttons/slow wheel-up
capture: ignored `build-pairing-analysis/probe-v8-wheel-up-20261115-002506.txt`
and summary JSON.1069 complete valid RX:928 discovery,18 length11,one length20,
122 length2.118 ACKs(19 reports,99 idle) over1973ms;no errors,all119 TXs complete.
Idle ACK sequence counts17:27,18:69,19:3. Data sequence17 at101965ms and18 at
103155ms each had button0,wheel+1,X0,Y0,consumer fields0. Each followed idle with
the same sequence, confirming report deduplication must ignore idle sequences.
Sequence16 had wheel+19,X1665,Y-528; retained as startup/accumulation observation.
The initial16 reports contained prior wheel-up data.

This supports idle ACK acceptance, resuming report traffic after idle, wheel-up
positive byte1, and released-button state. It does not establish a throughput,
latency, full-recovery or1000Hz result. Board V8 window expired/passive;COM11
closed. User told to stop/reconnect cable; reconnection unconfirmed.
Next: separate bounded composite CDC+HID prototype using the same V8 RF behavior,
real-report deduplication, USB backpressure and button release on stop/error.

## Bounded wireless HID bridge prepared (2026-11-15)

Separate `-App receiver` uses the V8 protocol/timing/256-ACK/two-second window,
with composite CDC+HID (local2FE3:0008,product DIY Mouse RX HID Probe). Only
validated type1 packets forward five buttons,signed16 LE X/Y,and signed8 wheel.
The independent six-byte descriptor omits unresolved consumer fields. No clipping
or arbitrary startup-report dropping. Real-report deduplication ignores idle;
255->0 remains a new sequence. USB enqueue precedes ACK; endpoint busy withholds
ACK without consuming RF budget or report sequence. Fatal errors stop the loop.
End/error attempts neutral button release with bounded waits. USB reset/disconnect
after configuration requires reboot for this prototype; recovery remains future work.

Eight tests pass: UF2/evidence tests plus native actual protocol in both build
modes and actual USB bridge with asynchronous mocks. Cases include duplicate/
idle->data handling,busy/retry/errors/short write,wrap and neutral release/timeout.
SDK's DMA allocation rounds six bytes to eight, so wire size is explicitly six;
native tests now model this padding. The first staged build caught an obsolete
allocation-size assertion; the corrected receiver build passed.

Receiver UF2: `C:\Users\Gabriel\zephyrproject\build-mouse-receiver\zephyr\zephyr.uf2`.
SHA256 `cfc2eb5820c34969f7299ee43b47a28b20bd1421dd5ac70acce816f566441b4b`.
Flash60,288/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF.
Probe/radio/original HID regressions and config/binary separation all pass.
New probe hash `fac516813e95b0b1e049079093b2d3acbed278d59cf701d6d4ef597f6e3816ef`;
new radio hash `6f0f263f5095a064aa8c782d42bb06973eef9c569eaf6c9ab745f0e3689eab8e`.
Original HID hash unchanged `058c1ff58130ab07c14f258cde93df331e5345f99b4b673db0ede46d2f4a5953`.
Preserved hardware-tested V8: ignored `build-pairing-analysis/probe-v8-4ad89938.uf2`.
HID forwarding/descriptor present only in receiver binary; RF arming only in
receiver/probe. No live build/serial session. Board still non-HID V8 expired/passive.

Receiver NOT flashed/tested on hardware. USB enqueue can alter ACK turnaround;
measure it in the first real input test. First test should use an empty workspace,
another pointer,mouse initially OFF/unplugged to reduce stale queued actions,then
power-on only after arming confirmation and movement without clicks/scrolling.
Verify passive enumeration/no input before arm and neutral stop after the window.
No measured latency,throughput,channel recovery or1000Hz claim.

## Receiver v1 flash and passive enumeration (2026-11-15)

User confirmed NICENANO ready/mouse wired. Verified bootloader0.6.0,
nRF52840-nicenano,S1406.1.1,receiver hash cfc2eb58... and application bounds;
copied receiver UF2. Windows PnP reports OK for composite USB,CDC COM12/MI_00,
HID-compliant mouse/MI_02 and HIDClass input on local2FE3:0008.
Five-second DTR-enabled COM12 baseline passed: receiver v1 banner,256-ACK/two-second
limit,1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors.
Evidence: ignored `build-pairing-analysis/receiver-v1-passive-20261115-005017.txt`.
No `p` sent; board receiver v1 passive/unarmed,all budgets unused. COM12 closed.
No pointer output has been observed/tested yet. User asked for separate readiness:
empty desktop,another pointer,mouse OFF/unplugged and left OFF until arming
confirmation. First test is motion only; inspect real cursor response and serial
timing/USB errors before deliberate click/wheel trials. No reflash/baseline repeat.

## Receiver v1 armed test: standalone status blocks progress

User prepared empty desktop,mouse OFF/unplugged,and confirmed readiness. COM12
armed once,then user was told to switch ON and move without clicks/scrolling.
Ignored `build-pairing-analysis/receiver-v1-armed-20261115-005234.txt` and summary:
285 valid RX:254 discovery,one length2 idle `00 00`,30 length11 status packets
`00 00 45 06 01 00 00 00 00 00 00`. One idle ACK(seq0->1); both TXs completed,
no errors/USB busy. No type1 movement reports arrived. User confirmed no wireless
pointer movement. Thus the USB forwarding path remains unverified; the immediate
block is V1's report filter rejecting standalone tag45, previously seen only in
the accepted compound tag41+45 form. Queue-empty idle did not consume sequence0.
Discovery timing58/439/487us(handler/READY/END),idle ACK32/163/207us.

Next: receiver v2/non-HID probe v9 accept length11/tag45, ACK but never forward
to HID or consume movement deduplication. Keep256/two-second guards and response
bytes/timing unchanged. Add native observed-status fixture and USB no-output test,
preserve v1 UF2,rebuild/test before asking for next verified flash. Board v1 window
expired/passive,COM12 closed; user told to stop/reconnect, reconnection unconfirmed.

Receiver v2/source fix now passes all eight tests. The observed standalone status
fixture is accepted by the real matcher/protocol and produces no USB output or
movement deduplication update in the real bridge,including while endpoint busy.
Receiver v1 UF2 preserved as ignored `build-pairing-analysis/receiver-v1-cfc2eb58.uf2`.
Receiver v2 build/UF2:flash60,304/RAM16,064 bytes,236 blocks,0x26000..0x34BFF,
SHA256 `e80eb30c51fa375b39a2af5a6d15de65341765673db0682fc9fe60a95591b6c9`.
Non-HID probe v9:flash58,796/RAM15,936 bytes,230 blocks,0x26000..0x345FF,
SHA256 `a1bc4de092a01bdce4c98eb907dfef85ac57675f0d84a33f17a18c1848c22c39`.
Config/binary separation checks pass;radio/default HID artifacts remain unchanged
from6f0f263f.../058c1ff5... above. No v2 hardware test yet. Board remains receiver
v1 expired/passive. Next: verified v2 flash/passive baseline after NICENANO readiness,
then separate off/unplugged/empty-desktop readiness and one motion-only test.

Receiver v2 subsequently flashed after user confirmed NICENANO ready/mouse wired.
Reverified bootloader0.6.0,nRF52840-nicenano,S1406.1.1 and e80eb30c... UF2 bounds/hash.
Five-second DTR-enabled COM12 baseline passed: v2 banner,256/two-second limits,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors/arming.
Windows composite/CDC/HID statuses all OK. Evidence: ignored
`build-pairing-analysis/receiver-v2-passive-20261115-010250.txt`.
Board v2 fresh/unarmed,all budgets unused;COM12 closed. User asked for separate
off/unplugged/empty-desktop readiness; v2 armed pointer test pending.

## Receiver v2: first end-to-end wireless pointer confirmation

After off/unplugged/empty-desktop readiness,armed once on COM12,confirmed arming,
then asked user to power ON/move without clicks/scrolling. Ignored capture:
`build-pairing-analysis/receiver-v2-armed-20261115-010552.txt` and summary JSON.
415 complete valid RX:271 discovery,34 idle,110 length11.122 ACKs over1971ms:
34 idle,one standalone status tag45,87 unique type1 reports. All123 TXs completed,
no errors/USB busy. Movement reports had buttons0,wheel0,X -117..149,Y -64..96.
ACK timing ranges(handler/READY/END after RX END,us):13..112/144..243/189..288,
now including USB enqueue cost before ACK.

User explicitly confirmed Windows pointer moved in response to the unplugged
Glorious mouse and stopped after the brief window,with no other pointer used.
This establishes bounded end-to-end RF-to-USB pointer control. Click/wheel USB
behavior,continuous recovery,latency/throughput and gaming performance remain
unverified. Board v2 window expired/passive,122 attempts used,COM12 closed.
User told to stop/reconnect cable;reconnection unconfirmed.
Next: longer bounded receiver v3 input test with1024 ACK cap/10-second window;
non-HID probe keeps256/two-second limits. No changes to RF bytes/timing or USB
mapping. Native deadline/limit checks and build validation precede another flash.
Capture45seconds to cover30-second discovery arming plus10-second active window.

Receiver v3 source and final build now pass all eight tests. Shared fixed constants
drive ACK deadline,listen duration and printed limits:receiver1024/10000ms,
non-HID probe256/2000ms. RF bytes/timing,USB descriptor and movement mapping remain
unchanged. A new native regression covers a256-status-only transport wrap:when
status advances beyond the last report,invalidate that old retry marker without
generating input;idle still leaves it unchanged. This avoids suppressing a new
report after a full status-only sequence cycle. Not a hardware wrap measurement.

Final receiver v3 UF2 SHA256:
`4bc43f3543638f337eb7d50d42621cc20be321e79ebdefed2ba8d94d9b73d4da`.
Flash60,332/RAM16,064 bytes,236 blocks,payload0x26000..0x34BFF. Do not use the
intermediate a020e8ab... build that preceded the wrap-gap fix.
Updated non-HID probe v9:flash58,808/RAM15,936,230 blocks,0x26000..0x345FF,
SHA256 `8fc95a4738701e29ed84f72e3430ccce89a450c917c7498f2879338402ff4de5`.
Radio regression remains6f0f263f... and original HID remains058c1ff5... above.
Config/binary separation and staged receiver source checks pass. Working v2
preserved locally at ignored `build-pairing-analysis/receiver-v2-e80eb30c.uf2`.
Board still v2 expired/passive;COM12 closed,no live build/serial session. V3 not
flashed. Next: verified v3 flash/passive baseline after NICENANO readiness,then
separate readiness on a harmless scrollable document/page and one45-second
capture for a ten-second deliberate movement/click/wheel test. No gaming claim.

## Receiver v3 flash/passive check and handoff (2026-11-15)

User confirmed NICENANO ready/mouse wired. Rechecked final4bc43f35... UF2 bounds/
hash,bootloader0.6.0,nRF52840-nicenano,S1406.1.1 and copied application UF2.
Five-second DTR-enabled COM12 startup passed:v3 banner,1024 ACKs/10000ms,
1MHz timing initialization,19channels,six zero-counter SCAN lines,no errors/arming.
Windows composite/CDC/HID statuses all OK. Evidence: ignored
`build-pairing-analysis/receiver-v3-passive-20261115-012451.txt`.
Board v3 passive/unarmed,all budgets unused;COM12 closed,no live build/serial
session. Mouse last wired. User requested next-chat handoff before arming.
Next: separate harmless-document/off-unplugged readiness,then one45-second
capture for the10-second controls trial. Do not rebuild/reflash/repeat baseline.

## Receiver v3 armed controls attempt: no RF received (2026-11-15)

User confirmed readiness with harmless scrollable content,another pointer,and
mouse OFF/unplugged. Sandbox COM12 open failed before sending any command;
authorized elevated device access succeeded. DTR enabled,one `p` sent,and exact
1024/10000ms arming text confirmed before the user power-on/controls cue.
45-second capture completed and COM12 closed. Evidence: ignored
`build-pairing-analysis/receiver-v3-armed-20261115-013158.txt` and `.summary.json`.
One arming,81 SCAN lines,zero valid RX/CRC failures/invalid lengths,zero discovery
responses/ACKs,zero errors/USB busy. There was no RF-to-HID traffic to validate
movement,clicks,wheel or neutral release. User observations/power-on timing pending;
no cause for absent traffic established. Arming window expired; budgets unused,
but one-arm opportunity consumed. No automatic rearm/reboot. A repeat requires
physical normal receiver reboot and fresh readiness,not rebuild/flash/baseline.
User subsequently confirmed powering on late or missing the cue. Controls were
not tested during this attempt; pause for normal receiver reboot/fresh readiness.

Second v3 attempt after fresh readiness: ignored
`build-pairing-analysis/receiver-v3-armed-20261115-013553.txt` and `.summary.json`.
One p/confirmed arming,then requested pop-up power-on cue;45seconds completed,
COM12 closed. Again zero RX/discovery replies/ACKs. User timing/observations
pending; no controls validation or firmware failure conclusion. Pause for user;
do not automatically retry. Use pop-ups for required physical intervention.
User confirmed prompt power-on with USB unplugged and no Windows response.
Next targeted diagnostic: mouse already ON near unchanged/expired receiver before
a15-second COM12/DTR capture,WITHOUT p,to separate RF silence from cue timing.
Pause for physical readiness; no rebuild/flash or automatic armed retry.
