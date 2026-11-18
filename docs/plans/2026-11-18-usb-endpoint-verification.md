# Read-only live USB endpoint verification

Goal: read actual full-speed HID interrupt-IN interval and active pipe on restored
normal v14. This verifies descriptor polling configuration, not physical cadence,
fresh sensor reports or gaming latency. No firmware, serial, reset/SET/vendor
requests, driver changes, identifiers/addresses/string dumps or input injection.

Use existing Windows SetupAPI/hub GET_NODE_INFORMATION,
GET_NODE_CONNECTION_INFORMATION_EX and GET_DESCRIPTOR_FROM_NODE_CONNECTION APIs,
following Microsoft's USBView model with installed SDK/MSVC. Enumerate readable
hubs, require exactly one local2FE3:0008 target and no incomplete enumeration.
Validate full-speed, currentconfiguration, bounded descriptors, unique HID alt0
interrupt-IN endpoint and matching activepipe. Fail closed on ambiguity/truncation.
JSON output contains only public descriptor fields; logs/binaries privateignored.

Files: research/inspect_usb_endpoint.c with --self-test (no hardware) covering
bounds, class/direction/type selection, zero interval/packet and duplicates.
First stub parser must fail the validfixture before implementation. Build with
installed MSVC/WindowsSDK, run selftest, then readonly hardware check. Preserve
source/build invocation/results and update context/push. Normal v14 stays active.

References:
https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/usbioctl/ns-usbioctl-_usb_descriptor_request
https://github.com/microsoft/Windows-driver-samples/tree/main/usb/usbview

The current inspector deliberately verifies full-speed bInterval1 only. Other Windows/full-speed intervals can map to rounded scheduling periods; refuse them rather than extrapolate1000/bInterval. Expand mapping/tests only if another interval is needed.


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
