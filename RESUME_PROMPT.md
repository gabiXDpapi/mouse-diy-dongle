Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-physical-usb-loss.md.
Inspect git status/log first. Branch:feature/physical-usb-loss-validation.
Paused ahead of17:10 Manila break on November16,2026;this continuation resumes.

WIRED USB-LOSS CONTROL MATCHED / PAUSED AHEAD17:10 BREAK 2026-11-16:
User confirms wired Glorious also kept selecting after physical USB unplug with
LEFT held;cleared afterward,other pointer normal. DIY receiver unplugged throughout.
Private base wired-usbloss-20261116-165945-b57d350d:60s read-only capture1458
wired-only raw records/errors0,one LEFTdown/no LEFTup. Host LEFTFalse initially,
True~24.527s,False~53.924s after cleanup;one later click transition. No RF/serial/
PnP/UAC,input injection or Windows setting change. All processes closed.
INTERPRETATION:physical receiver-removal behavior MATCHES wired reference on this
PC/app. The expected automatic release on abrupt unplug was not established by
baseline;do not call it a DIY-specific physical-removal bug. This is a scoped
local observation,not universal Windows behavior. Wired PnP restart was NOT
compared,so don't generalize that exact scenario. V12 startup-neutral did not
change the observed PnP held state;no demonstrated benefit for that symptom.
Known connected x/timeout neutral-release checks still passed;keep those guards.
Pause now ahead of user17:10 break;resume ONLY on user request. No more firmware
changes/live trials this session. Current branch feature/physical-usb-loss-validation.
Installed/source/staged image remains v12 SHA256
1e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30;receiver physically
unplugged. Glorious USB may be reconnected for ordinary wired use;not independently
checked after instruction. No live serial/raw/helper/build/UAC sessions.
NEXT WHEN RESUMED:consolidate findings and choose smallest stable baseline before
longer normal-use testing. Do not add more firmware release guesses for physical
unplug behavior. Review whether to drop v12 startup-neutral experiment (no benefit
shown for target symptom) in favor of preserved v11;no rollback/build/flash done.
A planned host restart can be made graceful by x+confirmed neutral before PnP,
but that is not unexpected-loss release and not newly hardware-tested. Keep the
remaining PnP-specific limitation scoped. Then plan finite longer-use validation
with explicit stop/time/reconnect limits and fresh physical readiness. No unlimited
mode or daily-use/gaming latency certification. Refresh user deadline after break;
old17:06/17:10 UTC guards must not be blindly reused in a later session.

Preserved images:receiver-v12-1e543890.uf2,v11-b1d27489,v10-32713b13 and older
under ignored build-pairing-analysis. SDK image/source stillv12 (no rollback).
V12 is v11 plus one completed all-zero startup HID report/pending-forward gate;
no synthetic press. Native8 checks/receiver-probe-radio builds/source/config/
UF2 validation passed. V11 configured-passive host restart and fresh explicit
arm/x-held-release passed. V10 full5min/two mouse-only recoveries passed;V8
held-left timeout passed. Current limits327680 ACK/5min,8 discoveries,1s gap,
30s initial discovery,one p/boot,x stop. USB reset recovery must stay passive
until explicit new arm (user policy);no automatic session continuation.

Physical DIY removal capture v12-physicalusb-20261116-165002-295ec075 had2050
raw/error0,leftdown2/up1,host held through serialloss;user selection persisted.
V12 PnP capture v12-activeusb-20261116-162436-bb444906 had2353 raw/error0,
hostLEFTTrue before/after reset despite neutral marker. User cleared states with
Esc/releases/receiver unplug. Wired reference now reproduces physical-unplug
symptom;don't repeat these tests without a new hypothesis. Host ClickLock was
read disabled;Windows accel1/speed9 unchanged,CORE wired1600 saved,wireless
DPI equivalence unverified,X/Y1:1. Raw/background timestamps are not physical
latency/DPI/polling/loss proof. No keyboard logging/input injection/driver replacement.

Host scripts:research/capture_wired_usb_loss.ps1(-SelfTest,or60s capture with
explicit FinishBeforeUtc),capture_physical_usb_loss.ps1(-SelfTest,or one-arm
physical test with ArmBeforeUtc),capture_active_usb_reset.ps1(Prepare/Capture/
SelfTest) and deferred receiver-only restart helper(UAC,180s expiry/75s arm gate).
All job/capture/target IDs ignored in build-pairing-analysis;old jobs consumed/
finished. No helper currently running. Use question pop-ups for physical
readiness;actual fresh arm confirmation BEFORE mouse ON. Never blindly reset
metadata or reuse a used boot/job. Keep both handoff files current,push tested
milestones on feature branches;leave .vscode/ and INFO_UF2.TXT untracked. No
repeated extraction/build/flash/baseline without reason. Respect new time limits.
