# Foreground Raw Input Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Separate foreground mouse delivery from background delivery so background Windows throttling cannot be mistaken for receiver timing.

**Architecture:** Preserve the existing message-only logger by default. Add one `--foreground` option that shows a normal capture window; the user activates it. Append the WM_INPUT delivery code to each CSV row (0 foreground, 1 background), preserving device filtering, bounds, cleanup and exclusive-create output. Never change firmware, input settings or inject input.

**Tech Stack:** Existing native C/MinGW/user32/shell32 and assert-style self-test; no new dependencies.

## Steps

1. In `research/capture_raw_mouse.c`, change the existing signed-extreme serialization check to expect the new delivery field. Compile to an ignored candidate executable and run `--self-test`; expect the old formatter assertion to fail.
2. Pass `GET_RAWINPUT_CODE_WPARAM(w)` through `write_mouse`, append `input_code` to CSV, and check both foreground and background rows in the self-test. Add optional final `--foreground`; reject other argument shapes. For that mode create a visible normal window with simple instructions to click it and keep it active. Do not force focus. Existing default remains hidden.
3. Compile with `gcc -std=gnu11 -Wall -Wextra -Werror -O2 research/capture_raw_mouse.c -o build-pairing-analysis/capture_raw_mouse.exe -luser32 -lshell32`. Run `--self-test`, a bounded hidden 1s smoke, invalid-option refusal, and a visible 1s lifecycle smoke. Verify output and normal closure, not human timing or rate. User-visible mode is authorized by this diagnostic task.
4. Update README, RADIO_ANALYSIS, PROJECT_CONTEXT and RESUME_PROMPT with the completed background pair, the user's random overshoot/clean stop, and the pending foreground comparison. Push software checkpoint on `feature/foreground-raw-input`.
5. Pause via popup for wired readiness. Capture foreground motion only after user activation; analyze only `input_code=0`. A later wireless comparison needs normal receiver reboot, OFF/unplugged readiness and confirmed one-shot arming. Preserve background samples; do not repeat them or claim packet loss/polling/DPI from background counts.

Existing user authorization is to continue until physical intervention; execute in this feature branch without a separate execution-choice pause or additional agents.

## Software result

Regression failed as expected on the old formatter. New foreground/background
serialization and existing filtering/bounds/extremes checks pass. Build passes
warnings-as-errors. Hidden and visible 1s lifecycle exit0;visible completion
marker verified after explicit stdout flushing. Invalid mode refused without
file creation. No controlled foreground movement yet;physical readiness next.
