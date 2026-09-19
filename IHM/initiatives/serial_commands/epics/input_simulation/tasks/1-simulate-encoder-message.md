# Task 1: simulate-encoder-message

**Status:** not started
**Branch:** `1-simulate-encoder-message` (from `tasks`)
**Depends on:** nothing

## Contract

### In

Nothing. Protocol-definition change plus a small, additive change to the
existing per-pass encoder-read loop in `main.cpp`.

### Delivers

1. **`mavlink/ihm_dialect.xml`** gains message **`IHM_SIMULATE_ENCODER`,
   id 305**, PC -> board:

   ```
   uint8_t encoder     # 0-2, target encoder index
   uint8_t direction   # enum IHM_ENCODER_DIRECTION -- CW or CCW; NOT_SUPPORTED is a no-op
   ```

   Message `<description>`: PC injects one simulated rotation step on the
   given encoder; the board applies it only if that encoder's real
   hardware read was idle (`not_supported`) on the same pass -- real
   input always wins, no ack, consumed exactly once (mirrors the real
   encoder's own edge-triggered/consumed-on-read semantics). Reuses the
   existing `IHM_ENCODER_DIRECTION` enum -- no new enum needed.

2. **`mavlink/generated/`** regenerated: `python mavlink/generate.py`.
   New `mavlink_msg_ihm_simulate_encoder.h` plus the touched shared
   headers, committed per policy (`mavlink/README.md`).

3. **`mavlink/README.md`** -- add the `IHM_SIMULATE_ENCODER` row to the
   Messages table (PC -> board, 2 bytes).

4. **`lib/MavlinkComms/src/MavlinkComms.h`** gains storage + a consuming
   getter, same shape as the existing `canConfig[2]`/`canConfigValid[2]`
   pattern:

   ```cpp
   uint8_t simulatedEncoderDirection[3];
   bool simulatedEncoderPending[3];
   ```

   - `dispatch()` gains a case for `MAVLINK_MSG_ID_IHM_SIMULATE_ENCODER`:
     decode, and if `encoder < 3` store `direction` and set
     `simulatedEncoderPending[encoder] = true` (unconditionally storing
     whatever direction was sent, including `NOT_SUPPORTED` -- validity
     is main.cpp's problem to check, matching how `getCanSignalConfig()`
     hands back whatever was last decoded).
   - New method `uint8_t consumeSimulatedEncoderDirection(uint8_t encoderIndex)`:
     if `encoderIndex >= 3` or nothing pending for it, returns `0`
     (`not_supported`); otherwise clears the pending flag and returns the
     stored direction. Do **not** `#include RotaryEncoder.h` here or use
     its `DIRECTION_TYPE`/`MAX_NUMBER_EMCODERS` -- this class stays
     decoupled from it (see the initiative's layering rule); hardcode `3`
     the same way `canConfig` already hardcodes `2` for CAN buses.

5. **`src/main.cpp`** -- inside the existing per-pass loop:

   ```cpp
   for (int i = 0; i < MAX_NUMBER_EMCODERS; i++) {
       dir[i] = rotaryEncoders.getDirection(i);
   ```

   add, still inside the loop body:

   ```cpp
       if (dir[i] == not_supported) {
           uint8_t simulated = mavlinkComms.consumeSimulatedEncoderDirection(i);
           if (simulated == CCW || simulated == CW)
               dir[i] = (DIRECTION_TYPE)simulated;
       }
   }
   ```

   Nothing downstream of `dir[i]` changes -- the existing rot0/rot1
   dispatch and the `IHM_BOARD_STATE` send both already consume `dir[]`
   unconditionally, so a simulated turn drives the UI and shows up in
   telemetry exactly like a real one, with no other edits needed.

## Dependencies

None. `IHM_ENCODER_DIRECTION`, `DIRECTION_TYPE`, and the per-pass
`dir[]` loop already exist exactly as described above.

## Pre-work

None beyond what `relay_control` task 1 already documented (Windows
Python + `pymavlink` for `generate.py`).

## Definition of done

- `python mavlink/generate.py` runs clean; `git status` shows only
  `ihm_dialect.xml`, files under `mavlink/generated/`, `mavlink/README.md`,
  `MavlinkComms.h`, and `main.cpp`.
- `platformio run` (env `megaatmega2560`) builds.
- `test_native/run_tests.ps1` passes (unchanged, but confirm).
- Task file marked done in the same commit.

## Notes

- Message ids in this branch's lineage: 300 `IHM_BOARD_STATE`, 301
  `CAN_SIGNAL_CONFIG`, 302 `RS485_SIGNAL_CONFIG`, 304 `IHM_RELAY_STATE`.
  303 belongs to the separate `pwm_control` branch and isn't present
  here. 305 is next and collides with neither.
- No `<include>common.xml</include>` -- same reasoning as the other
  messages.
- Conventional-commit: `feat(mavlink): add IHM_SIMULATE_ENCODER message`.
