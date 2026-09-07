# Epic: pwm_control

PC configures any of the board's 4 PWM channels over MAVLink, so PWM
output can be exercised on the bench (scope on OC3A / OC5A / OC1A-C /
OC4A-C -- see `demo/HARDWARE_RUNBOOK.md`'s pin table) without driving the
on-screen UI.

## The 4 channels (from `lib/MultiOutput/src/PWM.h`)

| Channel | Timer | Output(s) | Shape | API |
|---|---|---|---|---|
| 0 | Timer3 | OC3A | simplex | `setupPWMChannel0(f_selector, isInverting, enabled, frequency, dutyCycle)` |
| 1 | Timer5 | OC5A | simplex | `setupPWMChannel1(...)` same signature |
| 2 | Timer1 | OC1A/B/C | complex | `setupPWMChannel2(f_selector, frequency, invA, enA, invB, enB, invC, enC, dutyA, dutyB, dutyC)` |
| 3 | Timer4 | OC4A/B/C | complex | `setupPWMChannel3(...)` same signature |

`f_selector` is `PWMFrequency` (`lib/MultiOutput/src/PWMTiming.h`) -- 13
fixed entries plus `frequency_variable` (raw ICRn TOP supplied in the
`frequency` arg). `dutyCycle` args are 0-100 (percent), scaled against the
mode's resolution TOP by `pwmResolutionTop()`.

The `PWM pwm;` global in `main.cpp` is the instance to drive -- it exists
but is never configured at runtime today.

## The one message (unified, keyed by channel)

`PWM_CHANNEL_CONFIG`, id 303, PC -> board. Mirrors how the Janus `pwm`
message already models all 4 channels in one flat shape.

| Field | Type | Meaning |
|---|---|---|
| `channel` | `uint8_t` | 0-3 |
| `f_selector` | `uint8_t` (enum `PWM_FREQUENCY`) | frequency selector, mirrors `PWMFrequency` |
| `frequency` | `uint16_t` | raw ICRn TOP; only read when `f_selector == PWM_FREQUENCY_VARIABLE` |
| `out1_enabled`, `out1_inverting`, `out1_duty_percent` | `uint8_t` x3 | output A (the only output for simplex channels 0/1) |
| `out2_enabled`, `out2_inverting`, `out2_duty_percent` | `uint8_t` x3 | output B -- ignored for channels 0/1 |
| `out3_enabled`, `out3_inverting`, `out3_duty_percent` | `uint8_t` x3 | output C -- ignored for channels 0/1 |

13-byte payload. (Alternative considered and rejected during planning:
two messages `PWM_SIMPLEX_CONFIG` / `PWM_COMPLEX_CONFIG` matching the C
API 1:1 -- more decode paths, no meaningful RAM/wire saving under the
64-byte cap.)

## Tasks

```
1-pwm-channel-config-message   dialect + regenerate            (no deps)
2-pwm-channel-config-mapping   pure config->args mapping + test (no deps)
3-pwm-config-receive-apply     MavlinkComms decode + main.cpp   (deps: 1, 2)
                               apply to hardware + runbook
4-pwm-send-script              pymavlink bench sender           (deps: 1)
5-pwm-tab-reflects-mavlink     mirror into pwm_instance so the  (deps: 3)
                               on-screen PWM tab tracks it
```

Tasks 2 and 4 can run in parallel sessions once 1 lands. Task 5 is a
follow-up -- the hardware works without it; only the TFT is out of sync.

## Acceptance gate

- `platformio run` (megaatmega2560) builds, RAM within headroom (was
  75.8% before this epic -- flag a material jump).
- `test_native/run_tests.ps1` passes.
- A `PWM_CHANNEL_CONFIG` sent from `mavlink/scripts/pwm_config.py`
  measurably changes the probed output on a connected board. This is
  Rafael's bench step -- the connected board (COM7) is an old hardware
  revision, so a pass there verifies the message/apply path, not the
  board layout.
