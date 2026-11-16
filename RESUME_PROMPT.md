Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-physical-usb-loss.md.
Inspect git status/log first. Branch:feature/physical-usb-loss-validation.

WIRED USB-LOSS REFERENCE PREPARED 2026-11-16: physical receiver unplug
also retained LEFT per user;host cleared,other pointer normal. No firmware
changes. Added research/capture_wired_usb_loss.ps1:60s native raw+read-only host
LEFT state,NO serial/RF/PnP/UAC. One-second wrapper/native lifecycle selfcheck
PASS(records0/errors0,not movement evidence);expired break deadline refusal
PASS before capture. Script requires FinishBeforeUtc and60s wrap margin.
NEXT physical readiness:leave DIY receiver UNPLUGGED,connect Glorious by USB in
WIRED mode,same selectable text/other pointer,mouse cable reachable while LEFT
held. No arming. Start60s capture;user holds LEFT/drag then physically unplugs
Glorious USB,waits5s and tests other pointer without clicks while stillheld.
Then release/clear with Esc/click other pointer ifneeded. This reference tests
same Windows/app behavior without DIY radio/firmware. Record outcome before
attributing held state to dongle. Do not assume generic Windows behavior yet.
Use -FinishBeforeUtc 2026-11-16T09:10:00Z for17:10 Manila break;controller refuses
if60s capture+60s wrap won't fit. No new armed trial after17:06;wired hasnoarm.
All capture/serial/helper/build sessions currently closed;only1s software smoke
ran. Current installed v12 remains unplugged. Save/push before break.

Recent negative results:V11 active PnP restart left LEFT held;V12 startup-zero
report completed but same PnP issue persisted. V12 physical receiver unplug also
left selection following other pointer;cleared afterward. Physical job
v12-physicalusb-20261116-165002-295ec075.* :one p/link,no x,serial loss observed,
raw2050/errors0,LEFTdown2/up1,host LEFTTrue across removal until cleanup.
Do not call these release PASS;transport capture success is a separate field.
V12 startup-neutral hypothesis failed for PnP;don't add guessed firmware changes
or synthetic press pulses. Compare wired reference first. No host setting/input
injection change. ClickLock was read disabled;CORE wired1600 saved,Windows
accel1/speed9 unchanged,X/Y1:1. Host raw timestamps/background cadence not physical
latency/DPI/loss/polling proof.

Installed v12 SHA2561e543890474b81006de813ed4347fe171e26ca131ac3c02a0a5d517a1f75cf30;
C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2 and ignored
receiver-v12-1e543890.uf2. V11/v10/v9/v8 preserved. Receiver currently unplugged.
V12 tests/build/source/config passed;no new build/flash for these comparisons.
Bounds327680 ACK/5min,8 discoveries,1s gap,30s first arm,one p/boot,x stop.
V10 full5min/two mouse-only recoveries passed,V9 x-release/V8 expiry-release passed.
No automatic rearm afterUSBreset. Current wired reference has no p/serial at all.

Use question pop-ups for physical readiness/pause. Keep PROJECT_CONTEXT.md AND
RESUME_PROMPT.md current,push tested milestones on feature branches. Private
captures/addresses/binaries ignored under build-pairing-analysis;leave .vscode/
and INFO_UF2.TXT untracked. Respect17:10 break;later resume requires fresh time
review,not blindly using these old UTC cutoffs. No daily-use/gaming certification.
