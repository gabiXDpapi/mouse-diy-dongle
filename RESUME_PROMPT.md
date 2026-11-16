Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md, the wireless HID implementation plan and
 docs/plans/2026-11-16-fifteen-minute-session.md. Inspect git status/log first.
Use feature-based branches; current feature/receiver-fifteen-minute-session.
Keep context and this copy-paste prompt current for every meaningful step.
Commit/push tested milestones. Keep private captures/addresses/binaries ignored.
Leave unrelated .vscode/ and INFO_UF2.TXT untracked.

V13 FIFTEEN-MINUTE TRIAL PASS / READY FOR BREAK — 2026-11-16 17:50 Manila:
User confirms all controls worked smoothly after BOTH mouse-only OFF3s/ON cycles,
receiver untouched, and clean original15min stop. Full935s serial AND Raw Input
completed exit0;92400 receiver-only records/read_errors0/failed0 (all background).
One p,zero x,three link starts/three neutral stops,no ERROR. Host-observed first
link09:34:06.773UTC -> final stop09:49:06.705UTC (~900s;host chunk timestamps are
not firmware-level timing). Final883050/983040 ACKs;658536 HID reports,438 retries,
47107 busy;idle=224074/status=2/type1=658974. LEFT381down/381up,RIGHT86/86,wheelUP135/
DOWN262. Raw coalescing means these counts do NOT certify1000Hz or physical loss.
Private base v13-session-20261116-173338-5a62c157 (.txt/.csv/.host.jsonl/.capture.json/
.raw.stdout.txt/.raw.stderr.txt/.summary.json),all ignored. No private addresses
or binaries committed. All live readers/logger/build/UAC/helper sessions CLOSED;
exec55451 completed. Installed v13 remains connected,session EXPIRED/neutral,
one-arm-per-boot budget used;do NOT rearm without normal reboot and fresh setup.
Mouse instructed reconnect USB;user report does not independently verify cable.
Image396d7dd820c39456e4c87e62e99a7e5ec2a71dc40f8c121aab971ef307ea4bfc.
Branch feature/receiver-fifteen-minute-session. Previous milestones fa9936c software,
5e1ca7e flash/passive startup pushed. Context/resume updated with full result.
No further15min arm fits current18:00 stop (cutoff17:42 already passed). Pause
hardware work here;user may use Glorious wired. NEXT on explicit resume: refresh
user deadline, review this result and plan repeated bounded normal-use validation
or measured gaming-performance work;do not rebuild/flash/repeat completed baseline
without reason. Preserve v13 as tested finite-session baseline. No unlimited mode.
Remaining: repeated longer reliability,passive USB restart regression for this exact
v13 image if needed,physical polling/latency/channel/IRQ work. V11 recovery,x stop
and held-timeout previously tested;v13 long session does not independently repeat
every safety case. Abrupt physical unplug held state matched wired reference on
this PC/app;wired PnP restart not compared. Do not revive startup-neutral guesses.
Current state above supersedes earlier LIVE/IN PROGRESS/history entries below.

Standing workflow: Continue autonomously until physical intervention is needed;
use a question pop-up to pause so I notice. No blind reflashing/build repetition.
Before any new arm: current time/deadline, normal receiver reboot if session used,
harmless document/another pointer and Glorious OFF/USB unplugged confirmed.
One serial reader/DTR;drain fresh startup;one p;actual arming confirmation BEFORE
mouse-ON cue. Keep bounded active window and x stop. No automatic rearm.
Prior authorization to work until18:00 Manila November16 must not be reused for
a later day or a new trial. Ask my new work window when resuming.

V13 software restored exact tested v11 main/bridge/HID harness frome106a79;
v12 startup-neutral did not improve target symptom. Only receiver duration/cap/
banner changed fromv11.8 native tests andreceiver/probe/radio builds passed;
probe/radio/default HID hashes match v11 controls. V13 flash/passive verified.
research/capture_receiver_session.ps1 uses full935s capture and explicit UTC
finish deadline with18min arm margin. New raw logger accepts1..1200s,compile/
self-test/boundary/1s smoke passed. Native runs/CIM/serial need escalation in this
environment. Current expired session must not be restarted blindly.
