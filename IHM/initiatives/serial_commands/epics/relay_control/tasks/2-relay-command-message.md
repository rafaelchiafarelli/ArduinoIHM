# Task 2: relay-command-message

**Status:** planned
**Branch:** `2-relay-command-message` (from `tasks`, off `relay_control`)
**Depends on:** `relay_control/1` (done: `IHM_RELAY_STATE`, the readback).

Rafael, 2026-09-27: PC -> board relay control, message shape **mask +
state**. The companion side is `pc_companion/6-relay-switches`.

## Contract

### Delivers

1. **`mavlink/ihm_dialect.xml`**: message **`IHM_RELAY_COMMAND`, id 309**,
   PC -> board, fields `mask` (uint8_t) and `state` (uint8_t). Relay *i* is
   set to `state` bit *i* when `mask` bit *i* is 1; relays with mask bit 0
   are untouched. No ack: `IHM_RELAY_STATE` (304) is the readback. Last
   writer wins against the on-screen switches and RE2. `IHM_RELAY_STATE`'s
   description loses "there is no PC -> board relay command yet".
2. **`mavlink/generated/`** (and `generated_py/`) regenerated with
   `python mavlink/generate.py`, committed per `mavlink/README.md`.
3. **`mavlink/README.md`**: `IHM_RELAY_COMMAND` row in the Messages table,
   304's row no longer says no command exists, and a "Switch relays" row in
   the commands table.
4. **Pure merge rule** in `lib/MultiOutput/src/RelayConfig.h`, unit-tested
   in `test_native/test_relay_config.cpp`:
   - `relayCommandAccumulate(uint8_t &pendMask, uint8_t &pendState, uint8_t mask, uint8_t state)`:
     folds a newer frame into a pending one; for bits in the newer `mask`
     the newer state wins, other pending bits are kept.
   - `relayCommandApply(uint8_t current, uint8_t mask, uint8_t state)`:
     returns `(current & ~mask) | (state & mask)`.
5. **`lib/MavlinkComms/src/MavlinkComms.h`**: `dispatch()` accumulates
   `IHM_RELAY_COMMAND` into a pending mask/state (so two frames in one
   superloop pass are not lost); `takeRelayCommand(uint8_t *mask, uint8_t *state)`
   copies out and clears atomically, false when nothing is pending. Storage
   only, no relay dependency (layering rule).
6. **`src/main.cpp` + `src/janus_actions.cpp`**: `janus_actions.cpp`'s
   `setRelay(index, on)` (hardware, `relayState[]`, `relay_instance` +
   dirty) is exported as `relaySet(index, on)` and declared in `main.cpp`.
   The superloop takes the pending command next to the PWM one and calls
   `relaySet` for each masked relay whose state changes. Repaint the dirty
   widgets only when the Output tab is showing, as the PWM block does.
7. **`mavlink/scripts/relay_cmd.py`**: `--port`, `--baud` (default 250000),
   `--set 0=1 3=0 ...` sends one `IHM_RELAY_COMMAND`, then prints the next
   `IHM_RELAY_STATE` it sees. Same style as `pwm_config.py`.

## Definition of done

`python mavlink/generate.py` runs clean; `platformio run` builds (RAM within
headroom); `test_native/run_tests.ps1` passes. Bench (ask Rafael before
flashing: the connected board is an old hardware revision, so this checks
the message path, not the relay bus): `relay_cmd.py --set 0=1` reads back
bit 0 set and the Output tab's relay 0 switch/LED follow; `--set 0=0`
clears it. Task file marked done in the same commit.
