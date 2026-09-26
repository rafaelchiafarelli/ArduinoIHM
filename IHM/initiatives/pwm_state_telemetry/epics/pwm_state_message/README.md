# Epic: pwm_state_message

PC observes the board's actual, applied PWM channel state over MAVLink --
the missing counterpart to `PWM_CHANNEL_CONFIG` (303, PC -> board), same
shape `relay_control`'s `IHM_RELAY_STATE` gave relays. See the initiative
README for why this exists and why it's a defect record, not active work.

## The one message (as built, 2026-09-26)

`IHM_PWM_STATE`, id 307, board -> PC. **Per channel, same 12 fields as
`PWM_CHANNEL_CONFIG` (303)** -- `channel`, `f_selector`, `frequency`,
`out1..3_{enabled,inverting,duty_percent}` -- 13-byte payload, 25 bytes on
the wire. The board sends one channel per ~100 ms tick, round-robin 0-3, so
all four refresh every ~400 ms.

Source is `main.cpp`'s `pwmLast[4]` (the last config applied per channel,
from the PC or an on-screen switch, added by `fixes/000004`), not
`pwm_instance` as first planned: `pwmLast` also has the frequency and the
per-output inverting of the complex channels, which `pwm_instance` lacks.
Duty is clamped to 100 on the way out, as the adapters clamp it when
applying.

Decided by Rafael 2026-09-26, replacing this README's earlier flat 18-byte
draft (no frequency, no complex-channel inverting) and its open "flat vs.
per-channel" question:

- **Per-channel mirror of `PWM_CHANNEL_CONFIG`**, so the PC compares what it
  sent with what was applied field by field.
- **TX ring 64 -> 128** (`UART2_TX_RING_SIZE`). Each ~100 ms tick queues
  `IHM_BOARD_STATE` (31 B) + `IHM_RELAY_STATE` (13 B) + `IHM_PWM_STATE`
  (25 B) = 69 B. That doesn't fit 64, and `writeFrame()` drops whole frames.
  128 is `ByteRing`'s maximum.
- **Companion app in scope**: live `PWM chN` readback and an
  applied/differs result in place of "no ack -- check the output pin".

## Tasks

```
1-pwm-state-message   dialect + regenerate + main.cpp send + companion   (no deps) -- done
```

## Acceptance gate

- `platformio run` (env `megaatmega2560`) builds, RAM within headroom.
- `test_native/run_tests.ps1` passes.
- A PC-side MAVLink listener on a connected board sees `IHM_PWM_STATE`
  frames whose fields match the on-screen PWM tab after a
  `PWM_CHANNEL_CONFIG` is applied, and after a real board reboot (default
  state).
