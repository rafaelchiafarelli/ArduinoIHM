# Task 1: config-struct-and-steps

**Status:** planned -- blocked on initiative open question 5 (generator value steps)
**Depends on:** nothing

## Contract

1. `lib/BusConfig/src/SerialConfig.h/.cpp` (new, host-safe): a
   `SerialConfig` struct with one section per bus, holding the generator
   only: per CAN bus (0, 1) enable, id, extended, dlc, data[8],
   period_ms, repeat_count; for RS-485 enable, length, data[32],
   period_ms, repeat_count. Bus parameters and protocol settings are
   **not** here; bus initiatives add them through the extension contract
   (initiative README).
2. Validation/clamping that matches the existing 301/302 message ranges
   (DLC 0-8, standard ID <= 0x7FF, extended <= 0x1FFFFFFF, length 0-32).
3. RE2 step helpers per field kind, like `pwmStepDuty`: numeric step
   with clamp, enum cycle, toggle. Generic enough that an extension
   field reuses them instead of writing its own.
4. Conversions to and from `mavlink_can_signal_config_t` /
   `mavlink_rs485_signal_config_t` (the generator part).
5. Native tests for 2-4; the lib added to `run_tests.ps1`'s allowlist.
6. `lib/BusConfig/README.md`: the extension recipe from the initiative
   README, pointing at the real names in the code.
