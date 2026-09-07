# Task 3: pwm-config-receive-apply

**Status:** not started
**Branch:** `3-pwm-config-receive-apply` (from `tasks`)
**Depends on:** task 1 (message + generated header), task 2 (`PwmChannelConfig` + validation + `pwmResolveArgs`)

## Contract

### In

- `MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG` / `mavlink_pwm_channel_config_t`
  from task 1's regenerated headers.
- `PwmChannelConfig`, `pwmChannelConfigValid`, `pwmResolveArgs`,
  `PwmResolvedArgs` from task 2.

### Delivers

1. **`lib/MavlinkComms/src/MavlinkComms.h`** -- storage-only, matching the
   existing `canConfig` / `canConfigValid` pattern exactly:

   - members: `mavlink_pwm_channel_config_t pwmConfig[4];`
     `bool pwmConfigDirty[4];` (init all `false` in the ctor init list
     alongside `canConfigValid{false,false}`).
   - in `dispatch()`: new `case MAVLINK_MSG_ID_PWM_CHANNEL_CONFIG` --
     decode into a local, and `if (cfg.channel < 4) { pwmConfig[cfg.channel] = cfg; pwmConfigDirty[cfg.channel] = true; }`.
   - `const mavlink_pwm_channel_config_t* getPwmChannelConfig(uint8_t ch) const`
     -- `nullptr` if `ch >= 4`.
   - `bool consumePwmConfigDirty(uint8_t ch)` -- returns the flag and
     clears it (`if (ch >= 4) return false;` first). Non-const.

   Do **not** `#include` `PWM.h`, `MultiOutput.h`, or any Janus header
   here. This class keeps owning only the protocol.

2. **`src/main.cpp`** -- in the superloop, right after
   `mavlinkComms.poll();` (before the `newDataAvailable` block is fine):

   ```cpp
   for (uint8_t ch = 0; ch < 4; ch++) {
       if (!mavlinkComms.consumePwmConfigDirty(ch)) continue;
       const mavlink_pwm_channel_config_t* m = mavlinkComms.getPwmChannelConfig(ch);
       PwmChannelConfig cfg = /* copy m->channel, m->f_selector, m->frequency,
                                 m->out{1,2,3}_{enabled,inverting,duty_percent} */;
       if (!pwmChannelConfigValid(cfg)) continue;   // silently drop bad configs
       PwmResolvedArgs a = pwmResolveArgs(cfg);
       switch (ch) {
           case 0: pwm.setupPWMChannel0((PWMFrequency)a.f_selector, a.inverting[0], a.enabled[0], a.frequency, a.duty_percent[0]); break;
           case 1: pwm.setupPWMChannel1((PWMFrequency)a.f_selector, a.inverting[0], a.enabled[0], a.frequency, a.duty_percent[0]); break;
           case 2: pwm.setupPWMChannel2((PWMFrequency)a.f_selector, a.frequency,
                       a.inverting[0], a.enabled[0], a.inverting[1], a.enabled[1], a.inverting[2], a.enabled[2],
                       a.duty_percent[0], a.duty_percent[1], a.duty_percent[2]); break;
           case 3: pwm.setupPWMChannel3(/* same as case 2 */); break;
       }
   }
   ```

   Uses the existing `PWM pwm;` global (`main.cpp:60`). No UI mirroring in
   this task -- that is task 5.

3. **`demo/HARDWARE_RUNBOOK.md`** -- new top-level section
   "Driving PWM over MAVLink":
   - prerequisite: board flashed with this firmware, connected on the
     debug COM port; `mavlink/scripts/pwm_config.py` (task 4).
   - one worked example per channel shape (a simplex channel at a fixed
     frequency; a complex channel with two outputs at different duties),
     with the `pwm_config.py` invocation and what to see on
     OC3A / OC1A+OC1B (cross-ref the existing "Pin reference" table).
   - note that this drives the same registers as the on-screen PWM tab,
     so on-screen and over-serial config fight over the same channel --
     last writer wins (until task 5, the TFT won't show serial-driven
     changes).

## Dependencies

Task 1 and task 2, both merged into `tasks` before this branch starts.
If either is not yet in `tasks`, stop -- do not stub the message type or
re-derive the mapping here.

## Pre-work

None.

## Definition of done

- `platformio run` (env `megaatmega2560`) builds. Record Flash/RAM from
  the build output in the commit message; RAM was ~75.8% before this
  epic -- flag in the commit if it moves more than a couple of points.
- `test_native/run_tests.ps1` passes (task 2's tests still green;
  nothing new to test here -- the mapping is task 2's, the switch is
  thin glue).
- Runbook section added.
- Task file marked done in the same commit.
- Bench verification (scope shows the configured waveform) is Rafael's
  step -- note it in the report, do not block the task on it. The COM7
  board is an old hw revision (see `IHM/NEXT-SESSION.md`).

## Notes

- `MavlinkComms::poll()` drains its whole RX ring in one unbounded
  `while (serial->available())` -- known issue in `IHM/NEXT-SESSION.md`,
  **not** in scope here.
- Conventional-commits, one per logical change:
  `feat(mavlink): decode+store PWM_CHANNEL_CONFIG` then
  `feat(pwm): apply MAVLink PWM configs to the timers` then
  `docs(runbook): PWM-over-MAVLink bench procedure`.
