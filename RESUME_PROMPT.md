Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
Read PROJECT_CONTEXT.md(LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md,docs/plans/2026-11-16-receiver-usb-reset-restart.md.
Inspect git status/log first. Branch:feature/receiver-usb-reset-restart.

V11 PASSIVE USB RESTART SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-usb-reset-restart. V11 BUILT/VALIDATED,NOT FLASHED.
User policy:stay passive after USB reconnect,explicit arm required. Configured
RESET/DISCONNECTED requests COLD software reboot via main-thread service;callback
only latches off. Initial enumeration reset doesn't restart;USB_ERROR remains
fatal. Service runs in DTR wait and fatal receive path;new boot uses existing
unarmed/one-arm rule.5min/327680 ACK/8 discoveries/1s gap/x stop unchanged.
New missing-service regression failed compilation on v10;all8 native checks PASS
including reset/disconnect/error split,quick-config output block and deferred
restart. Receiver/probe/radio builds+UF2 PASS;receiver staged sources/config/
v11 banner verified,CONFIG_REBOOT=y. Probe/radio hashes changed with shared
edits,not byte-identical controls;default HID existing image unchanged.
V11 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256b1d27489bd8fa65b2972fa5fefe83c19b843428bb27492991cf66aa3e727561b.
61812 flash/16128 RAM,242 blocks0x26000..0x351FF. Private image
receiver-v11-b1d27489.uf2;v10/v9/v8 preserved. Metadata receiver-v11-build-validation.json.
Board STILL v10 expired/passive282670 ACKs/3 discoveries;COM12 closed,no sessions.
NEXT physical flash readiness:Glorious wired/OFF,receiver double-reset NICENANO,
verify identity/hash,copy then passive startup(no arm). Then receiver-only
Windows host restart test requires administrator/UAC because current tokennotadmin.
Exact USB root identified privately in receiver-usb-restart-target.json.
Ignored restart-receiver-usb-admin.ps1 prepared/syntax PASS,NOT EXECUTED;it
revalidates unique exact receiver root and runs pnputil /restart-device only on
that instance,writes private result. No driver replacement/other devices touched.
Begin with passive configured receiver;fresh v11 startup after host restart and
no arm/input required before further tests. Power unplug alone not proof. Active
USB-loss/held-button release remains separate hardware evidence if not tested.
Pause by02:30 Manila;no short armed trial after02:26 or5min after02:22.

V10 stable hardware milestone8977f4b:five-minute smooth movement/clicks/wheel,
two mouse-only OFF/ON recoveries,clean original300s stop,user confirmed. Full335s
serial,first180s background raw only. V9 held-left x-stop/smooth motion passed;
V8 held-left automatic timeout release passed. Do not repeat these baselines.
Current hardware image v10 SHA25632713b13dff337956808746b952bad1fa27d108079fa73ba52584c703ea4c2a9.
No automatic pointer session continuation allowed after USB recovery.

Use question pop-ups for physical readiness/pauses;actual arm confirmation BEFORE
mouse power-on. One reader,DTR,2s drain,one p/boot;used boot requires normal reboot.
Keep PROJECT_CONTEXT.md AND RESUME_PROMPT.md current,commit/push tested milestones
on feature branches. Private captures/addresses/binaries ignored under
build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked. No guessed scaling;
CORE wired1600 saved,wireless equivalence unverified;Windows accel1/speed9 unchanged.
Host raw timestamps not physical latency/DPI or RF-loss proof;background raw may
be Windows-throttled/coalesced. No final daily-use/1000Hz gaming certification.
