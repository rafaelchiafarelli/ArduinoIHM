# Task 2: board-apply-and-report

**Status:** done (2026-09-27) -- host-verified: 174 native tests pass, `platformio run` RAM 59.8 % -> 61.1 % (+108 B: live SerialConfig + 4-deep setting queue). Bench check (pymavlink on COM3) pending: run with the initiative's bench pass before `serial_config` merges into `features`.
**Depends on:** task 1, `config_model/1`

## Contract

1. `MavlinkComms` stores `IHM_SERIAL_SETTING` like 301/302 (storage only, per the layering rule in
   `mavlink/README.md`).
2. `main.cpp` applies 301/302 (and the extension message) into the live
   `SerialConfig` (replacing today's direct copy into
   `bus_status_instance`) and refreshes the SERIAL bindings from it.
   Extension keys with no owner yet are rejected, not stored.
3. The board sends the config-state telemetry, one bus per telemetry
   tick, within the 128-byte TX ring budget.
4. Last writer wins (initiative question 6).
