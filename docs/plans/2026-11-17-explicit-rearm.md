# Reusable explicit bounded sessions

The later plug-and-play request supersedes this as the normal-use target. These
changes remain in the bounded diagnostic receiver; see the plug-and-play plan.

User selected reusable explicit Start/Stop, retaining finite sessions and passive
USB reconnect. Preserve the tested v13 image. V14 changes command/session state,
not RF bytes, timing, mapping, quiet active behavior or USB reset policy.

- `p` starts one 30-second discovery arm, then at most 900,000 ms, 983,040 ACK
  attempts and eight discoveries. Repeated `p` during an arm, link or recoverable
  gap is ignored; it cannot renew the current session's budgets or deadline.
- Successful `x`, final time/ACK/discovery exhaustion, or initial discovery-arm
  expiry finishes neutralization and exposes an explicit rearm-ready response.
  No RF output resumes until a subsequent `p`. A gap within the existing session
  remains recoverable and does not expose fresh budgets.
- `p` after the ready response resets only session counters/deadlines/deduplication.
  USB offline/fault/restart and neutral/radio failures cannot be bypassed. USB
  reconnect still restarts passive. No persistent writes, autoarm or unlimited mode.

Touch discovery guards/command state, bridge session preparation, native protocol
and bridge checks, receiver banner, host capture compatibility and documentation.
Tests first: rearm after clean x/expiry, timeout without discovery, budget exhaustion,
ignored p during active/gap, pending/failed stop, fresh counters and USB-fault gate.
Run all native checks and receiver/probe/radio builds, validate UF2/config/staging,
preserve rollback and push software milestone before flash readiness.

After verified flash/passive startup, test two explicitly armed short sessions
without resetting/unplugging receiver between them. One serial reader; actual arm
confirmation before each mouse-on cue and physical readiness before each arm.
Confirm neutral x, passive interval, new counters/deadline, controls and no stuck
buttons. Save partial failures honestly; do not call software tests hardware proof.

Current pause deadline: 02:00 AM Manila November 17, 2026 (18:00 UTC November 16).
Refresh deadline on later resume. Keep captures, addresses and images ignored.
