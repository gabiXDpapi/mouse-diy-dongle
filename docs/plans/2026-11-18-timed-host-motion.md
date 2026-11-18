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
