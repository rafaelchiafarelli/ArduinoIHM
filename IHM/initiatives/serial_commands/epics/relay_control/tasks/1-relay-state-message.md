# Task 1: relay-state-message

**Status:** not started
**Branch:** `1-relay-state-message` (from `tasks`)
**Depends on:** nothing

## Contract

### In

Nothing. Protocol-definition change plus one send call wired to state
that already exists (`main.cpp`'s `relayState[NUMBER_OF_RELAYS]`).

### Delivers

1. **`mavlink/ihm_dialect.xml`** gains message **`IHM_RELAY_STATE`, id
   304**, board -> PC:

   ```
   uint8_t relays   # bitmask, bit i = relay i (0-7), 1 = on
   ```

   Message `<description>`: periodic snapshot of all 8 relay outputs,
   board -> PC telemetry, mirrors `main.cpp`'s `relayState[]` the same
   way `IHM_BOARD_STATE.buttons` mirrors the button bitmap. Note this is
   read-only observation -- no PC -> board relay command exists yet (see
   the epic README).

2. **`mavlink/generated/`** regenerated: `python mavlink/generate.py`.
   New `mavlink/generated/ihm_dialect/mavlink_msg_ihm_relay_state.h` plus
   the touched shared headers, committed per policy
   (`mavlink/README.md`).

3. **`mavlink/README.md`** -- add the `IHM_RELAY_STATE` row to the
   Messages table (board -> PC, 1 byte).

4. **`lib/MavlinkComms/src/MavlinkComms.h`** gains
   `sendRelayState(uint8_t relayMask)`, packing and transmitting
   `IHM_RELAY_STATE` -- same shape as the existing `sendBoardState()`
   (caller gathers the value, this class only packs/sends it; see the
   initiative's layering rule).

5. **`src/main.cpp`** builds the bitmask from `relayState[NUMBER_OF_RELAYS]`
   (bit *i* = `relayState[i]`) and calls `mavlinkComms.sendRelayState(...)`
   in the same `timeCounter>=100` block that already calls
   `sendBoardState()` -- same cadence, no new timing mechanism.

## Dependencies

None. `relayState[NUMBER_OF_RELAYS]` in `main.cpp` already exists and is
already the single source of truth relays are read from for the
on-screen tab.

## Pre-work

None beyond what task 1 of `pwm_control` already documented: `pymavlink`
is installed under the Windows Python this repo's `generate.py` is run
with (`C:\Users\rafae\AppData\Roaming\Python\Python314\site-packages\pymavlink`).

## Definition of done

- `python mavlink/generate.py` runs clean; `git status` shows only
  `ihm_dialect.xml`, files under `mavlink/generated/`, `mavlink/README.md`,
  `MavlinkComms.h`, and `main.cpp`.
- `platformio run` (env `megaatmega2560`) builds.
- `test_native/run_tests.ps1` passes (unchanged, but confirm).
- Task file marked done in the same commit.

## Notes

- Message ids so far: 300 `IHM_BOARD_STATE`, 301 `CAN_SIGNAL_CONFIG`,
  302 `RS485_SIGNAL_CONFIG`, 303 `PWM_CHANNEL_CONFIG` (separate,
  not-yet-merged `pwm_control` epic branch). 304 is next and does not
  collide with it -- both branches add a message, neither edits the
  other's.
- No `<include>common.xml</include>` -- same reasoning as the other four
  messages.
- Conventional-commit: `feat(mavlink): add IHM_RELAY_STATE message`.
