# Auxiliary USB HID Forwarding Plan (v26)

**Goal:** Forward wireless auxiliary RF reports (Keyboard, Consumer Control, System Control, and Vendor `FFA0:0001` / `FFFF:0001` notifications) over a second USB HID interface on local prototype IDs (`2FE3:0008`), with compound-packet busy retry safety and neutral release on link loss or stop, before enabling CORE keybinding writes or stock USB identity.

## Verified RF-to-USB Mapping

Cross-checked against live Windows wired HID layout inspection (`wired-hid-layout-20261126-183213-b93b7b4b.json`), existing local mouse dump (`0x1a7b8`, `0x14bb6..0x150b2`), and CORE static `ModelOSeries.js`:

| RF Type | Header | RF Payload | USB Interface | Report ID | USB Length | Usage / Collection | Neutral Release |
| --- | --- | ---: | --- | --- | ---: | --- | --- |
| 1 | `0x41` | 8 B | `HID_0` (Mouse) | None | 6 B | `0001:0002` Mouse | Yes (buttons=0) |
| 2 | `0x42` | 8 B | `HID_1` (Aux) | `0x03` | 9 B in / 2 B out | `0001:0006` Keyboard | Yes (all-zero) |
| 3 | `0x13` | 2 B | `HID_1` (Aux) | `0x01` | 3 B in | `000C:0001` Consumer | Yes (all-zero) |
| 4 | `0x0C` | 1 B | `HID_1` (Aux) | `0x02` | 2 B in | `0001:0080` System Control | Yes (zero) |
| 5 | `0x45` | 8 B | `HID_1` (Aux) | `0x04` | 9 B in | `FFA0:0001` Vendor Notify | No (stateless) |
| 6 | `0x46` | 8 B | `HID_1` (Aux) | `0x05` | 9 B in | `FFFF:0001` Vendor Notify | No (stateless) |

## Architecture & Guards

1. **Opt-in isolation**: Guarded behind `AUX_HID_FORWARDING` (`-App aux-hid`, product `DIY Mouse Aux HID v26`, `CONFIG_USB_HID_DEVICE_COUNT=2`, local `2FE3:0008`). Existing `dongle`, `receiver`, `latency`, `config-read`, and `config-hid` builds remain untouched.
2. **Independent standard HID descriptor**: `firmware/radio_logger/src/aux_hid.h` defines the 5-collection auxiliary descriptor (Consumer ID 1, System ID 2, Keyboard ID 3 + 1-byte LED Output, Vendor `FFA0:0001` ID 4, Vendor `FFFF:0001` ID 5) and accepts host Keyboard LED `SET_REPORT` (`wValue == 0x0203`).
3. **Compound-packet parsing & deduplication**: Validate every record `(type, len, cont)` strictly up to `p[0]` (max 8 records per packet). Track a per-sequence `forwarded_mask` bitmask so if a later record in a compound packet hits USB busy (`-EAGAIN`), ACK is withheld and retransmissions retry only the remaining records without duplicating already-forwarded mouse/key/vendor reports.
4. **Neutral release**: Track active non-zero Keyboard, Consumer, and System Control state; `aux_hid_finish()` sends bounded all-zero release reports for any active collection on link gap, explicit `x` stop, or error. Fail closed if neutral release fails.
5. **Verification**: Native C unit tests (`firmware/test_aux_hid.c`, `firmware/test_aux_hid.py`) cover descriptor validation, single and compound RF records, busy-retry without duplication, LED `SET_REPORT`, and neutral release. Hardware verification uses a separate read-only Windows enumeration + wireless movement/notification check on prototype IDs, then restores exact tested normal v14 (`93eb987a`).
