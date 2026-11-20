# CORE startup coverage and configuration preservation

**Goal:** Qualify the command set required by the installed CORE model before enabling its USB identity, while preserving existing mouse settings.

`research/trace_core_startup.js` executes the already extracted ModelOSeries source
with mock HID and DB only. Factory Series One PRO defaults produce42 SETs and29
GETs; actual saved user profiles may differ. No app launch, native addon, hardware
response or user DB write occurs in this trace. The installed package version is
1.1.36; absent BuiltType defaults to0 in its env source.

| Target / group / operation | Payload bytes | Factory SET count | Meaning |
| --- | ---: | ---: | --- |
| 02 / 03 / 00 | 10 | 24 | Eight bindings in each of three profiles |
| 02 / 01 / 01 | 18 | 3 | Four X/Y DPI stages per profile |
| 02 / 02 / 01 | 19 | 3 | Six stored DPI-indicator RGB entries per profile |
| 02 / 01 / 08 | 1 | 3 | Lift-off setting, legacy model global field |
| 02 / 01 / 00 | 1 | 3 | Polling divider, global field |
| 02 / 00 / 08 | 2 | 3 | Profile debounce |
| 02 / 00 / 05 | 1 | 1 | Active profile selection |
| 02 / 01 / 02 | 2 | 1 | Active DPI-stage selection |
| 00 / 00 / 81 | 3 | 1 | Receiver-local firmware version read |

General LED effect/brightness calls are skipped for LEDSetting=false. A separate
DPI-colour command remains. Mouse battery polling is outside this initializer
trace. Macro-body editing is outside the factory startup trace and current scope.

## Before any CORE connection

1. Prepare a read-only wired backup of startup-touched fields: globals plus three
   profiles' DPI stages/selection, indicator colours, debounce and eight slot0
   binding records each. Preserve opaque seven-byte binding data, not just names.
2. Run `research/backup_mouse_profiles.ps1 -SelfTest`, then ask fresh wired/CORE-
   closed readiness. The helper permits only fixed getter opcodes on258A:2018,
   expects40 complete responses and saves a private pointer only after success.
   It does not read RF addresses or macro bodies or write settings. Do not call
   this an independently tested full-device restore; restoration tooling must
   validate fields before replaying known setters.
3. Expand canonical command validation and add a truthful receiver-local version
   response before changing identity. Reject updater/pairing/address/reset and
   unqualified operations; never report fabricated write success.
4. Keyboard/consumer/system and vendor-notification forwarding must work before
   testing key remapping. Static source suggests RF types2/3/4 use8/2/1 payload
   bytes; types5/6 use8-byte vendor payloads. Verify actual mapping and neutral
   release, including HID-busy retries, before enabling CORE profile writes.
5. Use a separate prototype candidate and backup-aware reversible tests first.
   Keep exact normalv14 rollback. Only after required startup commands and input
   output paths qualify should CORE identity/UI tests begin.

User priority remains CORE configuration before the four-hour reliability trial.
Normal v14 stays installed during this read-only software/backup phase.

## Wireless backup fallback

After two fresh wired readiness/reconnection attempts, Windows reports no258A
device, bootloader or failedUSB device. No stream/query was opened. Use the
proven receiver Feature/RF path instead of blocking on the cable.

Extend only getter coverage to profiles1..3, DPI colours, debounce, lift-off and
the eight known slot0 binding IDs. Keep selected-stage writer restricted as in
v23; the backup helper sends getters only. Add a distinct v24 product and strict
`backup_mouse_profiles.ps1 -Wireless` identity gate; preserve wired default.
Test all40 request shapes, invalid profiles/buttons/slots/padding and no arbitrary
write expansion. Build/freeze/validate, then fresh receiver-flash readiness and
stationary wireless mouse readiness. Restore exact normalv14 after the read-only
backup batch. No new mouse settings write while wired recovery is unavailable.
