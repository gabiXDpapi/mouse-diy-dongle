Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read research/CORE_COMPATIBILITY.md and docs/plans/2026-11-19-glorious-core-compatibility.md.
Read PROJECT_CONTEXT.md, especially its newest LATEST HANDOFF / NEXT override,
README.md, research/RADIO_ANALYSIS.md and the wireless HID implementation plan.

Resumed November 19. Target before November 21: genuine DPI/lighting/keybind
customization through the receiver and 1 kHz polling. The app is NO LONGER
DEFERRED. Latest preference/question: can we reuse Glorious CORE? Investigate
that route before building a separate UI. CORE recognizes wired Series One PRO;
DIY v14 lacks compatible vendor configuration transport. Installed CORE app.asar
contains mouse protocol JS, native hiddevice source and device databases. Inspect
these existing assets privately; recover USB/RF settings commands rather than
fake DPI scaling or guessed writes. Lighting hardware support is unverified.

Normal tested v14 remains installed and working; no restoration needed. Exact
rollback: build-pairing-analysis/dongle-v14-93eb987a.uf2, SHA256
93eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052.
No active capture, serial, build, soak or timer. No new hardware changes made.
30-minute gameplay user-report passed with all controls. Verified 1 ms USB
interval and 964.9 host movement events/s are not fresh-sensor/physical latency
proof. Four-hour/multiday reliability remains unqualified; not started.

Keep context and this prompt current; commit/push tested milestones on feature
branches. Current branch feature/glorious-core-compatibility. Use popup pauses
for physical intervention. Keep captures, RF addresses and proprietary binaries
ignored. Leave unrelated .vscode/ and INFO_UF2.TXT untouched. Do not repeat
completed extraction, builds, flashing or baselines. Deadline is not a guarantee.

Latest progress: CORE supports stock Series One PRO dongle detection; LEDSetting
is false (general lighting disabled in installed app), DPI indicator colours
separate. Static firmware path for RF commands to USB settings mailbox found;
wireless sequence/replies still unvalidated. CORE initialization may WRITE
profiles, so do not expose a partial compatible receiver to it.
New research/read_mouse_settings.ps1 offline self-tests PASS, no hardware query
run. NEXT: popup for closing CORE and connecting Glorious by USB in WIRED mode,
receiver unchanged. Only then execute fixed read queries, saving observation in
ignored build-pairing-analysis. No speculative writes, bootloader or firmware
update commands. Four-hour trial explicitly deferred until this work.
