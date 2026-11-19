# USB-to-radio configuration read staging

**Goal:** Validate the complete host Feature-report -> receiver -> mouse RF -> host reply path before enabling CORE detection or settings writes.

**Architecture:** A second HID instance provides vendor usage FFFF/0000 and a64-byte Feature report. Retain CDC, the prototype USB IDs and the proven v20 RF codec; suppress pointer forwarding. Only the exact verified version-read body is accepted, once per boot. USB callbacks enqueue a flag and snapshot replies; radio work stays in main. CORE's VID/PID matcher cannot attach to this prototype identity.

**Stack:** Existing Zephyr4.2 legacy HID callbacks, fixed buffers and IRQ-locked mailbox; bundled HidSharp for Windows host checks. No new dependencies.

## Scope and regression surface

- Add `config_hid.h` callbacks/mailbox, `config-hid.conf`, opt-in build flag/App and native callback test.
- Share the fixed request literal from `config_read.h`; retain v20 automatic-read behavior when the new flag is off.
- Initialize HID_1 before USB enable; claim the USB request from main's command poll; inject ACK fragments only after claim. Publish complete/failure reply in main, never from an ISR.
- Existing normal and v20 paths remain gated. Preserve exact normal93eb987a and v20 images; no flashing or mouse settings writes during software work.
- USB control SET/GET must be Feature/ID0/exact64bytes. Reject arbitrary settings, updater packets, wrong type/size and repeated SET. GET while pending reports real busy A0; successful data comes only from the RF reply. Bound all buffers; do not return fabricated mouse firmware data.

## Checks and physical gate

1. Native tests first: setup validation, immutable allowlist, one-shot enqueue/claim, busy/publish/GET, USB-owned snapshot stability, failure response and HID initialization failure. Run existing codec/ACK regression checks.
2. Build separate v21 config-hid target, validate UF2/config, save exact hash privately. Verify default build flag remains off. Do not replace normal rollback with a software control build.
3. Prepare host read-only helper with offline tests and exact product/unique-feature gates. No CORE launch and no serial reader are needed to drive the query.
4. Pause via popup for physical flash, then separate stationary mouse readiness. Send one query, validate response against wired reference and restore exact normalv14 after the batch.

CORE identification, general settings writes, keyboard/consumer bindings and long soak remain later work. The user selected CORE compatibility before four-hour reliability testing.


## Software validation complete, November20

13 native tests pass, including full callback-to-codec path and actual host gate
in the probe harness. Host helper self-test passes. V21 image/config/UF2 and
staged-source hashes verified; frozen a7631b8d image remains private/unflashed.
Normal v14 restored/user-confirmed. Stop by05:00AM, no new flash at/after04:50.
Hardware USB Feature framing and end-to-end read remain unverified.
