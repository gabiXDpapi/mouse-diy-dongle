# MOUSE DIY DONGLE

Experimental mouse receiver firmware for a nice!nano-compatible ProMicro nRF52840.
Plug-and-play wireless Windows pointer control is user-confirmed on normal v14.
Long-term reliability and gaming performance remain under development; the default
build is still the original USB HID demo.

## Glorious CORE compatibility work

CORE integration now takes priority over the pending four-hour reliability trial.
Wired readback confirms firmware 1.0.0.0, a 1000 Hz setting, profile-1 DPI stages
400/800/1600/3200 and currently selected 800 DPI. General lighting is disabled in
CORE's support record for this model; DPI-stage colours use a separate command.
See [protocol findings](research/CORE_COMPATIBILITY.md).

The opt-in `-App config-read` v20 diagnostic sends one fixed wireless firmware-
version query, suppresses pointer output and emits its result only after CDC `x`
then `d`. Native codec/integration checks, UF2 validation and the first hardware
read-query roundtrip pass: firmware 1.0.0.0 matches the wired reference, with
eight fragment attempts and a complete 16-byte reply. It does not yet expose CORE-compatible configuration.
Keep the mouse wired/OFF before flash; restore the exact preserved, tested normal
v14 image afterward. No settings writes or mouse bootloader commands exist in
this experiment. Do not use the diagnostic for ordinary mouse operation.

V21's vendor interface failed to start in Windows (Code10); no settings write
occurred. V22 corrected the vendor descriptor, started both HID collections and
completed a USB-to-radio firmware-version read matching the wired reference.
Exact tested normal v14 was restored and ordinary wireless control confirmed.

The current `-App config-hid` staging build is v23. It prepares reusable reads
of version, active profile, polling and profile-1 DPI settings, plus a narrowly
allowed selected-stage change. Native/host checks, UF2 validation and first hardware trials pass: five read
queries and a verified 800 -> 1600 -> 800 selected-stage change/restore. The
recovery backup confirms no pending restore. It retains the working v22 descriptor,
prototype USB IDs and disabled pointer output. CORE cannot attach yet.
`research/test_receiver_config.ps1` defaults to read-only; `-ExerciseDpi` requires
a separate ready cue, saves a recovery backup and verifies original-stage restore.
A failed restoration requires the saved wired recovery before further use.
See [reusable configuration plan](docs/plans/2026-11-20-reusable-config.md).

## Plug-and-play receiver

The new `dongle` build (v14) starts automatically after USB configuration and
runs without a serial reader or manual arm. It has no fifteen-minute or packet
budget stop. A mouse link gap releases buttons and returns to discovery. USB
reconnect starts normal operation again. Optional CDC `x` stops output until
explicit `p` or reboot; errors fail closed.

```powershell
& ./firmware/build.ps1 -App dongle
```

This candidate passes native checks, build/config/source and UF2 validation;
**it is flashed and USB enumeration is verified** as DIY Mouse Dongle v14,
with HID and CDC present. No serial reader or arm command was used. Automatic
wireless input is recorded without serial activation: receiver events span over
eighteen minutes, with zero capture read errors. The user confirmed smooth
automatic startup, receiver USB-replug recovery, both mouse-only power cycles and
continued control past the old limit. It kept working after recording ended.
V13 remains the preserved rollback. Normal mode disables serial-console diagnostics and the
boot banner to work independently of DTR. Keep the mouse wired/OFF before flash.
The existing v13 serial capture controller is for the bounded diagnostic image,
and must not be used with the silent normal build.

`-App receiver` retains finite diagnostic sessions and adds explicit rearming
only after a completed neutral stop. Both variants share RF/HID behavior.
The sections below describe the validated v13 diagnostic baseline and history.

## Quiet latency measurements

`firmware/build.ps1 -App latency` builds the v15 timing image; `-App latency-fast`
builds the v16 comparison with a 10 us diagnostic polling wait instead of 50 us.
Both collect fixed-memory histograms without active serial text. Only after `x`
neutralizes input can `d` emit a frozen summary. Native tests, builds and both
first hardware captures passed; users reported smooth motion and clean stops.

