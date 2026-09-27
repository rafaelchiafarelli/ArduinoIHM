# Task 1: config-struct-and-steps

**Status:** planned -- blocked on initiative open question 5 (value lists)
**Depends on:** nothing

## Contract

1. `lib/BusConfig/src/SerialConfig.h/.cpp` (new, host-safe): a
   `SerialConfig` struct holding, per CAN bus (0, 1): enabled, bitrate
   (enum), protocol mode (enum), J1939 source address, and the generator
   (id, extended, dlc, data[8], period_ms, repeat_count); for RS-485:
   enabled, baud (enum), parity (enum), and the generator (length,
   data[32], period_ms, repeat_count).
2. Validation/clamping that matches the existing 301/302 message ranges
   (DLC 0-8, standard ID <= 0x7FF, extended <= 0x1FFFFFFF, length 0-32).
3. RE2 step functions per field kind, like `pwmStepDuty`: numeric step
   with clamp, enum cycle, toggle.
4. Conversions to and from `mavlink_can_signal_config_t` /
   `mavlink_rs485_signal_config_t` (the generator part).
5. Native tests for 2-4; the lib added to `run_tests.ps1`'s allowlist.
