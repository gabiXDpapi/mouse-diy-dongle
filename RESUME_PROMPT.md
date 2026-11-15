Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT),README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-16-receiver-explicit-stop.md.
Inspect git status/log first. Branch:feature/receiver-explicit-stop.
User requested a break after v9 software preparation;this continuation resumes.

V9 FLASH/PASSIVE CHECKPOINT COMPLETE 2026-11-16: user confirmed NICENANO
visible/Glorious wired-OFF. Bootloader0.6.0,nRF52840-nicenano,S1406.1.1 verified;
exact v9 hash ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e,
UF2 partition validation241 blocks0x26000..0x350FF PASS;copy completed.
COM12 expected USB identity verified.6.5s passive capture privately saved as
receiver-v9-passive-20261116-013345.txt:correct v9 explicit-x/quiet banner,
zero traffic,no errors/arm/link/disarm. COM12 closed,no live serial/raw/build.
Board NOW v9 UNARMED with unused65536 ACK/60s/eight-discovery budgets.
V8 rollback preserved. Metadata receiver-v9-flash-validation.json:armed=false.
NEXT PHYSICAL PAUSE: Glorious OFF/USB unplugged,selectable harmless text and
other pointer,same DPI/settings;leave receiver connected without reset.
Prepared ignored capture-v9-xstop.ps1 syntax PASS,not executed. Single p/DTR,
2s drain,95s serial+100s hidden raw;enforces01:56AM arm cutoff. Actual arm
confirmation BEFORE mouse ON. User circles5-10s,then hold LEFT dragging text.
Script sends x ONCE25s after observed link start,logs command host UTC/elapsed,
expects disarmed/neutral/reboot-required response. While LEFT physically held,
other pointer without clicks should leave selection fixed,then release.
No second p or automatic retry. Background raw for release events only,no
cadence claim. Verify motion stays v8-like by user report. End/save by02:00.

RESUMED UNTIL02:00 AM MANILA 2026-11-16: user authorizes work until02:00.
Current time on resume01:29. Prior break pause revoked for this interval.
Stop/save/close by02:00;no new armed60s trial after01:56,and start any longer
trial only if its full capture+wrap-up fits before02:00. Always recheck UTC time
before arming. V9 exact image hash reverified ecc725eb...;NOT flashed. NICENANO
not currently visible;physical readiness popup is next. Board last known v8
expired/passive59958 ACKs,COM12 closed,no live sessions. No build repeated.

EXPLICIT USER PAUSE 2026-11-16: user selected "Pause here — resume flashing
later" after v9 software milestone2f86867. Stop project work until user resumes.
V9 built/tested/pushed,NOT FLASHED;board remains v8 expired/passive59958 ACKs.
COM12 closed,no live serial/raw/build. Next after resume is physical NICENANO
flash readiness,not automatic copy/arming. See full v9 checkpoint below.

V9 EXPLICIT-STOP SOFTWARE CHECKPOINT COMPLETE 2026-11-16:
branch feature/receiver-explicit-stop. User resumed and requested break after
next step;software preparation complete,now at physical flash/break boundary.
V9 BUILT/VALIDATED,NOT FLASHED. Adds receiver-only lowercase x cancellation:
10ms paced CDC checks during scan/active loops,latches off ACKs/discovery/p
arming,disables RF before neutral,prints disarmed success only after release.
Reboot required to rearm. Poll interval is not a physical stop-latency guarantee.
Limits remain65536 ACKs/60s/8 discoveries/1s gap;no sustained limit increase yet.
Native active-stop regression failed on v8;all8 tests now PASS (prearm/active/
rediscovery cancellation,disable/neutral errors,no ACK after latch,no p rearm,
repeated handling and paced polling). Receiver/probe/radio builds/UF2 checks PASS.
Staged sources/config/HID separation/v9 banner+disarmed response verified.
Probe/radio controls not byte-identical after shared-source edits;native probe
behavior checked,new stop code receiver-only;default HID existing image unchanged.
V9 UF2 C:/Users/Gabriel/zephyrproject/build-mouse-receiver/zephyr/zephyr.uf2
SHA256ecc725eb44c39194d1288873b049f26fedc66bbf0d300768d991f349cfe4718e.
61536 flash/16128 RAM;241 blocks,0x26000..0x350FF. Private rollback/candidate
receiver-v8-5df75330.uf2 and receiver-v9-ecc725eb.uf2;metadata
build-pairing-analysis/receiver-v9-build-validation.json. No v9 hardware claim.
Board STILL validated v8 EXPIRED/PASSIVE,one arm/discovery,59958 ACKs used.
COM12 closed,no live serial/raw/build. No new flash/arming on this resumed turn.
NEXT PHYSICAL PAUSE when user continues: Glorious wired/OFF,double-reset only
RECEIVER into NICENANO;verify bootloader identity/exact v9 image before copy,
then passive startup without arming. Separate explicit motion/held-left x-stop
readiness and one arm;capture x command timestamp,neutral/reboot-required response
and Windows release while physically held. Also check motion remains v8-like.
Do not extend limits until explicit-stop hardware pass;preserve v8 as baseline.
Do not repeat completed extraction/v8 tests/builds without a new reason.

Validated v8 results:foreground motion feels wired-like,no random overshoot;
movement,left/right clicks,wheel up/down before/after mouse-only OFF/ON;clean
original60s stop;held-left timeout releases Windows while physically held beyond
70s. Current board still v8;no p can be reused before normal reboot. V8 image
SHA2565df75330e7d0ac8bd95af8fc33799bac92d83e5a87f93e32440748e3960e97bd.
V9 is a stop prerequisite,not yet a longer-session version. Do not enable
unbounded operation or promise daily-use/gaming reliability/physical1000Hz.
Current first discovery arm30s,once per boot. One serial reader,DTR,drain2s,
single p,actual new arming confirmation BEFORE mouse power-on. Separate physical
readiness via question popups for flash and each live trial. Never assume reset,
USB unplugging,mouse power state or readiness. Capture enough time for limits.

research/capture_raw_mouse.c is host-only mouse logger. CLI accepts seconds
(1..180),new.csv,[--foreground]. Input_code0 foreground/1 background. User must
activate window for motion cadence;background delivery may be throttled/coalesced
by Windows. Raw arrival is not physical latency/DPI/RF loss. Wired CORE Profile1
selected1600 saved unchanged;wireless equivalence unverified. Windows accel1/
speed9 unchanged;receiver X/Y1:1. No guessed scaling/ACK retuning.

Continue until physical intervention;use question pop-ups to pause. Update
PROJECT_CONTEXT.md AND RESUME_PROMPT.md after meaningful steps. Push tested
milestones on feature-based branches. Private captures/addresses/binaries ignored
under build-pairing-analysis;leave .vscode/ and INFO_UF2.TXT untracked.
