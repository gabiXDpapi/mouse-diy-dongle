# Timed host movement delivery measurement

Goal: measure normal v14 foreground movement delivery in a predefined sustained
motion epoch, after live USB1ms descriptor/active-pipe verification7e23c76. No
firmware flash, serial, DPI/Windows setting changes or input injection.

Reuse capture_raw_mouse.c; preserve existing3arg/background and --foreground
modes, CSV columns, device privacy/filtering, CREATE_NEW output and1200s hard cap.
Add --timed-motion: wait for window activation,5s countdown,30s MOVE,5s STOP then
close automatically. Losing foreground during countdown/movement invalidates the
trial. QPC timestamp markers define phases exactly; timer dispatch may be delayed,
so marker times rather than assumed walltime are authoritative. Update text only
at phase/whole-second changes. No keyboard capture, focus stealing or mouse blocking.

Files: new research/motion_epoch.h pure state helper; state assertions in existing
native --self-test fail before helper implementation. Existing raw selftests remain.
Build via new build_timed_raw_mouse.ps1 to ignored capture_raw_mouse_timed.exe,
leaving historical capture_raw_mouse.exe/hash untouched for old controllers.
New --timed-motion defaults no longer than180s per trial through its controller;
abort/incomplete phase returnsfailure. No silent retry. Timermax still1200s.

Analyze via research/analyze_motion_delivery.py and tests. Require complete
markers, nofocusloss/readerror/failure, source receiver or wired, relative motion
and foreground delivery. Use central20s (5s after MOVE start through25s), excluding
warmup/stop reaction by design; never select best epochs after observing data.
Report all20 one-second counts, mean/min/max movement-event rate, gapmedian/p95/
p99/max and>2ms gaps. Include zero-motion count; reject control clicks/wheel during
the measured epoch. Validate timestamps/fields and normal closure. Tests cover
1kHz fixture, exact boundaries, background/absolute/control rejection, missing
markers/errors, incomplete and partial CSV, otherdevice filtering and slow/gapped
runs. Output is host-delivery evidence, NOT physical USBtokens/sensorfreshness/
end-to-endlatency or proof that gaps areRF loss. WM_INPUTqueueing/coalescing and
actual hand movement remain confounders.

Controller verifies normalUSBproduct, starts visible180s boundedcapture only,
reportsready for useractivation, then validatesclosedCSV and savesprivateJSON.
Selftestguards for raw completion/deadline ifnewlogic needs them. Physicalpopup
only aftersoftwarechecks/push: normalv14connected, GloriousON USBunplugged, same
DPI/surface,otherpointer. User activates capturewindow thenfollowscountdown/motion/
stop. Record usercontinuousmotion/focus/pauses and inspect actualcounts without
premature1kHz qualification. Keepcontext/handoffcurrent andpushmilestones.


### First guided normal-wireless delivery trial: discrepancy unresolved

The guided window completed its countdown/MOVE/STOP phases without focus loss,
raw read errors or capture failure. The predefined central20seconds contained
11,078 receiver movement events (553.9events/s), including three initial seconds
with zero receiver events. Later seconds contained286–809events/s. Median host
arrival gap was1028us,p95 2972us,p99 4187us; these statistics are not USBtoken or
sensor timestamps. All logged sources were receiver, with no wired records.

The user explicitly reports moving continuously from the MOVE cue and feeling
smooth motion. Preserve that report alongside the discrepancy; do not assume a
late start, trim away the zero seconds or diagnose RF loss from this capture.
The timed markers/source filtering were rechecked. Next: same guided wired
comparison to test the common host/capture path, then reassess wireless evidence.
Use Glorious itself to activate the window and keep it in hand during countdown,
reducing pointer-handoff ambiguity. No firmware or setting change is involved.

The controller now accepts `-Source wired` and verifies a unique258A:2018 root;
receiver mode continues to require normalv14. Analyzer source isolation and
controller identity/error gates pass18analysis tests andSelfTest/syntax checks.
The same timed native executable is reused, without a rebuild or altered timing.
Private run:v14-timed-motion-20261118-184803-b718bd7e. Sustained1kHz remains
unqualified despite separately verified live1ms descriptor/active-pipe configuration.


### Guided wired small-circle reference

The same timed native binary and fixed central20seconds captured12,469 wired
movement events (623.45events/s), ranging328–879persecond with nozero seconds.
Median host gap1024us,p95 3031us,p99 5063us. All18,103 raw records were wired;
no receiver-source mixing. Phase/focus/completion checks passed without raw errors.
The user confirmed continuous smooth movement, mostly small/controlled circles.

This reference also falls below1000nonzero movementevents/s. It therefore does
not establish the wireless receiver as the cause of the low movement-event count.
Movement size/speed and host delivery still need controlling. Next use comfortable
larger steady circles in wired mode, then match that motion wirelessly with the
target mouse itself activating the window; no DPI/Windows/firmware changes. This
is a defined movement-size/source check, not another firmware baseline repetition.
Keep all fixed-epoch results; do not trim/select the best seconds or call event
gaps RF loss. No sustained fresh1000Hz or physical token cadence claim yet.
