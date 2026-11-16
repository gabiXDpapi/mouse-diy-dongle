# Physical USB Removal Diagnostic

V11 and v12 both left Windows LEFT held after a targeted PnP restart. V12's startup-zero report completed but did not clear that case. Stop firmware guesses and compare physical removal on unchanged installed v12.

**Scope:** One host capture script plus documentation. No build/flash/UAC/PnP command/host setting change. Keep private output ignored.

1. Add a bounded physical-removal capture using the existing native mouse-only logger and one serial reader. Require fresh v12 startup/neutral marker and explicit mouse OFF/reboot readiness,one p and actual confirmation. User moves briefly,holds LEFT and physically unplugs only the receiver,then tests the other pointer without clicks. Keep receiver unplugged until observation ends.
2. Record serial-loss timing and read-only global LEFT state once per second. Serial loss alone is not proof of physical unplug;get user confirmation. If user cannot unplug,cancels,or no link starts,send x once while port is available (60s after link/35s without link). Never rearm/reopen after removal. Raw120s,controller130s bound.
3. Self-check the actual disconnect/cancel/timeout action gate. Require a supplied UTC arm cutoff;check before hardware and immediately before p. Current user break17:10 Manila,arm cutoff17:06 (2026-11-16T09:06Z). Validate syntax/firmware diff0,push then pause for physical readiness.
4. Physical popup:receiver normal replug(no double-reset),Glorious OFF/unplugged,selectable text/other pointer,receiver cable reachable while holding mouse LEFT. After arm cue,hold LEFT and unplug receiver around15s after ON. Observe selection while held,then release. If stuck,Esc/release/click other pointer;receiver already unplugged. Separate host/physical observations from raw transition counts.

Capture does not claim a release fix or unexpected USB-reset recovery. A successful physical-removal result only distinguishes it from the failed PnP-restart scenario. Save/close/push and pause by17:10.

Software checkpoint:actual action gate and expired-cutoff entry refusal PASS;
syntax/diff checks PASS,firmware diff0. No hardware/serial/UAC action. Capture
ready;physical setup/one-arm observation next before17:06 cutoff.

## Physical result and wired reference

User confirmed physical receiver unplug with LEFT held;selection followed other
pointer until cleanup. Raw2050/errors0,left down2/up1,host LEFT remainedTrue
after port loss. No x/PnP/UAC. This is not specific to PnP. Before further firmware
changes,run same held-unplug procedure on Glorious wired USB with receiver off.
New capture_wired_usb_loss.ps1 provides60s raw/global-state capture,no serial or
mutating APIs;1s lifecycle smoke/deadline refusal PASS. Requires finish deadline
with60s wrap margin. Wired hardware observation pending;pause by17:10.

## Wired result and interpretation

User confirms stock wired Glorious also retained selection after physical USB
unplug while LEFT held;cleared afterward.60s capture1458 wired-only records,
no read errors,LEFTdown without up,host state held until cleanup. Thus abrupt
physical-unplug expectation was not met by wired baseline either;this is not
evidence of a DIY-specific physical-removal regression. Do not generalize to
all systems or wired PnP restart (untested). No further firmware guess justified
for this symptom. Pause ahead17:10;later consolidate stable baseline/consider
dropping v12's unhelpful startup-neutral experiment before longer testing.