| Instrumented receiver metric | v15, 50 us wait | v16, 10 us wait |
|---|---:|---:|
| Accepted reports matching host records | 37,597 | 36,065 |
| Mean RF END to bridge entry | 42.95 us | 21.61 us |
| Mean RF END to USB submission start | 52.46 us | 30.91 us |
| Mean RF END to USB-completion callback | 0.609 ms | 0.525 ms |
| Maximum RF END to USB-completion callback | 1.153 ms | 1.107 ms |
| p99 histogram upper bound, RF to USB callback | 1 ms | 1 ms |
| USB-busy candidates / all ACK candidates | 2.03% | 0.303% |

The tool `research/analyze_latency.py` validates histogram, sample-count and time
component reconciliation. These are short, instrumented receiver-stage timings,
including callback dispatch and instrumentation overhead. They exclude sensor,
earlier mouse queue/retry waits, OS/game and display latency. Percentiles are
histogram upper bounds. Manual motion and USB phase differed between runs; this
single comparison does not establish causality or long-term reliability.

A reverse-order repeat now provides two short runs per condition. RF-to-USB
callback means were 0.609/0.503 ms for the 50 us baseline and 0.525/0.583 ms for
10 us polling. Radio handling stayed faster, but overall latency and backpressure
did not improve consistently. Keep normal v14 while investigating USB timing and
input preservation. In the last baseline, 27,594 device reports corresponded to
27,592 host Raw Input records; that unexplained difference is not proof of RF loss,
and prevents a claim of lossless end-to-end input from report counts alone.

The exact tested normal v14 image has been restored and its USB identity verified.
The faster polling setting is **not promoted to normal use**. Repeatability,
longer reliability and gaming qualification remain next steps. The timing images
and measurements are preserved separately for those tests.

## Project stack

V13 was the finite fifteen-minute diagnostic baseline:
983,040 ACK attempts and 900,000 ms from first link, with eight discoveries,
one arm per boot, explicit `x` stop and passive USB restart. It restores the
tested v11 bridge; v12's startup-neutral experiment did not fix the held-state
symptom. V13 is built, flashed and software-checked; passive startup passed on COM12.
Its full fifteen-minute trial passed: smooth movement, left/right clicks and
wheel after both mouse-only OFF/ON cycles, followed by a clean original deadline
stop. Full 935-second serial/Raw Input recording completed with 92,400 receiver
records and zero read errors. At the end of those historical trials it was expired/neutral; a new diagnostic
arm required a normal reboot and fresh physical readiness.

`research/capture_receiver_session.ps1` records 935 seconds of serial and
mouse-only Raw Input. Supply the current user-approved UTC finish deadline;
it refuses to arm with eighteen minutes or less remaining. A per-job
`.stop-request` file sends `x` once; early-abort raw data is explicitly partial.
The controller's deadline/stop self-check and syntax checks pass.
Use `-ForegroundMotion` for a focused 100-second visible-window capture; it
sends `x` once about 60 seconds after the first link and confirms neutral stop.
This mode passed its first hardware run: 39,210 foreground records, zero read
errors, smooth user-reported motion and confirmed explicit stop. Median host gap
was 1.007 ms; peak full-second delivery was 988 events. This is host timing,
with manual movement, and does not certify physical polling or end-to-end latency. Full-length
capture and v13 controls/recovery passed in the first fifteen-minute trial.
A second fifteen-minute run also passed with both mouse-only recoveries and
a clean original deadline stop: 75,552 background records, zero read errors.
That is two separate bounded sessions, not continuous thirty-minute validation.
All five buttons and wheel UP/DOWN now have deliberate hardware coverage.
Middle and each thumb button produced five balanced press/release pairs; the
user confirmed clean stop with no stuck buttons. Front thumb maps to Windows
button 5, rear thumb to button 4. The intended desk-position check also passed
with smooth motion and clean stop: 39,356 foreground events, zero read errors,
median host gap 1.003 ms and peak 1,000 events in a full second. Distance and
obstacles were not recorded. Physical polling/latency and broader RF robustness
remain unverified.

