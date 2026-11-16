# Active USB Loss Capture Plan

**Goal:** Test installed v11 during active held input, without a UAC consent click contaminating the observation. No firmware change, rebuild or flash.

**Files:** `research/restart_receiver_usb_deferred.ps1`, `research/capture_active_usb_reset.ps1`, README/context/research/resume prompt. All job files, device instance IDs and captures stay in ignored `build-pairing-analysis`.

1. Reuse the fixed receiver-only PnP restart logic. Add a deferred helper that obtains UAC first, publishes readiness and expires after 180 seconds. It restarts only on a unique job's explicit signal, rechecks the exact receiver root, and rejects cancellation/expiry before mutation. Keep the old immediate helper as a historical private capture artifact.
2. Add one controller with prepare/capture modes. Prepare creates an exclusive job and starts the elevated helper; capture requires at least 75 seconds of helper lifetime and an unused job. Physical normal-reboot/OFF readiness precedes capture. Require a fresh v11 startup, start a 150-second mouse-only raw logger, send one p and confirm the actual arm response. At link+25 seconds close the sole serial reader and signal the helper. Reopen the uniquely identified receiver CDC after restart and capture passive startup without a second p. Cancel the helper on exit;best-effort x if an armed attempt fails without verified recovery.
3. Self-check actual target selection, expiry/cancellation/signal gating and unused-job/lifetime validation with synthetic fixtures. Check script syntax. Do not run UAC, PnP restart, serial commands or native capture in self-test.
4. Update/push software checkpoint on a feature branch, then pause for UAC readiness with Glorious wired/OFF. After helper readiness, separate physical popup for normal receiver reboot, Glorious OFF/unplugged, selectable text and other pointer. Refuse to arm if helper expires;no automatic retry.
5. After confirmed arm, user moves briefly then holds LEFT through the targeted USB restart. Other pointer must leave selection fixed while LEFT remains physically held. After release, moving Glorious must stay inactive until a fresh explicit arm. Record Windows observations, raw events, reset command/result timestamps and fresh passive v11 startup. Active-loss button release is not assumed from the passive-only result.

No automatic session resume, repeated arm, driver replacement, other-device restart or new timing claim. Previous 02:30 cutoff belongs to the paused session;user's new continuation resumes work without a new cutoff. Update handoff after each meaningful step and pause whenever physical intervention is needed.

## Software checkpoint

Both scripts' self-checks and syntax/diff checks PASS. Actual CLI fixture confirms
existing-log refusal and preservation before hardware access. JSON date-object
versus string conversion issue caught by the serialized fixture and corrected.
Target validation includes argument character restrictions and exact healthy
instance matching. PnP process limited15s. Capture errors cancel helper and
attempt x if needed. Firmware unchanged;no builds/flash/serial/UAC/PnP run.
Next user intervention is UAC preparation,then separate prompt for fresh receiver
reboot/mouse OFF readiness before one active-reset capture.
