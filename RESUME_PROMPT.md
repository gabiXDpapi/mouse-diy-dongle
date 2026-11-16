V13 FOREGROUND MOTION PASS - 2026-11-17 00:21 Manila:
User:Smooth like wired;stopped cleanly. Full100s raw/serial exit0,39210 receiver
records,ALL code0 foreground,read_errors0/failed0. One p,one link,one x,one neutral
linkstop+disarm,no ERROR. x sent60027ms after host-observed link;disarm chunk25ms
later (host timestamps,not device precision). Native controller mode hardware PASS.
Foreground median gap1007us,p90=2016us,p99=4926us,max939042us.56 interior one-second
bins;peak988events/s,4 bins950..1050.39208 motion records,maxabsX36/Y22. Firmware
58809 ACKs,39210 HIDreports,39 retries,535 busy;idle19559/status1/type139249.
User confirmed capture window active. Reused older wired/v8 foreground reference;
manual motion not matched,so do NOT rank latency/loss or diagnose maxgap as RF stall.
Host data supports near1000event/s capability in parts of run,not certified physical
1000Hz or end-to-end latency. No firmware/settings/DPI changes or injected input.
Private base v13-motion-20261117-001840-d854159f (csv,serial,journal,stdout/stderr,
capture.json,summary.json),ignored. exec23691 complete;no live serial/raw/helper/
UAC/build. Installedv13 now DISARMED/neutral;normal reboot required for new arm.
Glorious instructed reconnectUSB;cable not independently verified. Deadline02:00 AM
Manila November17 (UTC2026-11-16T18:00Z). Branch feature/receiver-foreground-timing.
NEXT repeatability: second bounded15min normal-use/recovery trial on samev13,
using existing default935s controller;reason is repeated reliability beyond single
long-run pass,not a new build/baseline. Need new normal reboot/input readiness pop-up.
No flash. Keep same surface/DPI/settings and harmless doc/other pointer. Mouse-only
OFF3s/ON around5/10min;all controls/smoothness/original15min cleanstop. Existing
foreground test establishes host capacity;defaultlongrun backgroundcannotcertifypolling.
No15min arm at/after01:42. Keep private evidence ignored and push tested result.
EarlierLIVE entries are history;currentcompletedstate here supersedes them.

V13 FOREGROUND CAPTURE LIVE - 2026-11-17 00:18 Manila:
User confirmed fresh normal receiver reboot/GloriousOFF USBunplugged/other pointer.
Controller -ForegroundMotion started;fresh v13 verified,one p,actual arming
confirmed before powerON popup. LIVE exec23691,private base
v13-motion-20261117-001840-d854159f.100s raw+serial;auto x at60s from first link.
Do NOT open another reader/rearm/reset. User activates visible capture window,
continuous circles/strokes45-60s;no clicks/wheel/DPI/powercycle. Await completion
and user observations;not yet a passed measurement. Deadline02:00 Manila.
If user requests stop,create exact private base.stop-request (one x,partial capture).
NEXT poll23691,analyze code0 host timing with manual-motion limitations,save/push.

V13 FOREGROUND CONTROLLER READY / PHYSICAL SETUP NEXT - 2026-11-17:
Branch feature/receiver-foreground-timing. Current deadline02:00 AM Manila today
(UTC2026-11-16T18:00:00Z). No hardware access,build,flash or arming this session.
Existing capture_receiver_session.ps1 now accepts -ForegroundMotion:100s serial/
raw,existing visible Raw Input window,one x after60s from first link,confirmed
neutral/disarm and full raw completion. Default935s fifteen-minute mode unchanged;
conservative18min deadline margin retained in both modes. SelfTest passes original
deadline/single-stop plus exact60s motion deadline/no-link/disabled cases;syntax
and diff checks PASS. New live mode NOT hardware-tested. No firmware changes.
Reuses prior wired foreground reference;do not repeat extraction/build/flash/wired
baseline. NEXT pop-up: receiver normal unplug/replug (not double-reset),Glorious
OFF/USBunplugged,unchanged DPI/surface/settings,other pointer. Keep OFF until new
actual arming confirmation. Then ON,use other pointer to activate capture window,
move continuous circles/strokes45-60s without buttons/wheel/powercycle. Keep window
active until automatic close;expect x stop around60s after link. Capture lasts100s.
Analyze code0 only,report manual motion/focus limitations;no physical1000Hz/latency
claim from host arrival alone. Old v13 fifteen-minute result preserved.

RESUMED / V13 FOREGROUND TIMING NEXT - 2026-11-17 00:12 Manila:
User resumes and sets pause time2 (interpreted/announced02:00 AM Manila today,
UTC2026-11-16T18:00:00Z). Yesterday18:00 deadline expired. Git clean except
unrelated untracked .vscode/ and INFO_UF2.TXT; latestfb26dad pushed. No hardware
access,arming,build,flash or live capture has occurred in resumed session.
Installed image last verifiedv13; last session expired/neutral,one arm used.
Current physical connection/state not reverified since break.
Reviewed context,README,radio analysis,original wireless HID plan,15min plan,
foreground capture plan,existing C logger/controller and prior timing summaries.
Reuse prior wired foreground reference raw-wired-foreground-20261116-002746;
no repeat of completed wired baseline. V8 foreground capture50748records/allcode0,
median hostgap1001us and several~1000event/s seconds. This is host delivery,
NOT physical polling/latency certification. V13 full935s recording was entirely
background(code1),so cannot establish current foreground delivery rate.
NEXT focused v13 motion-only foreground timing,unchanged firmware: normal receiver
reboot;GloriousOFF/USBunplugged,same surface/DPI/settings,other pointer available.
One serial reader/freshv13 banner/onep/actualconfirm before mouseON. Show existing
Raw Input window;user activates with other pointer and makes steady circles/strokes
~45-60s,no clicks/wheel/DPI/powercycle. Explicit x around60s after first link;
full100s serial/raw captures with confirmed neutral/disarm,not full15min validation.
Need prepare bounded controller by reusing existing session and xstop patterns,
with deadline/one-stop checks;no firmware build or flash needed. Pause via readiness
pop-up now;do not arm based on yesterday's readiness. Save controls/feel separately
from host statistics;foreground(code0) only for rates/gaps;manual motion gaps are
not proof of RF loss. Finish/save/push by02:00. Existing v13 pass remains baseline.

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