```powershell
& ./research/capture_receiver_session.ps1 -SelfTest
# Only after separate physical readiness, before 17:42 Manila on November 16:
& ./research/capture_receiver_session.ps1 -FinishBeforeUtc '2026-11-16T10:00:00Z'
```

| Technology | Use |
|---|---|
| C and Zephyr RTOS 4.2 | Receiver firmware, USB drivers, timing and synchronization |
| nRF52840 radio registers | Proprietary 2.4 GHz packet reception and ACK transmission |
| Zephyr SDK, west, CMake and Ninja | Board configuration, compilation and linking |
| USB HID and CDC serial | Windows mouse input plus commands/diagnostics |
| UF2 bootloader | Validated firmware copying through NICENANO |
| Python, unittest and native GCC | Protocol/USB behavior checks, image validation and analysis |
| PowerShell | Build, hardware inspection, flash and serial-capture workflow |
| Win32 Raw Input and MinGW C | Separate Windows wired/wireless mouse-event logger |
| Git and GitHub | Versioned experiments and tested milestones |

V8 is the validated hardware baseline. V9 adds a receiver-only lowercase `x`
stop and has passed software checks; it is flashed and passive startup has passed. After a
successful stop response, a receiver reboot is required before arming again.
V9 held-left explicit stop and smooth motion are user-confirmed. V10 extends the
finite cap to 327,680 ACKs/five minutes, keeping the other guards. It is built
and flashed. Its first five-minute trial passed: the user confirmed smooth
movement/clicks/wheel after two mouse-only OFF/ON cycles and a clean original
session stop. Sessions remain bounded; USB reconnect recovery and longer-term
reliability are still pending.

V11 is a flashed USB-recovery experiment with passive startup verified: configured USB reset or
disconnect requests a software restart to passive startup; a new explicit arm is
required. Generic USB errors remain fatal. A targeted Windows restart while configured/passive returned a fresh unarmed
v11 startup without errors. A fresh explicit arm afterward restored smooth
controls and x-stop held-button release by user report. Active USB-loss testing returned to passive startup but FAILED held-button
release: selection still followed the other pointer. The host state was cleared
with Esc/button release and receiver unplug. Startup-neutral reporting is the
next isolated experiment; v11 is not validated for abrupt active USB loss.

V12 is a flashed startup-neutral experiment with passive startup completion verified for that failure. It sends
one all-zero HID report after USB configuration and waits for completion before
allowing RF input. It adds no button press or motion. The active PnP-reset retest still left Windows LEFT held (host probes and user
confirmed). The host was cleared by Esc/releases/receiver unplug. This did not
fix the PnP case;physical USB removal is the next separate diagnostic.

The physical receiver-unplug trial also retained the held state. A wired
Glorious reference reproduced the same physical-unplug behavior on this PC/app.
Treat physical-removal held state as the local baseline,not a DIY-specific
regression. Wired PnP restart was not compared;keep that scenario separate. This uses only read-only mouse capture, no serial/RF/UAC.

```powershell
& ./research/capture_wired_usb_loss.ps1 -SelfTest
# After wired physical readiness; this example is17:10 Manila on November16:
& ./research/capture_wired_usb_loss.ps1 -FinishBeforeUtc '2026-11-16T09:10:00Z'
```

The60s reference refuses to start unless another60s of wrap time fits before
the supplied break. Its1s smoke and deadline-refusal checks pass;wired held-loss
behavior matched the receiver on this setup. Physical observations and raw transition counts must
be reported separately.

## Physical USB-removal diagnostic

The next comparison uses installed v12 without rebuilding or flashing. It records
one arm, read-only LEFT state and serial loss while the user physically unplugs
only the receiver with LEFT held. No UAC or PnP action is used. User confirmation
is required;serial loss alone does not establish physical removal or release.

```powershell
& ./research/capture_physical_usb_loss.ps1 -SelfTest
# Only after physical readiness; use the current user's explicit UTC arm cutoff:
& ./research/capture_physical_usb_loss.ps1 -ArmBeforeUtc '2026-11-16T09:06:00Z'
```

