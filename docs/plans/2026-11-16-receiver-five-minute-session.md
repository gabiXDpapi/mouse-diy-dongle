# Bounded Five-Minute Receiver Experiment

**Goal:** Extend the hardware-validated v9 receiver to one finite five-minute session, retaining the explicit stop prerequisite.

**Scope:** Receiver-only constants in `discovery_probe.h`, v10 banner, dependent native bounds/deadline checks, and documentation. No new protocol, scaling, hopping, persistent settings or dependency. Non-HID probe remains 256 ACKs/two seconds. Limits become 327,680 shared ACK attempts/300,000ms, eight discoveries, one-second gap and 30-second initial discovery. Lowercase `x` still latches off until reboot; neutral/error handling is unchanged.

1. Add a runtime requirement assertion for the new receiver duration/cap; run focused native test to fail on v9.
2. Change the two receiver constants/banner and update dependent static/deadline assertions. Run all eight tests, retaining expiry/recovery/stop/failure checks against the original shared session deadline.
3. Build receiver and validate UF2, staged source/config/banner, preserve v9/v10 images. Verify existing probe/radio/default HID control hashes; no rebuild needed for unchanged non-HID paths (only receiver constants/banner change).
4. Update/push software checkpoint on `feature/receiver-five-minute-session`; pause for verified physical flash, then passive startup and separate input readiness.
5. Later trial: full 335-second serial capture (30-second discovery +300-second session +cleanup), optionally first180 seconds of background Raw Input for controls only. User exercises harmless movement/click/wheel and mouse-only OFF/ON at about one and three minutes. Stop early with x on reported trouble. Record final neutral stop and user observations;do not treat this as hours-long reliability or latency validation.

**Time boundary:** Stop/save by02:00 AM Manila2026-11-16. No five-minute arm after01:52;defer hardware work rather than overrun. Physical readiness must be explicit. Private captures/images ignored. Existing authorization is to continue until physical intervention and within this deadline;limits are explicitly communicated,not silently enabled.

Software checkpoint: bounds regression failed on v9;all8 checks pass on v10.
Receiver build/UF2/source/config/banner validation pass;61560 flash/16128 RAM,
241 blocks,hash32713b13... . Non-HID paths unchanged;existing control hashes
verified without rebuilding. V9 and v8 preserved. Physical flash readiness next.
