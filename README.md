# MOUSE DIY DONGLE

Experimental mouse receiver firmware for a nice!nano-compatible ProMicro nRF52840.
Bounded wireless Windows pointer control is user-confirmed. Continuous operation
and gaming performance remain under development; the default build is still the
original USB HID demo.

## Default USB HID demo behavior

- Enumerates as `DIY Mouse HID Prototype` (Windows may display `HID-compliant mouse`).
- Turns the board LED on, waits for USB configuration, then waits five seconds.
- Sends one relative X movement of +40 counts and one of -40 counts, 250 ms apart.
- Sends no clicks or wheel movement. Pointer distance depends on Windows settings.
- Turns the LED off after completion and sends no further movement until reboot.
- On initialization or transfer failure, stops sending; the LED stays on if LED setup succeeded.
- Uses a 1 ms HID endpoint interval; this is not a measured 1000 Hz receiver.

## Build (Windows PowerShell)

Tested toolchain: Zephyr v4.2.0, Zephyr SDK 0.17.2, Python 3.12.
Keep source in this repository and build output outside OneDrive. The build script
copies the three application files to `C:\Users\Gabriel\zephyrproject\mouse-hid-source`
because Zephyr 4.2 cannot build directly from this repository's path with spaces.
Edit the originals here, not that temporary copy.

```powershell
$project = 'C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE'
& "$project\firmware\build.ps1"
```

The script runs the validator tests, builds the firmware, and validates its UF2.
If PowerShell blocks scripts, paste the contents of `firmware/build.ps1` into the
terminal after replacing `$PSScriptRoot` with the full path to this project's
`firmware` folder. No machine-wide execution policy change is required.
To repeat just the checks in a terminal with the Python environment enabled:

```powershell
python -m unittest discover -s "$project\firmware" -p 'test_*.py'
python "$project\firmware\check_uf2.py" "$env:USERPROFILE\zephyrproject\build-mouse-hid\zephyr\zephyr.uf2"
```

The validator requires the nRF52840 family ID and application-only writes within
`0x26000..0xEBFFF`, preserving the configured SoftDevice, storage, and bootloader
regions. It checks file structure and address bounds, not physical clone compatibility.
Use the `/uf2` board target, not the board's unqualified target.

## Flash and verify

1. Double-reset RST to GND while USB is connected. Confirm `NICENANO` appears.
2. Check `INFO_UF2.TXT` against the recorded bootloader: 0.6.0, nice!nano,
   S140 6.1.1. Do not use these instructions for a different bootloader layout.
3. Copy the validated `C:\Users\Gabriel\zephyrproject\build-mouse-hid\zephyr\zephyr.uf2`
   to the root of `NICENANO`. This replaces Blinky, not the bootloader.
4. The drive disappears when the application starts. Allow Windows time to enumerate.
5. Keep your other mouse still and the pointer away from screen edges. After five
   seconds, expect a short movement right and left, then the board LED goes off.
6. In Device Manager, check **Mice and other pointing devices** and
   **Human Interface Devices**. Under Properties > Details > Hardware Ids, the
   test device uses `VID_2FE3&PID_0007`. No custom HID driver is required.
7. Unplug/reconnect: the test should run once again. Double-reset: `NICENANO`
   should return. Record these outcomes before marking hardware validation complete.

The Zephyr sample USB IDs are for this local prototype only, not product distribution.
No proprietary firmware or Glorious device IDs are used.

## Troubleshooting

- No drive during normal operation is expected; double-reset to flash again.
- LED stays on: enumeration or a transfer did not finish. Check Device Manager,
  reconnect directly to another USB port, and confirm the correct UF2 was copied.
- LED goes off but movement was missed: reconnect and watch after five seconds.
- Explorer copy error: check application behavior before assuming failure. The
  bootloader can disconnect during transfer completion. Do not format its virtual drive.
- No useful LED behavior: return to bootloader and restore the previously tested
  Blinky file at `C:\Users\Gabriel\zephyrproject\zephyr\build\zephyr\zephyr.uf2`.
- Do not erase the chip or update the bootloader as part of this test.

## Status

Blinky execution, restart, and bootloader re-entry were confirmed by the user.
On 2026-11-14, the user confirmed HID right/left movement followed by LED off,
and NICENANO bootloader re-entry. Device Manager shows `VID_2FE3&PID_0007`.
The USB HID hardware milestone is complete. Glorious RF protocol investigation
is in progress. Bounded discovery/ACK sessions and wireless pointer control are
verified. Windows left/right clicks,wheel up/down and normal button release are
user-confirmed. Forced left-button release while held at expiry is also validated.
Sustained link recovery and measured1000Hz performance remain open.

## Receive-only radio experiment

The Glorious mouse's application firmware was read over USB without erasing or
programming it. It returned to normal mode, and the user confirmed it still works.
Recovered static radio settings support a first packet-capture experiment;
see [research/RADIO_ANALYSIS.md](research/RADIO_ANALYSIS.md) for evidence and assumptions.

Build the separate logger:

```powershell
& "C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE\firmware\build.ps1" -App radio
```

The build requires the local firmware snapshot and address observation in ignored
`build-pairing-analysis/`. It checks the snapshot's SHA256, generates the private
radio header in the staged source, runs tests, and validates the new UF2's address bounds.
The original HID build remains separate and is still the default.

Radio UF2: `C:\Users\Gabriel\zephyrproject\build-mouse-radio\zephyr\zephyr.uf2`.
After a successful build, double-reset the board into NICENANO and copy this file.
Windows should show `DIY Mouse RX Logger` / a USB serial port (`2FE3:0001`).
It waits with the LED on until a serial reader opens the port with DTR enabled;
then it scans and toggles the LED after each pass. Codex can identify the COM port
and collect the output. No additional terminal software is required.