That example cutoff is specific to November16,2026 at17:06 Manila. Expired
cutoffs are refused. Capture is120s;no-link/cancel/no-unplug fallback sends x
once while connected. No automatic reopen/rearm. All output is private/ignored.
Self-checks pass;the physical-removal result is still pending.

## Active USB-reset capture tools

No firmware rebuild is needed for the next v12 test after verified flashing. The deferred
receiver-only helper obtains UAC approval before the held-input observation,
waits at most 180 seconds for its unique job signal, and revalidates the exact
receiver target before one PnP restart. The controller refuses reused/expired
jobs and existing capture files. All job data and device instance IDs are private.

```powershell
& ./research/restart_receiver_usb_deferred.ps1 -SelfTest
& ./research/capture_active_usb_reset.ps1 -Mode SelfTest
# After UAC readiness; this prepares a helper but does not restart or arm:
& ./research/capture_active_usb_reset.ps1 -Mode Prepare
# Only after separate physical reboot/OFF readiness, using the returned job:
& ./research/capture_active_usb_reset.ps1 -Mode Capture -RunName <ready-job>
```

Capture needs at least 75 seconds of helper life left. It arms once, requests
receiver restart 25 seconds after link start, and observes passive recovery
without another arm. A 150-second hidden Raw Input recording supports button
observations, not foreground cadence claims. Self-checks pass; the v11 trial exposed held-button persistence and the v12 retest is pending.
The controller requires the v12 startup-neutral marker and records read-only
host LEFT state before/after reset. Windows settings remain unchanged.

## Host Raw Input diagnostic

The diagnostic compares wired mouse and DIY receiver reports arriving at
Windows while keeping CORE's saved1600 profile and Windows settings unchanged.
`research/capture_raw_mouse.c` registers mouse usage only,filters known device
IDs,and writes monotonic arrival times and raw motion/button/flag fields to CSV.
It leaves normal pointer input enabled and never registers keyboard input.
Raw Input motion is separate from Control Panel pointer-speed processing:
https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse

Build with the existing Windows MinGW toolchain:

```powershell
gcc -std=gnu11 -Wall -Wextra -Werror -O2 research/capture_raw_mouse.c -o build-pairing-analysis/capture_raw_mouse.exe -luser32 -lshell32
& ./build-pairing-analysis/capture_raw_mouse.exe --self-test
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
& ./build-pairing-analysis/capture_raw_mouse.exe 45 "build-pairing-analysis/raw-wired-$stamp.csv" --foreground |
    Tee-Object -FilePath "build-pairing-analysis/raw-wired-$stamp.stdout.txt"
```

