# Glorious Series One PRO — DIY 2.4 GHz Wireless Receiver

An open-source, independently written 2.4 GHz USB wireless receiver firmware for the **Glorious Series One PRO Wireless** gaming mouse using an inexpensive **nice!nano V2 / ProMicro nRF52840** development board (for replacing a lost original dongle).

> **Status:** Hardware-verified on Windows 11 with plug-and-play wireless control, **1,000 Hz** USB polling, **Glorious CORE** wireless detection/configuration, and **OpenMouse (WebHID)** support.

---

## Features

- **Plug-and-play wireless mouse control**: Automatic discovery/reconnect, smooth motion, all 5 buttons, scroll wheel, and safe neutral button release on link loss.
- **1,000 Hz USB polling**: Full-speed `bInterval = 1` (1 ms) HID reporting, verified via native Windows Raw Input (`WM_INPUT`, ~1.00 ms median inter-event gap).
- **Glorious CORE & OpenMouse compatibility** (opt-in `-App core-id`):
  - Exposes all 7 matching HID collections (Mouse, 64-byte Vendor Config Feature, Keyboard + LED Output, Consumer/Media, System Control, and Vendor Notifications `FFA0:0001` / `FFFF:0001`).
  - Live wireless battery %, firmware version, DPI stages/selection, polling rate, lift-off distance, debounce, DPI indicator colors, and slot-0 button bindings.
  - 4-slot FIFO config queue for back-to-back WebHID writes (OpenMouse) and immediate offline `0xA4` status when the mouse is asleep/off.
- **Safety built in**:
  - Firmware updater, bootloader entry, RF address, pairing, and factory-reset commands are strictly blocked in receiver firmware.
  - Every build runs 15 native C/Python unit tests and validates UF2 address bounds (`0x26000..0xEBFFF`) so the SoftDevice and UF2 bootloader are never overwritten.

---

## Hardware Requirements

