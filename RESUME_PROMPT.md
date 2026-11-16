Continue my DIY mouse receiver project in:
C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE

Read PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and docs/plans/2026-11-17-plug-and-play.md.
Inspect git status/log first. Branch feature/receiver-plug-and-play.
Keep context and this ready-to-copy prompt current for every meaningful step.
Commit/push tested milestones;use feature-based branch names. Private captures,
addresses and binaries stay ignored. Leave .vscode/ and INFO_UF2.TXT untracked.
Use question pop-ups when physical intervention is required,then pause.

V14 PLUG-AND-PLAY HARDWARE PASS / BREAK HANDOFF - 2026-11-17 01:51 Manila:
User confirms: all worked automatically and smoothly, including receiver USB
unplug/replug with mouse left ON, both mouse-only OFF/ON cycles, and controls at
16/18min beyond old limit. User additionally reports mouse STILL WORKING after
capture ended. This is intended: normal firmware runs independently until unplug,
without Codex, serial reader or arm command. User may keep using it wirelessly.
Do not stop/reflash/reset it merely because recording or the work session ended.

First normal trial:1160s mouse-only recording exit0,44814 receiver-only background
records,read_errors0/failed0. Receiver input span1131.448104s (18m51.448s);
11522 records after15min,8365 after16min,2710 after18min fromfirst receiver event.
LEFT213down/213up,RIGHT9/9,wheelUP107/DOWN132. Middle/thumb events absent in this
run;their deliberate v13 coverage and unchanged mapping remain separate evidence.
No CDC/DTR/arm command or Windows/DPI setting change. No active serial/raw/build/
UAC/helper processes. Normalv14 installed and autoactive/listening while connected;
NOT unarmed/passive/expired. Mouse is currently being used wirelessly per user.
Private base v14-normal-20261117-012742-226176cb with complete CSV/metadata/summary.
USB replug/cycles are user-confirmed; LastArrivalDate predates capture and does
not independently establish their timing. Background arrivals do not certify
physical polling/loss/latency. No claim of long-term reliability from one normal run.

Current branch feature/receiver-plug-and-play. Software3f5934c,flash/enumeration
withoutseriala05de2f,recordingfd4c32bpushed;nowrecordconfirmedhardwarepass. Normal
image SHA25693eb987ae2ba123404a241677b66e3374cfdf545f2a7f64a470f5f0f6ffd1052;
private preserved dongle-v14-93eb987a.uf2,46124flash/16064RAM,181UF2blocks,
0x26000..0x31500exclusive. V13rollback396d7dd8 remains preserved. Do not flash
bounded receiver-v14 image by mistake; normal uses build.ps1 -App dongle.

CORE NORMAL-DONGLE BEHAVIOR NOW USER-CONFIRMED. Automatic startup follows USB
configuration, no DTR/manualarm/30sdiscovery/15min/ACK/discoverycap;mouse link gaps
neutralize and rediscover. USB restart can automatically start normalmode again,
per user's latest request overriding earlier passive/finite preference. Optional
x stop/p restart is native-tested but NOT separately hardware-validated in normal
mode; use physical mouseOFF or receiver unplug for immediate stop when needed.
Abrupt unplug while held can leave Windows held state (also wired baseline);
release buttons before unplug. No speculative startup-neutral fixes needed.

Pause project work before02:00AMManilaNov17,as requested;dongle may remain in
ordinary use. NEXT on explicit project resume: refresh available work window,
review user experience from ordinary use;investigate only observed problems or
plan measured gaming latency/long-term/RF robustness qualification. Do not repeat
completed extraction/build/flash/functional baselines without reason. Old bounded
v13 serial-controller MUST NOT be run against silent normalv14. Use raw-only
capture for further normal-use observation unless deliberately changing mode.
Keep private captures/addresses/images ignored,context and copy-paste handoff
current,and push tested milestones. Unrelated .vscode/ and INFO_UF2.TXT untracked.
This current completed state supersedes earlier LIVE/pending/passive history.

Background:
V13 had two separate15min sessions with both mouse-only recoveries and clean
original stops;allfivebuttons+wheel physically validated. Frontthumb=Windows5,
rearthumb=Windows4. Foreground and intended-desk tests were smooth;peakhostdelivery
1000events/fullsecond,median1.003ms in desk trial. Notphysicalpolling/latency proof.
V14 shares RF timing/mapping/idleACKhold with v13;adds normal compile mode and
shared safe session preparation. All8 native checks pass with protocol subtests
probe/receiver/dongle;USBfault gates,normalno-commandstart,oldlimits/telemetrywrap,
x latch/fatalTX andboundedrearm/neutralfailurecases tested. Four firmware builds,
stagedsource/config andUF2validationpassed;defaultHIDunchanged. Buildnormalwith
firmware/build.ps1 -App dongle;bounded -App receiver is separate and notinstalled.
Normal config PRINTK/UART_CONSOLE/BOOT_BANNER disabled,HID+REBOOT enabled,1ms
endpoint. OptionalCDC input remains;no terminal required. SDKstagingoutsideOneDrive.
No new build or flash is needed for currently installed/tested normalimage.