`SCAN` lines report cumulative CRC-valid, CRC-failed, and invalid-length counts.
`RX` lines contain raw CRC-valid payloads. Test the Glorious mouse on battery power
near the board while capturing; reconnect its cable afterward.
The logger never transmits, pairs, or sends pointer movement. It is not a
1000 Hz performance test, and zero packets does not prove incompatibility.
Logger USB startup and scanning passed an eight-second hardware baseline on
COM11 with the mouse still wired; no packets or errors were reported.
The first battery-powered capture saved 595 valid packets, all containing the
same 0x20 byte and mouse address. Pointer control is not implemented.
Logger v2 adds three discovery frequencies found in the firmware; its build,
six tests, and flashed serial startup check pass. Its 30-second wireless capture
saved 855 valid discovery packets, including all three exact discovery frequencies.
The logger build and application-only UF2 validation passed on 2026-11-14.

An experimental **transmitting** probe is built separately with
`firmware/build.ps1 -App probe`. It stays passive until serial `p`, then may send
one discovery response within 30 seconds, only to this mouse's matching request.
The first probe succeeded: the mouse switched to channel 51 and sent 31 repeated
eleven-byte reports before returning to discovery. Probe v2 adds one experimental
data ACK within two seconds, for at most two transmissions per boot. It does not
implement continuous acknowledgments or Windows pointer control.
The v2 data ACK did not produce an observed sequence advance. Probe v3 removes
the extra data-ACK delay and avoids USB logging while waiting for the first report;
v3 also produced no observed sequence advance. Probe v4 now adds hardware radio
event timing measurements; it builds and passes seven tests. V4 is flashed and
its passive startup confirms 1 MHz timing initialization with no errors. Its armed
capture showed sequence0->1 after the single ACK, supporting ACK acceptance.
Sustained connectivity and movement decoding remain unverified.
Probe V5 is built for the next experiment: one discovery response plus at most16
data ACK attempts in two seconds, with the same explicit arming and packet filters.
V5's hardware capture showed sequence0..15 with15 ACKs sent. It then rejected an
observed20-byte compound report under its11-byte filter. A narrow V6 filter update
passed its hardware capture: the compound sequence-15 packet was acknowledged
and sequence advanced to 16, exhausting the configured ACK budget. A longer bounded
V7 diagnostic is built with a 256-ACK cap and the same two-second deadline; it is
flashed and its capture advanced sequence0..124 with124 ACKs across approximately
two seconds, without errors. A controlled left-button/rightward capture supports
button bit0 and positive X, with startup/cross-axis caveats documented in research.
The right-button/downward test also supports button bit1 and positive Y downward.
Wheel-up produced a positive byte1 value with buttons released, followed by
two-byte idle packets that V7 did not ACK. V8 idle handling is built, passes seven
tests and receive-only regression checks, and passed its flashed passive startup.
V8's capture ACKed99 idle packets and resumed fresh wheel reports (wheel+1,
buttons0,X/Y0), supporting idle handling and wheel-up mapping. A separate bounded
wireless HID prototype is the next implementation step.
Other buttons, negative directions and repeated wheel behavior remain unverified.
See PROJECT_CONTEXT.md for the current artifact and handoff.
See
[research/RADIO_ANALYSIS.md](research/RADIO_ANALYSIS.md). The default HID build and
the receive-only `radio` build remain separate.

## Bounded wireless HID receiver

Build with `firmware/build.ps1 -App receiver`. This separate CDC+HID application
uses local USB ID `2FE3:0008` and product `DIY Mouse RX HID Probe`.
Its UF2 is staged outside OneDrive in
`C:\Users\Gabriel\zephyrproject\build-mouse-receiver\zephyr\zephyr.uf2`.
Do not flash until the build and application-only UF2 validation pass.
Build and UF2 validation now pass: 236 blocks, payload `0x26000..0x34BFF`;
Current receiver v3 SHA256 `4bc43f3543638f337eb7d50d42621cc20be321e79ebdefed2ba8d94d9b73d4da`.
All eight tests, affected builds and separation checks pass. V3 is flashed and
passed passive COM12/HID startup and a9974ms ACK-span movement trial with586
unique type1 reports,no errors/USB busy,and user-confirmed pointer movement.
The next trial confirmed Windows left/right clicks,wheel up/down and stopping
with no stuck buttons:383 unique type1 reports,611 ACKs over9980ms,no errors or
USB busy. A further trial validated forced LEFT release at expiry:final ACKed
report still had LEFT held,and user confirmed other-pointer motion no longer
extended text selection while LEFT remained physically held.617 ACKs over9983ms,
no errors/USB busy. USB fault-path release and continuous recovery remain open.
V2 already
forwarded87 unique movement reports and produced user-confirmed wireless Windows
pointer movement followed by the bounded stop.

It stays passive until serial `p`, then uses the proven discovery/ACK protocol
for at most ten seconds and 1,024 ACK attempts in v3. The non-HID probe keeps its
two-second/256-attempt limits. Validated unique reports forward
five button bits, signed16 X/Y and signed8 wheel to USB; idle and retransmitted
reports do not replay movement. USB busy withholds ACK. Test termination/error
attempts neutral button release with bounded waits. USB reset/disconnect after
configuration requires a board reboot. No input is generated during passive startup.
Idle/status packets never generate input. Use a45-second capture to cover the
30-second discovery arming window plus the ten-second active session.

This first HID bridge keeps USB text logging and radio polling; neither latency
nor1000Hz operation is established. The original HID demo, passive logger and
non-HID probe remain available. See PROJECT_CONTEXT.md for exact build/hardware state.
