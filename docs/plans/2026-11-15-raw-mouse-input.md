# Raw Mouse Input Diagnostic Plan

**Goal:** Compare reports arriving at Windows from the wired mouse and DIY receiver without modifying firmware,DPI or pointer settings.

**Architecture:** Small native Windows message-only-window logger using RegisterRawInputDevices/GetRawInputData. Register mouse usage only with INPUTSINK(no NOLEGACY),filter known wired258A:2018 and receiver2FE3:0008 device paths,and save source label,monotonic host-arrival time,X/Y,buttons and flags to exclusive-create private CSV. Never register keyboard or inject input. Bound run to1..180s;timer and per-message deadline stop it. Buffered file output,cleanup registration/window/file on exit. Use existing MinGW and Windows libraries,no external dependency.

**Files:** `research/capture_raw_mouse.c`,README/context/research. Binaries/CSVs stay in ignored `build-pairing-analysis`.

1. Implement actual filter/record-validation self-checks for known/unknown devices,truncated/wrong-type packets,signed motion extremes and preserved flags. Compile with warnings-as-errors;run `--self-test` without input registration.
2. Validate hidden1s start/stop capture and exclusive-create protection;this is not performance evidence.
3. Save build/validation metadata and push software checkpoint on `feature/raw-mouse-input-diagnostics`.
4. Pause for wired readiness,then capture controlled motion. Later pair receiver Raw Input with the existing single-arm serial capture after normal reboot/readiness. Keep v7 and saved1600 profile unchanged.

Host-arrival timestamps may reflect Windows scheduling/coalescing. These logs do not alone measure physical-event latency,sensor DPI or RF packet loss. Source labels omit full device paths;other mice/keyboard events are not written. Preserve normal pointer operation.

**Software checkpoint2026-11-16:** GNU11 compile/warnings-as-errors,self-checks,
1s timed start/stop and exclusive-create refusal PASS. Smoke had0 input records,
not performance evidence. Final binary hash41a46ce5...,metadata in ignored
raw-input-software-validation.json. Legacy headers need documented Vista
notification constants;timer uses wrap-safe GetTickCount for bounded durations.
No firmware changes or controlled raw motion capture. Next wired readiness
and45s capture with20-30s deliberate motion. Maintain RESUME_PROMPT.md for handoff.
