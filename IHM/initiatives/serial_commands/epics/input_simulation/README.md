# Epic: input_simulation

PC injects simulated rotary-encoder rotation (CW/CCW) over MAVLink, so
the board's on-screen Janus UI can be navigated and tested from the PC
controller app without touching the physical encoders -- "emulate the
actual commands from the user."

## Scope of this first cut: encoder rotation only, not button presses

The board has 3 rotary encoders (`RotaryEncoder.h`, `MAX_NUMBER_EMCODERS`)
and 7 buttons (4 standalone + 3 encoder push-buttons, see `ButtonMap.h`).
This epic's first task covers only encoder CW/CCW simulation -- the
piece actually requested. Simulated button presses (including the
encoder push-buttons, which activate/confirm the focused item) are a
natural follow-up task in this same epic, using the same wire pattern,
but are not built here.

## How real encoder input reaches the UI (src/main.cpp)

Each superloop pass:
```cpp
DIRECTION_TYPE dir[MAX_NUMBER_EMCODERS];
for (int i = 0; i < MAX_NUMBER_EMCODERS; i++)
    dir[i] = rotaryEncoders.getDirection(i);
```
`getDirection()` is edge-triggered and consumes its latch on read -- a
real turn shows up as CW/CCW for exactly one pass, then reads
`not_supported` again. `dir[0]` switches the active screen (tab), `dir[1]`
moves focus within the screen; `dir[2]` currently drives nothing but is
still reported in `IHM_BOARD_STATE` telemetry.

## The mechanism: consumed only when the real read is idle

A simulated direction takes effect **only when the real encoder read for
that pass is `not_supported`** -- real hardware always wins if someone is
also physically turning the encoder on the bench at the same time; there
is no fight between the two sources, and no new state machine. Consumed
exactly once (mirrors the real encoder's own one-shot-per-turn
semantics), so a single simulated CW/CCW behaves identically to a single
real detent.

## The one message

`IHM_SIMULATE_ENCODER`, id 305, PC -> board, command.

| Field | Type | Meaning |
|---|---|---|
| `encoder` | `uint8_t` | Target encoder, 0-2. Out-of-range is dropped by the board. |
| `direction` | `uint8_t` (enum `IHM_ENCODER_DIRECTION`) | `CW` or `CCW`; `NOT_SUPPORTED` is a no-op. |

2-byte payload. No ack -- same "board applies immediately, PC doesn't
wait for confirmation" convention as `PWM_CHANNEL_CONFIG`; the PC can
already see the effect either on the physical screen or by watching its
own `IHM_BOARD_STATE.encoderN_direction` echo back.

**Id note:** 303 (`PWM_CHANNEL_CONFIG`) is reserved by the separate,
not-yet-merged `pwm_control` epic branch and does not exist in this
branch's `ihm_dialect.xml` yet -- 305 is the next id unused in *this*
branch's lineage (300-302, 304). No collision either way; whoever merges
all three epics together just ends up with a dialect containing
300-305, once.

## Tasks

```
1-simulate-encoder-message   dialect + regenerate + MavlinkComms + main.cpp   (no deps)
```

## Acceptance gate

- `platformio run` (env `megaatmega2560`) builds, RAM within headroom.
- `test_native/run_tests.ps1` passes.
- Sending `IHM_SIMULATE_ENCODER` from a connected PC (the IHMPCController
  app, or a throwaway pymavlink script) while the real encoder is idle
  visibly moves the on-screen focus/tab the same way a real turn would.
