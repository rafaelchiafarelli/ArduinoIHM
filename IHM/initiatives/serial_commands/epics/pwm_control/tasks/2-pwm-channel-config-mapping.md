# Task 2: pwm-channel-config-mapping

**Status:** not started
**Branch:** `2-pwm-channel-config-mapping` (from `tasks`)
**Depends on:** nothing (decoupled from task 1 via a POD struct)

> **Revised 2026-09-20.** The original draft invented a second
> config->args layer (`PwmChannelConfig`/`pwmResolveArgs`) and proposed a
> filename that collides case-insensitively with the existing
> `PWMChannelConfig.h` on Windows. It also assumed `setupPWMChannelN`
> takes duty **percent**; it takes a **raw OCR count**
> (`regs.ocrA = dutyCycle`, `PWMConfig.cpp`). The existing
> `computeSimplexCallArgs` / `computeComplexCallArgs`
> (`PWMChannelConfig.h`) already do the percent->raw scaling correctly, so
> this task now only adds the *wire -> existing config types* adapter.

## Contract

### In

A wire-shaped POD, independent of `mavlink.h` so it stays host-testable:

```c
struct PwmWireOutput { uint8_t enabled, inverting, duty_percent; };
struct PwmWireConfig {
    uint8_t channel;        // 0-3
    uint8_t f_selector;     // wire value of PWM_FREQUENCY
    uint16_t frequency;     // raw ICRn TOP, only for f_selector == frequency_variable
    PwmWireOutput out[3];   // A,B,C; [1],[2] ignored for channels 0/1
};
```

### Delivers

New `lib/MultiOutput/src/PWMWireConfig.h` + `.cpp` (name deliberately
distinct from `PWMChannelConfig`), AVR-header-free:

1. `bool pwmWireConfigValid(const PwmWireConfig&)` --
   `channel <= 3` and `f_selector < NUMBER_OF_PWM_FREQUENCIES`. Duty is
   **not** a validity failure (clamped below); flags are treated as
   `!= 0`.
2. `PWMChannelConfig pwmWireToSimplex(const PwmWireConfig&)` (channels 0/1):
   `enabled = out[0].enabled`, `inverting`, `frequency = (PWMFrequency)f_selector`,
   `variableTopValue = frequency`, `dutyCyclePercent = min(duty,100)`.
3. `PWMComplexChannelConfig pwmWireToComplex(const PwmWireConfig&)`
   (channels 2/3): channel `enabled = true` (per-output flags do the
   gating -- see the `enabled` gate comment in `PWMChannelConfig.h`),
   `outputA/B/C` from `out[0..2]`, same clamp.
4. The register call still goes through the **existing**
   `computeSimplexCallArgs` / `computeComplexCallArgs` -- do not
   reimplement scaling.

### Tests

`test_native/test_pwm_wire_config.cpp`; add `PWMWireConfig.cpp` to
`$prodSourceAllowlist` in `run_tests.ps1`. Cases: valid ch0..3; ch 4
invalid; f_selector 13 valid / 14 invalid; duty 150 -> 100; variable TOP
survives; simplex ignores out[1..2]; **end-to-end**: wire ch0 @ 62.5 kHz,
50% -> `computeSimplexCallArgs(...).rawDutyCycle` equals
`scaleDutyCycleToRaw(50, pwmResolutionTop(...))` (guards the
percent-vs-raw trap above).

## Definition of done

`test_native/run_tests.ps1` passes; `platformio run` builds; task file
marked done in the same commit.
