# Idle ACK Queue Guard Experiment

**Goal:** Test whether advancing idle ACK sequence can discard newly queued motion and cause wireless-only speed variation.

**Evidence and limit:** Mouse snapshot0x1890A..0x18950 builds idle from empty queue;0x16088..0x160B8 rereads current queue and pops on ACK=sequence+1. A report can potentially arrive between those steps. This interleaving is inferred,not proven;original receiver behavior remains unknown.

**Smallest change:** Receiver v7 uses ACK sequence unchanged for length2 idle;length11/type1 or status and length20 compound still use sequence+1 modulo256. Second ACK byte,RF timing/identity/CRC filters,65536/60s/eight-discovery bounds,recovery,USB mapping/backpressure/neutral release unchanged. Non-HID probe retains old idle+1 behavior as control. No Windows settings changed.

**Scope:** `discovery_probe.h`,receiver banner in `main.c`,actual `test_probe.c`,README/context/research. Preserve tested v6;branch `feature/idle-ack-queue-guard`.

1. Fail native actual-ACK assertions for repeated idle and255/0 boundary followed by data using same sequence. Check data/status+1 and non-HID mode unchanged.
2. Add only the receiver idle ACK conditional;mark v7 experiment in banner.
3. Run all eight tests;build/check receiver/probe/radio,UF2 bounds and binary/config/source separation. Preserve default HID and v6 artifacts.
4. Update exact state/hashes and push software checkpoint;pause for physical NICENANO readiness.
5. Verified v7 flash/passive check,separate harmless-document/mouse-OFF readiness,single-arm95s steady-motion comparison. Record user sensitivity consistency plus idle/type1/HID/retry/busy counters;do not infer cause from report totals alone. Revert to preserved v6 if link/input behavior worsens.

**Software checkpoint complete:** intended receiver ACK assertion failed before
implementation,then all8 native tests pass,including idle/data/status/compound
wrap cases and unchanged non-HID behavior. All three builds/UF2 bounds and
source/config/binary checks pass;probe/radio/default HID artifacts unchanged.
V7 hashb6a63b99...,full metadata in context. Hardware still expired v6;PAUSED
before physical flash. Queue-race hypothesis and v7 motion quality unproven.
