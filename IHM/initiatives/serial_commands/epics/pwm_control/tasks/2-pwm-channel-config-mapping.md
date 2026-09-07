# Task 2: pwm-channel-config-mapping

**Status:** not started
**Branch:** `2-pwm-channel-config-mapping` (from `tasks`)
**Depends on:** nothing (deliberately decoupled from task 1 via a POD struct)

## Contract

### In

A plain-old-data description of one channel's requested PWM config,
independent of the MAVLink wire type so this layer stays host-testable
without pulling in `mavlink.h`:

```c
struct PwmOutputConfig {
    bool     enabled;
    bool     inverting;
    uint8_t  duty_percent;   // caller-supplied, may be out of range
};

struct PwmChannelConfig {
    uint8_t          channel;      // 0-3
    uint8_t          f_selector;   // cast of PWMFrequency
    uint16_t         frequency;    // raw ICRn TOP, only meaningful when f_selector == frequency_variable
    PwmOutputConfig  out[3];       // A, B, C; out[1]/out[2] ignored for channels 0 and 1
};
```

### Delivers

A new AVR-header-free translation unit -- proposed
`lib/MultiOutput/src/PwmChannelConfig.h` + `.cpp` (keeps it next to `PWM`
and inside a lib dir the native runner can already reach; a standalone
`lib/PwmCommand/` is fine too if that's cleaner for the allowlist).

Public surface:

1. **`bool pwmChannelConfigValid(const PwmChannelConfig& c)`**
   - `c.channel <= 3`
   - `c.f_selector < NUMBER_OF_PWM_FREQUENCIES` (i.e. 0-13; reject 14+)
   - returns `false` on any violation. Does **not** mutate.

2. **`bool pwmChannelIsComplex(uint8_t channel)`** -- `channel == 2 || channel == 3`.

3. A resolved-arguments producer. Because the simplex and complex
   `setupPWMChannelN` signatures differ, return a tagged struct rather
   than trying to unify the call:

   ```c
   struct PwmResolvedArgs {
       bool           is_complex;
       PWMFrequency   f_selector;
       uint16_t       frequency;
       // simplex (is_complex == false): only [0] used
       // complex (is_complex == true):  [0]=A [1]=B [2]=C
       bool           enabled[3];
       bool           inverting[3];
       uint16_t       duty_percent[3];   // clamped 0-100
   };
   PwmResolvedArgs pwmResolveArgs(const PwmChannelConfig& c);
   ```

   Rules baked in here:
   - `duty_percent > 100` -> clamped to `100`.
   - simplex channel -> `out[1]`/`out[2]` are not copied; `enabled[1..2]`
     / `inverting[1..2]` left `false`, `duty_percent[1..2]` left `0`.
   - `frequency` passed straight through (it's the caller's ICRn TOP;
     only the `PWM` layer / `pwmResolutionTop` interprets it).
   - `pwmResolveArgs` assumes a valid config -- callers must gate on
     `pwmChannelConfigValid` first. Document that.

   The actual `pwm.setupPWMChannel0/1/2/3(...)` call is **not** in this
   task -- it's a thin switch in `main.cpp` (task 3) fed by
   `PwmResolvedArgs`. This unit stays free of the `PWM` class (whose
   `.cpp` names real SFRs and is AVR-only).

### Tests

`test_native/test_pwm_channel_config.cpp`, added to the allowlists near
the top of `test_native/run_tests.ps1` (`$libAllowlist` for the include
dir, `$prodSourceAllowlist` for `PwmChannelConfig.cpp`). Mirror the
structure of the existing `test_pwm_state_machine.cpp`.

Cases:
- valid simplex ch0 and ch1 -> `is_complex == false`, output A mapped.
- valid complex ch2 and ch3 -> `is_complex == true`, A/B/C mapped.
- `channel == 4` -> `pwmChannelConfigValid` false.
- `f_selector == NUMBER_OF_PWM_FREQUENCIES` (14) -> invalid.
- `f_selector == 13` (`frequency_variable`) -> valid; `frequency` value
  survives into `PwmResolvedArgs` unchanged.
- `duty_percent == 150` -> resolved to `100`.
- simplex channel with `out[1].enabled == true` -> ignored
  (`enabled[1] == false` in the result).

## Dependencies

Only `PWMFrequency` / `NUMBER_OF_PWM_FREQUENCIES` from
`lib/MultiOutput/src/PWMTiming.h`. No dependency on task 1: the POD struct
is defined here, and task 3 copies the MAVLink fields into it.

## Pre-work

Create the empty `test_pwm_channel_config.cpp` and wire the allowlist
entries before implementing -- this is planning setup, no product logic.
Confirm `run_tests.ps1` still runs (existing tests green) with the new
allowlist lines pointing at not-yet-existing symbols commented out, then
fill in.

## Definition of done

- `test_native/run_tests.ps1` passes including the new cases.
- `platformio run` still builds (the new TU compiles for AVR too, even
  though nothing references it until task 3).
- Task file marked done in the same commit.

## Notes

- Conventional-commit: `feat(pwm): pure PwmChannelConfig -> setup-args mapping`.
- Keep `PwmChannelConfig.h` includable from plain C++ with no Arduino /
  AVR headers -- that property is the whole point.
