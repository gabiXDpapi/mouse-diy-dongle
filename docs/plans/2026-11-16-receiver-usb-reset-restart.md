# Passive USB Reset Recovery Plan

**Goal:** Recover from configured USB reset/disconnect without requiring a physical receiver reboot, while remaining unarmed until a new explicit command.

**User policy:** Stay passive after USB recovery; explicit rearm required. No automatic continuation of the old RF session.

**Smallest design:** On reset/disconnect after configuration, retain the existing online/failure latch and additionally request a software reboot. The application services that request outside the USB callback, both while waiting for initial DTR and in the existing fatal receive path. Hardware/software reboot clears endpoint/DMA/session state and returns to the known passive startup. Initial enumeration reset is not a reboot request; USB errors remain fatal without automatic restart. Keep five-minute/327680-ACK/eight-discovery/one-second-gap bounds, quiet logging and x stop unchanged. No new session-state machine or repeated-arm command.

**Files:** `wireless_hid.h`, `main.c`, receiver-only `receiver.conf`, actual HID bridge native harness, README/research/context/resume prompt. Use Zephyr `sys_reboot(SYS_REBOOT_COLD)` with `CONFIG_REBOOT=y`. No dependency or host-driver replacement.

1. Add a failing native expectation that configured disconnect requests the mocked reboot service, without allowing HID forwarding. Verify failure on v10 before implementation.
2. Add request flag/service and call sites. Cover initial reset, configured reset/disconnect, quick reconfiguration still blocked, offline forwarding/neutral behavior, unchanged generic USB error behavior and no report submission by restart service. Reboot service must not run inside the USB callback.
3. All8 native tests;receiver/probe/radio builds and UF2/source/config/banner checks. Preserve v10. Record hashes honestly;shared assertion locations may change non-HID images despite unchanged behavior.
4. Push software checkpoint. Physical flash/passive readiness,then a targeted receiver-only host USB restart check. Determine Windows restart permissions first;do not replace drivers or touch other devices. USB power unplug/replug alone cannot establish software-reset recovery because it resets the MCU independently.
5. Hardware evidence must show passive re-enumeration and no automatic pointer restart;separate explicit-arm readiness before checking controls. Do not claim restart recovery until observed. Pause by02:30 Manila;no short armed trial after02:26 or five-minute trial after02:22.

Local Zephyr4.2 sources reviewed:USB core cancels transfers/disables interface endpoints on reset/disconnect;HID configuration is cleared;application callback follows class callbacks. Full software reboot avoids reusing a stale semaphore/DMA buffer across USB generations. Host button release during a lost USB connection still needs hardware observation.

Software checkpoint:all8 checks and receiver/probe/radio builds/UF2 pass. New
service was missing in red regression;tests cover deferred reboot and no old
output after quick configuration. V11 hashb1d27489...,61812 flash/16128 RAM,
242 UF2 blocks. Source/config/banner match. Hardware still v10;flash/passive
readiness next. Windows restart needs UAC;exact receiver root identified and
fixed-purpose admin helper prepared but not executed. Stop by02:30.

## Hardware checkpoint and pause

Flash/passive startup PASS. Exact receiver-only Windows PnP restart via UAC
returned exit0 and one fresh unarmed v11 startup,no loop/errors. Fresh explicit
arm afterward:user confirms smooth controls and held-left release on x. Raw
release/neutral/disarmed observed,no read errors. Active USB loss is NOT tested.
Next prepare UAC sequencing before held-input observation (a bounded deferred
helper may be needed,not implemented). Paused ahead of02:30,all sessions closed.
