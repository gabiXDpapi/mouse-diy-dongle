# Startup Neutral HID Experiment

**Problem:** v11 active PnP restart returned to unarmed reception but the user observed a stuck LEFT selection. Raw input had a final LEFT-down without LEFT-up. Esc/releasing buttons/physical receiver unplug cleared it;Windows ClickLock was disabled. Do not treat transport recovery as full release success.

**Hypothesis:** Explicitly reporting all buttons released after new USB configuration may clear retained host state. It is not established that Windows will generate a release transition from an initially zero report after PnP restart. Hardware repetition is required;do not synthesize a pressed-button pulse.

**Smallest change:** Receiver v12 marks startup-neutral pending on first successful USB configuration. A main-thread service sends one six-byte all-zero report and waits for endpoint completion before DTR/arming can progress. RF forwarding returns EAGAIN while pending. Reuse the existing neutral writer/completion path;keep ordinary link-end no-op behavior when buttons are already released. Duplicate CONFIGURED notification must not repeatedly send zeros. Errors keep pending/output blocked;existing USB restart handling remains. Limits/mapping/protocol/quiet behavior unchanged.

**Files:** `wireless_hid.h`, `main.c`, actual native HID harness, README/research/context/resume. No host setting or driver change.

1. Failing native assertion:configured receiver cannot forward data before startup-neutral completion. Run on v11 to demonstrate failure.
2. Implement pending/service/reused neutral helper. Test unconfigured state,EAGAIN/I/O/completion timeout,exact six zero bytes,no motion/click/counter/sequence consumption,idempotent service/duplicate config,and prior bridge/stop/reset regressions.
3. All8 checks;receiver/probe/radio builds,UF2/source/config/banner validation. Preserve v11 and record candidate hash/footprint;default HID remains untouched.
4. Push software checkpoint. Receiver is physically unplugged after host-state cleanup. Pause for reconnect+NICENANO readiness,verify/flash/passive startup. Repeat active USB-loss experiment only after separate UAC and physical readiness. No hardware fix claim until user-held release succeeds;physical no-resume result must remain separate.

User's continuation authorizes preparation until physical intervention. Current host is cleared,receiver unplugged,no live capture/admin/serial sessions. Keep private artifacts ignored and handoff current.

## Software result

Runtime forwarding-gate regression failed on v11. All8 checks pass with v12;
receiver/probe/radio builds/UF2/source/config/banner checks pass. Footprint
61988 flash/16128 RAM,243 UF2 blocks,hash1e543890... . V11 preserved. Host capture
now requires v12 and startup-neutral completion,also samples host LEFT state
read-only. Selfchecks/syntax pass. No flash/hardware result. Receiver remains
physically unplugged and host state cleared;physical bootloader readiness next.