Run controlled input only after physical readiness. Duration is1..1200s;CSV paths
must be new(existing files are refused). Source labels are `wired` or `receiver`;
full device paths are not written. All CSV/executable outputs remain ignored.
Self-checks, timed 1s hidden/visible lifecycle and invalid-option checks pass.
Wired and wireless background captures completed; wired felt smooth, wireless
occasionally overshot during circles and stopped cleanly. Background event rates
cannot establish polling or loss: Windows can throttle/coalesce background listeners.
Use `--foreground`, click the capture window and keep it active during movement.
CSV `input_code=0` is foreground delivery; `1` is background. The activation click
is recorded normally and should be excluded from motion-only analysis.
https://blogs.windows.com/windowsdeveloper/2023/05/26/delivering-delightful-performance-for-more-than-one-billion-users-worldwide/
Native execution may require elevation in the Codex sandbox. Controlled
foreground wired/wireless comparison now completed: wired smooth, v7 still
occasionally overshoots. V8 suppresses periodic/per-busy text while linked and
retains cumulative busy totals. In its first foreground motion trial the user
reported no overshoot, wired-like control and a clean stop. V8 movement/clicks/wheel and mouse-only OFF/ON recovery are also user-confirmed
with clean stops. Held-left expiry release on v8 is also user-confirmed; sessions stay bounded. Host arrival
times alone do not measure sensor DPI, RF loss or physical input latency.

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
Current receiver v7 build and UF2 validation pass:240 blocks,payload
`0x26000..0x34FFF`,SHA256 `b6a63b99667a764ed32efb2a3f46bf0721c99f43ecbd9131f81ea08370fde16a`.
V7 is flashed and its motion comparison felt definitely more consistent than
v6 to the user,but residual inconsistency remains. One continuous link,59121
ACKs,47750 unique HID submissions,65 retries,191 USB-busy events,no errors.
This bounded experiment holds sequence for idle ACKs to
avoid a possible unsent-queue release;data/status ACK+1 and all other behavior
stay unchanged. The risk is inferred from the saved mouse firmware;no v7
motion-quality improvement or original-receiver ACK behavior is established.
Non-HID probe keeps its earlier idle+1 behavior. Tested v6 is preserved locally
at ignored `build-pairing-analysis/receiver-v6-c0e18e29.uf2`.
For the next DPI-controlled comparison,Glorious CORE's wired Profile1 had1600
DPI selected and was saved unchanged. No separate wireless DPI control was
exposed;wireless retention/equivalence still needs checking. Windows settings
and receiver1:1 motion mapping remain unchanged.
V6 is flashed and hardware-tested:controls worked before/after mouse power-cycle,
clean stop,and user-reported much better responsiveness. Intermittent pointer
stops during continued motion remain unresolved. A focused motion trial had no
noticed hard freezes,but user-reported varying pointer speed/sensitivity persists;
wired comparison is next. Its mixed-input trial had55417 ACKs,
14557 unique HID submissions,51 retries,95 USB-busy events and no errors.
It retains quiet logging and adds cumulative successful
idle/status/type1 ACK counters,HID unique-submission/retry counters,and a finite
65536-ACK cap. USB submission does not prove Windows delivery. The60s session,
eight-discovery cap,1s link gap and RF/USB report mapping remain unchanged.
V5 reduces per-packet USB text to1s counters,retaining discovery/first-ACK timing
samples per link,link transitions and error/busy logs. V5 is flashed and passed
passive COM12/HID startup. Its input-delay comparison was inconclusive:8192 ACK
budget exhausted within roughly10s of observed counter progress;user reports
no usable pointer/button response. No latency improvement is established.
The tested v4 image is preserved at ignored
`build-pairing-analysis/receiver-v4-2162a852.uf2`.
All eight native tests,receiver/probe/radio builds and mode-separation checks
pass. V4 previously passed its60s controls/mouse-OFF-ON recovery trial:
3348 ACKs across two links without budget/deadline reset,no errors/USB busy,
user-confirmed controls before/after and stop without stuck buttons.
The user noticed visible pointer delay;latency remains an open limitation.
Hardware-tested v3 is preserved locally at ignored
`build-pairing-analysis/receiver-v3-4bc43f35.uf2`.
V3 previously
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

V6 stays passive until serial `p`,with30seconds for initial discovery. After the
first successful discovery,it permits60seconds total,65536 total ACK attempts
and eight total discovery attempts across all links. A1000ms gap since the last
successful ACK disables radio and attempts neutral release before resuming
discovery scanning. Rediscovery never refills budgets or extends the deadline.
The silence threshold is experimental;RF recovery still needs a hardware test.
The non-HID probe keeps its one-discovery/two-second/256-attempt limits.
Validated unique reports forward
five button bits, signed16 X/Y and signed8 wheel to USB; idle and retransmitted
reports do not replay movement. USB busy withholds ACK. Test termination/error
attempts neutral button release with bounded waits. USB reset/disconnect after
configuration requires a board reboot. No input is generated during passive startup.
Idle/status packets never generate input. For v4/v5/v6,use a95-second capture after
arming to cover30seconds initial discovery plus60seconds total session and
cleanup margin. Keep one continuous serial reader and save/drain startup output
before sending one `p`;confirm NEW `RECEIVER armed:` text before mouse power-on.
V5 has no raw RX/per-ACK lines;analyze cumulative stats/link-stop totals instead.
Timing lines are samples and cannot be counted as total transmissions.
V3's earlier ten-second/1024-ACK sessions used45-second captures.

This first HID bridge keeps USB text logging and radio polling; neither latency
nor1000Hz operation is established. The original HID demo, passive logger and
non-HID probe remain available. See PROJECT_CONTEXT.md for exact build/hardware state.


