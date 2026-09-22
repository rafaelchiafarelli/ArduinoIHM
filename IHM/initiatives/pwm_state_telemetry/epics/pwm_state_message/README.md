# Epic: pwm_state_message

PC observes the board's actual, applied PWM channel state over MAVLink --
the missing counterpart to `PWM_CHANNEL_CONFIG` (303, PC -> board), same
shape `relay_control`'s `IHM_RELAY_STATE` gave relays. See the initiative
README for why this exists and why it's a defect record, not active work.

## The one message

`IHM_PWM_STATE`, id 307 (next free after `IHM_SIMULATE_BUTTON`, 306),
board -> PC. One message covers all 4 channels, mirroring
`PWM_CHANNEL_CONFIG`'s own per-channel field layout (not bit-packed
across bytes -- same reasoning as every other message in this dialect,
see `mavlink/README.md`) and exactly the fields `main.cpp` already
computes into `pwm_instance` for the on-screen tab:

| Field | Type | Meaning |
|---|---|---|
| `ch0_enabled`, `ch0_duty_percent`, `ch0_inverting` | `uint8_t` x3 | channel 0 (simplex, output A only) |
| `ch1_enabled`, `ch1_duty_percent`, `ch1_inverting` | `uint8_t` x3 | channel 1 (simplex) |
| `ch2_a_enabled`, `ch2_a_duty_percent`, `ch2_b_enabled`, `ch2_b_duty_percent`, `ch2_c_enabled`, `ch2_c_duty_percent` | `uint8_t` x6 | channel 2 (complex, outputs A/B/C) |
| `ch3_a_enabled`, `ch3_a_duty_percent`, `ch3_b_enabled`, `ch3_b_duty_percent`, `ch3_c_enabled`, `ch3_c_duty_percent` | `uint8_t` x6 | channel 3 (complex) |

18-byte payload (3 + 3 + 6 + 6). Frequency selector is deliberately left
out: nothing on the PC side currently needs to display it back, and it
would need either 4x `PWM_FREQUENCY` bytes or a shared-timebase encoding
(channels 0/1 vs. 2/3 don't share timers, but 2 and 3 each have one
selector for all 3 outputs) -- revisit if a consumer actually needs it,
don't guess the shape now (workflow skill: explicit, not inferred).

Whether this should be one flat message (as above, matching
`PWM_CHANNEL_CONFIG`'s existing "channel" field pattern would instead
need 4 separate sends, one per channel, each keyed by `channel` like
`PWM_CHANNEL_CONFIG` itself does) is an open planning question for
whoever picks this up -- the task below assumes the flat form since it
needs no per-channel key and matches `pwm_instance`'s own flat shape, but
confirm against `janus_generated.harpia`'s `pwm` message (same flat
shape) before implementing.

## Tasks

```
1-pwm-state-message   dialect + regenerate + main.cpp send   (no deps)
```

## Acceptance gate

- `platformio run` (env `megaatmega2560`) builds, RAM within headroom.
- `test_native/run_tests.ps1` passes.
- A PC-side MAVLink listener on a connected board sees `IHM_PWM_STATE`
  frames whose fields match the on-screen PWM tab after a
  `PWM_CHANNEL_CONFIG` is applied, and after a real board reboot (default
  state).
