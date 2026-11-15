Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-usb-reset-restart.md.
Inspect git status/log first. Branch:feature/receiver-usb-reset-restart.
Paused ahead of02:30 AM Manila on November16,2026;this continuation resumes.

PAUSED BEFORE02:30 / V11 POST-RECOVERY ARM PASS 2026-11-16:
User's02:30 Manila work window is ending;pause after completed test. No further
hardware actions until user resumes. Branch feature/receiver-usb-reset-restart.
User confirms all controls smooth and held-left release after NEW explicit arm
following receiver-only Windows USB restart. Configured-passive restart PASS;
active USB-loss/held-button behavior across reset is NOT yet tested.
Private base build-pairing-analysis/receiver-v11-rearm-20261116-022118:
100s hidden raw1975 receiver-only background records,0 read errors;left down/up
15/15,wheel45. Right click user-confirmed but no independent right transition
in this short capture. Link21.144s,x46.166s,raw up46.176s,neutral/disarmed chunk
46.199s;host timing is not physical latency. One arm/discovery,24544 ACKs,
8065 HID submissions,48 retries,busy75,no ERROR;one disarmed/reboot response.
Board v11 NOW DISARMED/LATCHED,one arm/discovery,24544 ACKs used. COM12 closed,
no live serial/raw/build/admin helper/UAC session. User told release/reconnect USB.
Installed v11 SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and private
receiver-v11-b1d27489.uf2. V10/v9/v8 rollback images preserved. Bounds remain
327680 ACK/5min,8 discoveries,1s gap,30s initial discovery,one p/boot,x stop.
NEXT WHEN USER RESUMES: prepare ACTIVE USB-reset test,including old-session
abort,no automatic resume,fresh explicit arm and held-button behavior. Review
UAC sequencing first:approval must occur before held-input observation,otherwise
consent clicks confound the test. Bounded deferred receiver-only admin helper
may be needed;NOT implemented. Existing helper restarts immediately. Exact
receiver root privately identified;never replace drivers/target other devices.
Configured-passive PnP restart returned one fresh unarmed boot/no loop;MCU reset
cause not separately instrumented. Active-reset release/startup neutral remains
unverified. Do not claim complete USB recovery or daily-use/gaming reliability.
Current used boot needs explicit normal-reboot readiness before another arm.
No repeated v11 build/flash/passive baseline without a new reason.

V11 policy:user chose passive recovery with explicit rearm. Configured USB
RESET/DISCONNECTED latches HID off and requests COLD reboot,serviced outside
callback in DTR wait/main fatal receive path. Fresh boot unarmed. Initial
enumeration reset and generic USB_ERROR do not automatically reboot;generic
error stays fatal. All8 native checks,receiver/probe/radio builds and UF2/source/
config checks passed. Receiver CONFIG_REBOOT=y;61812 flash/16128 RAM,242 UF2
blocks0x26000..0x351FF. No firmware changes since v11 image validated. Probe/
radio hashes changed with shared edits;not byte-identical controls. Default HID
existing image unchanged,not rebuilt. V10 had passed full5-minute smooth controls,
two mouse-only recoveries and clean original deadline. V9 x held-release and
V8 automatic held-release passed. Preserve these baselines.

Private files:restart-receiver-usb-admin.ps1 (immediate fixed receiver-only
PnP restart,UAC needed),receiver-usb-restart-target.json,
receiver-v11-usbrestart-20261116-021713.* (helper/pnputil exit0,new passive boot),
capture-v11-rearm.ps1. Flash metadata armed=true;never clear without new boot/
readiness evidence. No deferred active-test helper exists yet. No admin helper
or timer task remains running.

Use question pop-ups for physical readiness;confirm actual new arm BEFORE mouse
power-on. One serial reader,DTR,drain2s,one p per boot. No automatic rearm or
unbounded operation. Raw logger max180s;foreground requires window activation,
input_code0 foreground/1 background. Background raw can be Windows-throttled/
coalesced;not polling/loss proof. Host timestamps not physical latency/DPI.
CORE wired selected1600 saved,wireless equivalence unverified;Windows accel1/
speed9 unchanged,receiver X/Y1:1. No guessed scaling. Keep PROJECT_CONTEXT.md
AND RESUME_PROMPT.md current,commit/push tested milestones on feature branches.
Private captures/addresses/binaries ignored under build-pairing-analysis;leave
.vscode/ and INFO_UF2.TXT untracked. Respect future user time boundaries.
