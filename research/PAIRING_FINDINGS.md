# Series One PRO pairing investigation

Date: 2026-11-14 (Asia/Manila).

## Evidence and provenance

- User's mouse: wired `258A:2018`, CORE reports firmware `v1.0.0.0`.
- Glorious standalone pairing utility `3.0.0.7` recognizes this mouse.
- Download: https://downloads.gloriousgamingservices.com/download/GloriousPairingUtility.zip
- Downloaded self-extracting EXE SHA256:
  `4A1BAFB07E4A72448970DFFDB5C321839079A393645217119D34588E8EB475CB`.
- 7-Zip extracted Config.ini and a .NET executable. ILSpyCmd 9.1.0.7988
  was used for static inspection. Decompiled vendor code and bundled HidSharp
  are retained only in ignored `build-pairing-analysis/`, not added to source control.
- Config.ini maps Series One PRO to mouse `258A:2018` and receiver `258A:2031`.
  Its golden-dongle `258A:2032` entry includes this model, but replacement hardware
  compatibility has not been physically tested.

## Hardware result: address query succeeded

The mouse has seven HID collections. The vendor interface `MI_02` exposes
65-byte Feature reports (including report ID), with no input/output reports.
The ordinary pointer interface `MI_00` is not the configuration interface.

`read_mouse_address.ps1` sent the query constructed by the utility's
`MainForm.GetRFAddress` method. It received status `0xA1` and a four-byte address.
The exact request, reply and address are saved in ignored
`build-pairing-analysis/mouse-address-observation.json`.

This establishes a device-specific value the utility calls an RF address.
It does NOT establish the on-air address width, byte ordering, Nordic BASE/PREFIX
register mapping, or whether additional address bytes are used.

## USB pairing behavior from static inspection

All indices below include the Windows report-ID byte at index 0.
Unspecified request bytes are zero; buffers are 65 bytes.

| Operation | Request bytes | Reply/use | Evidence status |
| --- | --- | --- | --- |
| Read RF address | `[4]=04`, `[6]=8A` | Address in `[7..10]`; utility accepts status `[1]=A0` or `A1` | Confirmed on user's wired mouse, status A1 |
| Write receiver address | `[4]=04`, `[6]=0A`, `[7..10]=mouse address` | Requires status A1 | Static only; NOT sent |
| Subsequent receiver command | `[4]=02`, `[7]=C0` | Follows successful address write | Static only; exact device-side effect unverified; NOT sent |

Transport helper `ALsbW19wdDh1ViGN9N.jeuxZyOrRk` calls its Feature-report path
with default mode 3: SET_FEATURE, 200 ms delay, GET_FEATURE. No checksum is
added for these calls (optional checksum flag defaults false).

MainForm's ordinary pairing branch copies the mouse's displayed address into
the receiver request. Its distinct scan-pairing branch is for indices 9 and 11;
do not assume its `0C/8C` commands apply to Series One PRO.

Local static evidence locations (ignored decompiler output):
- `GloriousPairingUtility/MainForm.cs`: `GetRFAddress`, `DeviceArrived`,
  `AOk9FfXF0`, `SelectModelOK`, `SetPairStatus`, `GetPairStatus`.
- `n8wpvbrr7rjtj6dRLU/ALsbW19wdDh1ViGN9N.cs`: `jeuxZyOrRk`, `Uwhx2gTxgY`.

## Firmware investigation

Public research repository:
https://github.com/0zLloen/mouse-firmware-re

Inspected commit: `68baf22302820bb0b927093da99493248a086bc7`.
This is third-party evidence, not independent validation on this mouse.
It documents firmware layout and bootloader reads, but does not provide a
complete recovered radio protocol or ready replacement receiver.

Official manifest retrieved successfully:
https://gloriouscore.nyc3.digitaloceanspaces.com/Glorious_Core/Version.json

Its Series One PRO firmware URL returned HTTP 403 when checked:
https://nyc3.digitaloceanspaces.com/gloriouscore/Glorious_Core/v1.0.0.0_20220810_Series_One_ProFirmware.zip

No cached HEX/BIN/ZIP firmware package was found in the installed CORE's
ProgramData directory. No updater was run.

## Application firmware read — completed

`read_mouse_firmware.ps1` is a Windows adaptation of the documented read protocol,
restricted to the wired mouse on firmware 1.0.0.0 and application range
`0x6000..0x1CC9F` (93,344 bytes, including 16 bytes past the reported stock image).

Protocol reference:
https://github.com/0zLloen/mouse-firmware-re/blob/68baf22302820bb0b927093da99493248a086bc7/platforms/compx-nrf52/bootloader-protocol.md

It checks identity, requests bootloader mode, reads through VerifyByAddress,
and attempts to return to application mode in a finally block. It contains no
Erase/Program command and refuses to overwrite an existing snapshot.

Observed result on 2026-11-14, after the user closed other software and confirmed readiness:
- Bootloader `258A:2103` enumerated and reported version bytes `B0 01 00 00`.
- Read completed: 93,344 bytes, saved as ignored
  `build-pairing-analysis/mouse-app-6000-1cc9f.bin`.
- SHA256: `965c8f8cc17e912d0ede5f0ed82b4c12a15d21fae931aeaf59921c94794e1738`.
- Initial stack pointer `0x2000AF50`; reset vector `0x000063DD`.
- Both mode-changing SET transfers reported an error during USB disconnect;
  bootloader and application re-enumeration were checked successfully afterward.
- Normal application `258A:2018` returned, and the user confirmed movement/clicks work normally.
- No erase or flash-program commands were sent. The reader was not run again.

Important limits for any future read:
- Temporarily interrupts mouse input and changes USB identity to `258A:2103`.
- Keep another pointer available and close CORE/pairing utility before running.
- The snapshot excludes bootloader and pairing/calibration pages. It is NOT a
  full-chip backup or authorization to modify the mouse firmware.
- Packet/bounds/response self-tests passed, followed by the successful hardware read above.

Run tests without hardware:
```powershell
& "$env:WINDIR\SysWOW64\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File .\research\read_mouse_address.ps1 -SelfTest
& "$env:WINDIR\SysWOW64\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -File .\research\read_mouse_firmware.ps1 -SelfTest
```

ExecutionPolicy Bypass applies only to each process; no system policy was changed.
Hardware access requires running outside the restricted sandbox. The scripts use
the utility's 32-bit HidSharp 2.0.2 DLL extracted locally; no vendor binaries are
distributed in the repository.

## Still unknown

See `RADIO_ANALYSIS.md` for settings recovered from this snapshot. Their over-air
behavior has now been partly validated: two captures received CRC-valid discovery
packets with the USB-read address mapping. Subsequent probes established a
discovery-to-data transition, and V4 observed sequence0->1 after one data ACK;
see RADIO_ANALYSIS.md for capture evidence and measured timing. Sustained
acknowledgments, movement payload meaning, and polling performance remain unverified.
Do not implement speculative RF decoding or flash the stock receiver image onto
the nice!nano. Its bootloader/memory layout differs from the original receiver.
