# Task 3: pwm-config-receive-apply

**Status:** done
**Branch:** `3-pwm-config-receive-apply` (from `tasks`)
**Depends on:** task 1 (message), task 2 (`PwmWireConfig` adapter),
`serial_transport` task 1 (ISR-side receive + atomic `take*` accessors)

> **Revised 2026-09-20.** (a) Receive now happens in the Timer2 tick, so
> the hand-off is an atomic copy-out, not a `get*()` pointer. (b) Duty
> goes through `computeSimplex/ComplexCallArgs` (raw OCR), not percent.

## Contract

### Delivers

1. **`MavlinkComms.h`** -- storage-only, no `PWM`/Janus dependency:
   - `mavlink_pwm_channel_config_t pwmConfig[4]; bool pwmConfigDirty[4];`
     (ctor-initialised false); `dispatch()` case for
     `MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG` stores when `channel < 4` and
     sets dirty (last writer wins per channel).
   - `bool takePwmChannelConfig(uint8_t ch, mavlink_pwm_channel_config_t* out)`
     -- `ATOMIC_BLOCK` copy + clear dirty; `false` if `ch >= 4` or not
     dirty.
2. **`src/main.cpp`** -- after the tick-driven receive, in the superloop:
   for ch 0..3, `take...`, map to `PwmWireConfig`, drop if
   `!pwmWireConfigValid`, then:
   - ch 0/1: `computeSimplexCallArgs(pwmWireToSimplex(w))` ->
     `pwm.setupPWMChannel0/1(a.frequency, a.inverting, a.enabled, a.rawFrequency, a.rawDutyCycle)`
   - ch 2/3: `computeComplexCallArgs(pwmWireToComplex(w))` ->
     `pwm.setupPWMChannel2/3(...)` in the signature's argument order.
3. **`demo/HARDWARE_RUNBOOK.md`** -- "Driving PWM over MAVLink" section:
   companion PWM panel and `pwm_config.py`, one simplex and one complex
   worked example with the probe pins, and the last-writer-wins note
   (until task 5 the TFT does not reflect PC-driven changes).

## Definition of done

`platformio run` (report RAM), `test_native/run_tests.ps1`, task file
marked done in the same commit. Waveform check is Rafael's bench step.