1. **Glorious Series One PRO Wireless** mouse (tested on stock firmware `v1.0.0.0`, wired USB ID `258A:2018`).
2. A working **USB-C data cable** (needed once to read your mouse's unique RF address).
3. A **nice!nano V2 / ProMicro nRF52840** board running the standard Adafruit UF2 bootloader:
   ```text
   UF2 Bootloader 0.6.0
   Model: nice!nano
   Board-ID: nRF52840-nicenano
   SoftDevice: S140 version 6.1.1
   ```
   *(Do not flash these images onto boards with a different SoftDevice or bootloader memory layout without checking `firmware/check_uf2.py`.)*

---

## Software Prerequisites (Windows)

Because each mouse has its own unique 4-byte RF address (and proprietary mouse binaries are not redistributed in this repository), you build your personalized `.uf2` locally once:

1. **Zephyr RTOS v4.2.0** and **Zephyr SDK 0.17.2** installed under `%USERPROFILE%\zephyrproject` (with `.venv`, `west`, CMake, Ninja, and Python 3.12).
2. **MinGW GCC** on `PATH` (used by `firmware/build.ps1` to run the 15 native C unit tests).
3. **32-bit `HidSharp.dll`** placed at `build-pairing-analysis\HidSharp.dll` (download the `HidSharp` NuGet package, rename `.nupkg` to `.zip`, and extract `lib/net35/HidSharp.dll` or `lib/netstandard2.0/HidSharp.dll`).

---

## Quick Start: Build & Flash Your Own Receiver

### 1. Clone the repository
```powershell
git clone https://github.com/gabiXDpapi/mouse-diy-dongle.git
cd mouse-diy-dongle
mkdir build-pairing-analysis -Force
```
Copy `HidSharp.dll` into `build-pairing-analysis\HidSharp.dll`.

### 2. Read your mouse's RF address and radio table (one-time, read-only)
1. Close **Glorious CORE** (and any OpenMouse browser tabs).
2. Turn the Glorious mouse's bottom wireless switch **OFF**.
3. Connect the mouse directly to your PC with a working **USB data cable** and confirm wired movement works.
4. Run both read-only helpers in **32-bit (SysWOW64) PowerShell**:

```powershell
# Read your mouse's unique 4-byte RF address (saves build-pairing-analysis/mouse-address-observation.json)
C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -NoProfile -ExecutionPolicy Bypass -File research/read_mouse_address.ps1

# Read-only snapshot of v1.0.0.0 radio register constants (saves build-pairing-analysis/mouse-app-6000-1cc9f.bin)
# No erase or write commands are sent; the mouse returns to normal wired mode automatically.
C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -NoProfile -ExecutionPolicy Bypass -File research/read_mouse_firmware.ps1
```

> **Privacy note:** Everything inside `build-pairing-analysis/` is git-ignored so your mouse's RF address, firmware snapshot, backups, and compiled `.uf2` images stay local on your PC.

### 3. Build and validate the receiver firmware
Choose the firmware variant you want:

- **Full Glorious CORE & OpenMouse compatibility (`258A:2031`, recommended for personal use):**
  ```powershell
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File firmware/build.ps1 -App core-id
  ```
  Output: `%USERPROFILE%\zephyrproject\build-mouse-core-id\zephyr\zephyr.uf2`

- **Plug-and-play mouse only (prototype USB ID `2FE3:0008`, no CORE/OpenMouse):**
  ```powershell
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File firmware/build.ps1 -App dongle
  ```
  Output: `%USERPROFILE%\zephyrproject\build-mouse-dongle\zephyr\zephyr.uf2`

The build script automatically runs all 15 unit tests, validates your local radio evidence, compiles the firmware outside spaces-in-path directories, and verifies the UF2 address bounds.

### 4. Flash the nice!nano
1. Plug the nice!nano into your PC and **double-tap `RST` to `GND`** until the `NICENANO` USB drive appears.
2. Open `INFO_UF2.TXT` on `NICENANO` and confirm it matches `UF2 Bootloader 0.6.0`, `nRF52840-nicenano`, `S140 version 6.1.1`.
3. Copy your validated `zephyr.uf2` to the root of `NICENANO`. The drive will automatically disconnect and reboot into the receiver firmware.

### 5. Use it!
1. Unplug the USB cable from your Glorious mouse.
2. Flip the bottom wireless switch **ON**.
3. The mouse connects automatically — move/click right away, or open **Glorious CORE** / **OpenMouse** (with `-App core-id`) to check battery and configure DPI/polling/LOD/debounce.

---

## Available Build Targets (`firmware/build.ps1 -App <name>`)

| Target | USB ID | Description |
| --- | --- | --- |
| `core-id` | `258A:2031` | **Full receiver**: Mouse + Config Feature + Auxiliary HID (7 collections), Glorious CORE & OpenMouse compatible (opt-in local build) |
| `core-proto` | `2FE3:0008` | Same full 7-collection firmware on local prototype USB IDs (CORE/OpenMouse will not attach) |
| `dongle` | `2FE3:0008` | Minimal plug-and-play wireless mouse receiver (Mouse + CDC only) |
| `aux-hid` | `2FE3:0008` | Wireless Mouse + Auxiliary HID (Keyboard/Consumer/System/Vendor notifications) without Config Feature |
| `radio` | `2FE3:0001` | Receive-only 2.4 GHz diagnostic packet logger over USB CDC |
| `hid` | `2FE3:0007` | Default minimal USB HID mouse movement test (no radio) |

---

## Troubleshooting

- **Windows says *"The file or directory is corrupted and unreadable"* when copying to `NICENANO`:**
  **Do NOT format `NICENANO`.** This is a known Windows FAT cache glitch on virtual UF2 drives. Unplug and replug the nice!nano, double-reset into a fresh `NICENANO` drive, and copy `zephyr.uf2` again.
- **Explorer shows a copy error right at 100%:**
  Normal on UF2 bootloaders — the board reboots into the new firmware as soon as the final block lands. Check Device Manager to confirm the receiver enumerated.
- **`read_mouse_address.ps1` says no `258A:2018` device found:**
  Make sure you are using a USB **data** cable (not charge-only), plugged directly into the PC, with the mouse wireless switch **OFF** and wired pointer movement working.
- **Browser polling testers (or OpenMouse) report ~65–250 Hz instead of 1,000 Hz:**
  Browsers throttle/coalesce Pointer Events to display refresh. Use a native Windows Raw Input (`WM_INPUT`) tool (such as `research/capture_raw_mouse.c` or MouseTester) to measure true 1,000 Hz (~1.00 ms median gap) delivery.
- **Notice on USB IDs:**
  The `258A:2031` identity in `-App core-id` belongs to Glorious/SINOWEALTH and is provided strictly as an opt-in local configuration for personal hardware compatibility, not for commercial product distribution.

---

## Engineering Notes & History

This repository includes the full reverse-engineering trail, native test suite, and hardware validation logs:
- [PROJECT_CONTEXT.md](PROJECT_CONTEXT.md) — Complete milestone history and current hardware state.
- [research/CORE_COMPATIBILITY.md](research/CORE_COMPATIBILITY.md) — Glorious CORE & OpenMouse HID protocol findings, descriptors, and validation results.
- [research/RADIO_ANALYSIS.md](research/RADIO_ANALYSIS.md) — 2.4 GHz radio parameters, discovery/ACK protocol, and latency/phase measurements.
- [docs/plans/](docs/plans/) — Design plans for every milestone from initial radio capture through v30.