### November 18: input-accounting diagnostic software ready

Normal v14 remains installed. The opt-in `input` build (USB product `DIY Mouse Input v17`) adds quiet accepted movement/wheel/button totals with a stopped-only dump and a matching Raw Input analyzer. Software checks and both diagnostic/normal-control builds passed; hardware validation is pending. This preserves baseline polling and does not promote the faster experiment. Equal aggregate totals cannot prove individual-event preservation or explain the earlier two-record count difference. See [the diagnostic plan](docs/plans/2026-11-18-input-conservation.md).

V17 hardware accounting also passed one foreground trial: 9,585 device/host reports and movement, wheel and five-button totals matched; the user confirmed smooth operation and clean stopping. The earlier count difference remains unexplained. See `research/RADIO_ANALYSIS.md` for scope and limitations.

Normal v14 also recovered smoothly after a user-confirmed five-minute inactivity interval without reconnecting. Deep sleep and Windows sleep/resume remain unqualified; see the research notes for recording coverage.

One Windows S3 sleep/resume trial also passed on normal v14 with released buttons: the user reported automatic smooth recovery of all controls, and Windows events plus post-wake receiver presence were verified. Repeated sleep, hibernation and long endurance remain pending.


### USB frame-phase diagnostic v18: software preparation

The opt-in `phase` build retains baseline 50us polling and v17 input accounting.
It uses an otherwise disabled TIMER3 and a hardware PPI SOF capture, checks the
startup frame cadence, and separates accepted USB submissions into ten 100us phase
buckets plus an explicit invalid bucket. Stale or changing references are not
counted as valid phases. Per-bucket counts, timing sums and latency histograms must
reconcile with the complete stopped timing frame. No queued SOF callback is added.

Normal v14 remains installed while software is prepared; v18 hardware validation
is pending. This instrumented submission-phase measurement includes USB callback
dispatch delay and is not physical end-to-end latency or evidence for promoting
faster polling. The phase controller requires complete timing, input and phase
footers; native output is cross-checked with the host analyzer. See the phase
implementation plan and latest PROJECT_CONTEXT handoff for exact image/state.

V18’s first hardware trial passed SOF calibration, complete phase accounting and exact device/host input totals (21,818 reports). The user confirmed smooth controls and clean stop. Callback latency was strongly associated with USB frame phase in this run; this does not prove an optimization gain. See the research table for counts, stage timings and limits.


### Phase-aware fast-poll comparison v19 (software ready)

`-App phase-fast` builds `DIY Mouse Phase Fast v19`. It reuses the exact C source
and private radio parameters of the hardware-qualified v18 phase image. The
existing fast-poll flag is ON (10us instead of 50us); the Zephyr configuration
differs only in USB product name. Native/research checks, controller gates, build,
source/config and UF2 verification passed. Hardware behavior and benefit remain
unproven; normal v14 stays installed until physical readiness.

Preserved image hash:
`8ad6dd031a595b4558112c47f915f52b4dc5ebd0754213cd89ff46b0aff9b17e`.
Comparison plan: `docs/plans/2026-11-18-phase-poll-comparison.md`. No optimization
promotion is assumed. Coarse100us phase buckets and manual workload limit any
causal interpretation of a small difference; compare independent runs explicitly.

V19’s first phase-aware hardware run passed input accounting and user control checks. Faster polling reduced measured RF reaction, while total callback timing remained sensitive to USB phase mix; the small phase-weighted difference does not establish a reliable overall gain. Normal v14 remains the everyday baseline; repeatability is pending.

The completed phase-aware four-run comparison shows a repeatable~18us RF-to-submit reduction with10us software radio checking. It does not establish a reliable whole-system gaming-latency gain, so exact tested normal v14 (50us software wait, configured1ms USB interval) is restored and user-confirmed. Next: live USB endpoint/polling verification and sustained reliability checks.


### Live USB polling configuration verified (November 18)

A read-only Windows hub/descriptor check on restored normal v14 verified:

| Field | Live value |
|---|---|
| Link speed | Full speed |
| HID interface | 2 |
| Interrupt IN endpoint | 0x83 |
| Maximum packet | 16 bytes |
| bInterval | 1 |
| Active pipe | Matches configuration descriptor |

