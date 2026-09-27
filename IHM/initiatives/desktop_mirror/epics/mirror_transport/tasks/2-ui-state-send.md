# Task 2: ui-state-send

**Status:** done 2026-09-27. Delivered as written. The send sits in the ~100 ms
telemetry tick: on change, else every 5th tick (500 ms), first tick always.
Pre-work check passed: focus is a runtime global and `janus_focus_index`
counts only the active screen's widgets, so `get(&janus_app)` reports the
index the nav-less `screenOnly` move set. Delta vs the baseline below:
flash +584 B (77414 B, 30.5 %), RAM +18 B (4849 B, 59.2 %). 135 native
tests pass. **Bench (epic gate) not run yet:** watching 308 on COM3 needs
the board. Deferred to the initiative's bench step.
**Branch:** `2-ui-state-send` (from `tasks`)
**Depends on:** task 1

## Contract

1. `main.cpp` calls `janus_remote_state_get(&janus_app, &s)` in the same
   telemetry section that sends 304/307. It sends `IHM_UI_STATE` when `s`
   differs from the last sent value, or when the heartbeat period has
   passed with no send.
2. It reads `janus_app`, the real app with its nav strip, **not** the
   nav-less `screenOnly` copy used for RE1 moves (`main.cpp`, "focus
   movement only reads the app"). Pre-work check before coding: confirm
   `get` reports the focus index RE1 actually produced, by reading
   `janus_remote.c` against how `screenOnly` moves focus. If they
   disagree, stop and flag it. That would be a Janus question, not
   something to patch here.
3. Record the flash/RAM delta in this file. `janus_remote.c` is its own
   translation unit, so this is its first link into the firmware.
   Baseline: 2026-09-27 `dev` 2c4a864, RAM 59.0 % (4831 B), flash 30.3 %
   (76828 B).

## Verification

Build + native suite. On the bench, use pymavlink to watch 308 while
turning RE0/RE1 (or `sim_input.py`).
