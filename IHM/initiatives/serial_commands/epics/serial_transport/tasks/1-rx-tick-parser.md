# Task 1: rx-tick-parser

**Status:** not started
**Branch:** `1-rx-tick-parser` (from `tasks`)
**Depends on:** nothing

## Contract

### Delivers

1. **`lib/MavlinkComms/src/MavlinkComms.h`**
   - `void tick()` -- feeds at most `MAVLINK_RX_BYTES_PER_TICK` (macro,
     default 16, `#ifndef`-guarded) available bytes into
     `mavlink_frame_char_buffer()`; on `MAVLINK_FRAMING_OK` calls the
     existing `dispatch()`. Bounded, never loops on `available()` alone,
     never blocks. Must stay ISR-safe: no `Serial.print`, no heap.
   - `poll()` is **removed** (its only caller is `main.cpp`).
   - The ISR-to-superloop hand-off accessors become atomic copy-outs:
     - `bool takeSimulatedEncoderDirection(uint8_t idx, uint8_t* dir)`
       replaces `consumeSimulatedEncoderDirection` (same one-shot
       semantics; copy + clear inside `ATOMIC_BLOCK`).
     - `getCanSignalConfig` / `getRs485SignalConfig` keep their shape
       (nothing consumes them yet) but get a header comment saying they
       are **not** ISR-safe until a consumer exists -- convert them to
       `take*` copy-outs in the task that adds the first consumer.
   - Still no dependency on `PWM`, `MultiOutput`, `MotorDC` or Janus.
2. **`src/main.cpp`**
   - Call `mavlinkComms.tick()` as the **last** statement of
     `TIMER2_COMPA_vect`; delete the `mavlinkComms.poll()` call and the
     three-line "this is bullshit..." note (its intent is now this task).
   - Update the simulated-encoder consume site to `take...()`.
3. **`mavlink/README.md`** -- short "Receive path" section: who fills the
   ring (core USART0 ISR), who drains it (Timer2 tick, bounded), who
   consumes results (superloop, atomic `take*`).
4. **`ARCHITECTURE.md`** -- serial section updated to match.

### Tests

Host-native test for the bounded-drain/dispatch logic is desirable but
`MavlinkComms` includes `<HardwareSerial.h>`. Only add a native test if it
can be done with a ~20-line fake `HardwareSerial` in `test_native/` without
touching production headers; otherwise say so in the commit and rely on the
AVR build + bench. Do not restructure `MavlinkComms` (template/interface)
to chase testability -- flag it as its own task instead.

## Definition of done

- `platformio run` builds; report RAM/Flash before/after.
- `test_native/run_tests.ps1` passes.
- Task file marked done in the same commit.
- Bench verification is Rafael's (see epic README "Risk").
