# Task 2: board-apply-and-report

**Status:** planned
**Depends on:** task 1, `config_model/1`

## Contract

1. `MavlinkComms` stores the new PC -> board message (if Q7 chose the
   generic pair) like 301/302 (storage only, per the layering rule in
   `mavlink/README.md`).
2. `main.cpp` applies 301/302 (and the extension message) into the live
   `SerialConfig` (replacing today's direct copy into
   `bus_status_instance`) and refreshes the SERIAL bindings from it.
   Extension keys with no owner yet are rejected, not stored.
3. The board sends the config-state telemetry, one bus per telemetry
   tick, within the 128-byte TX ring budget.
4. Conflict rule per open question 6 (proposal: last writer wins).