For this full-speed endpoint, bInterval1 requests a1ms interrupt period (1000Hz).
This verifies the live USB configuration, not actual bus cadence, sustained fresh
sensor reports, host delivery timing or physical gaming latency. The read touched
no serial connection and made no reset/SET/vendor requests, input injection,
firmware flash, setting or driver changes. Both present hubs were readable and
exactly one matching local receiver was found. Normal v14 remains active.

Tool: research/inspect_usb_endpoint.c. Build and hardware-free self-test:
`& research/build_usb_endpoint.ps1` (installed MSVC/Windows SDK). Run the built
private `build-pairing-analysis/inspect_usb_endpoint.exe` for the read-only check.
The parser rejects truncated/malformed/ambiguous descriptors and checks active
pipe correspondence. This version deliberately refuses full-speed intervals
other than1; it does not generalize Windows interval mapping by dividing1000 by
arbitrary bInterval. Logs/binary/object remain ignored. Private verification JSON
records exact source/binary hashes and result.

Primary interval reference:
https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/usbspec/ns-usbspec-_usb_endpoint_descriptor


### Guided normal-receiver movement delivery measurement

The new `--timed-motion` mode waits for capture-window activation, shows a5-second
countdown, cues30seconds of movement, then5seconds STOP and closes automatically.
Focus loss during countdown/MOVE invalidates the trial. Normal mouse input stays
enabled; no firmware, serial, input injection or settings changes. Existing
background/foreground modes and CSV fields remain compatible, with the1200-second
hard cap retained. The old capture executable is preserved separately.

Build/self-test: `& research/build_timed_raw_mouse.ps1` creates the private
`capture_raw_mouse_timed.exe`. Native checks cover phase boundaries/focus loss and
existing raw filtering/bounds/format. Seventeen analyzer tests and the controller
completion/error/abort self-check pass. Live guided-window qualification is pending.
Controller: `research/capture_timed_motion.ps1` verifies current source/header/binary
hashes and normal v14 identity, then runs a bounded180-second recording that closes
early after the guided motion. It analyzes only after successful process closure.

`research/analyze_motion_delivery.py` uses the predefined central20seconds, reports
all20 one-second movement-event counts and gap distribution, and rejects invalid
markers, background/absolute/control input or incomplete CSV. These are host
WM_INPUT movement-delivery statistics, not measured physical USB tokens, fresh
sensor sampling, RF loss or end-to-end gaming latency. Queueing/coalescing and
actual hand motion remain confounders. Live1ms descriptor evidence is separate.

The first guided normal-wireless trial completed, but an initial event gap conflicts with the user’s report of smooth continuous motion. A matching wired capture is next to check the host/capture path. No sustained1kHz claim is made; the live1ms USB configuration is separately verified.

A guided larger-motion run on normal v14 now supports near1,000Hz foreground host delivery:964.9movement events/s over the entire predefined20seconds,916–993each second, median1ms; the user confirmed smooth continuous motion. This is not physical bus/sensor latency certification, and occasional host gaps remain. Normal v14 stays installed; real gameplay and longer reliability checks are next.


### Preliminary30-minute gameplay check passed (November19)

On exact tested normal v14, the user reported completing30minutes of gameplay,
with smooth aim/movement throughout, no issues or interruptions, and actual use
of left/right clicks, wheel, middle and both thumb buttons. The completion report
arrived more than30minutes after the recorded start cue. Game/mode was not
specified. This is a user-reported manual-timer result; no input recording,
automated gameplay, continuous health monitor or objective latency measurement
was running. Exact physical play start/end are not independently observed.

Normal v14 remains the known installed automatic receiver, with no firmware,
serial, driver or settings change during this check. This passes preliminary
in-game functionality/reliability for the reported session. It does not establish
4h/8h/multiday reliability, fresh sensor1kHz, perfect jitter or physical input-to-
screen latency. The previously observed rare15.279ms host gap remains unexplained.
Next: a separately started four-hour ordinary wireless-use window if practical,
recording actual use and interruptions rather than counting earlier mixed trials.
