# Quiet Active Receiver Window Plan

**Goal:** Compare residual motion inconsistency with periodic/busy USB text removed from active RF processing.

**Architecture:** V8 keeps v7 idle sequence hold and all RF/USB/budget behavior. Periodic stats are eligible only outside an active link. Replace the active USB-busy print with a cumulative input-busy counter in the actual HID bridge. Final neutral-complete summary includes the counter. Keep discovery/first-ACK timing samples,link transitions and fatal errors.

**Scope:** `discovery_probe.h`, `wireless_hid.h`, `main.c`,actual native tests,README/context/research. No dependencies/config change. Preserve v7;branch `feature/receiver-quiet-active-window`.

1. Failing native assertion that active link suppresses periodic stats without changing session state;busy counter cases for no slot/write-EAGAIN vs I/O/short-write/neutral paths.
2. Add minimal active guard/counter increments and final-summary field;remove only per-busy text;mark v8 banner. Preserve code/timing/limits outside diagnostics.
3. All8 native tests;receiver/probe/radio builds and UF2/source/config/binary checks. Keep non-HID control artifacts/default HID unchanged where possible.
4. Update hashes/state and push checkpoint. Pause for physical verified flash/passive startup,separate motion readiness,one95s same-surface/DPI/settings comparison.

This is an A/B diagnostic,not a proven fix. Losing1s active stats reduces time-resolution;retain totals/limited timing samples and compare user consistency. If no benefit,next investigate motion payload/cadence instead of guessing gain/ACK constants.


## Execution checkpoint 2026-11-16

Foreground v7 reproduced overshoot;larger raw steps occasionally occur near
busy output chunks,correlation only. Native active-suppression regression failed
on v7 before patch;all8 checks pass after change,including counter paths for
slot/write EAGAIN versus I/O/short-write/neutral. Receiver/probe/radio builds and
UF2 validation pass. Probe/radio hashes remain unchanged;existing default HID
image unchanged without rebuild. V8 source/config/banner match and image is
preserved (hash5df75330...). No flash or passive startup yet. Physical bootloader
readiness is next;hardware still v7 expired/passive.
