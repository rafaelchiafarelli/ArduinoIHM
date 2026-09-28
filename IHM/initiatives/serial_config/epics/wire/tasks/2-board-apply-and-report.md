# Task 2: board-apply-and-report

**Status:** done (2026-09-27) -- bench-verified 2026-09-28
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

## Bench (2026-09-28, old board revision: flashed via COM7, MAVLink on COM3)

Script checks, all passed: 311/312 stream at 10/s (one bus per tick); a
301 is applied and read back; an out-of-range 301 (standard ID 0x800) is
dropped; a 313 is answered by 314 "unknown key"; RE0/RE1/RE2 (simulated)
reach the CAN0 ID field, one slow click is +1, 12 fast clicks (40 ms
apart) give +836 (acceleration); after a reset (COM7 DTR) the
toggle-saved config loads and the later un-toggled knob edit is gone.
Companion driven headlessly (WM_COMMAND / WM_SETTEXT): connects, fills
all three readback lines, a panel Send reads "Board applied CAN1", bad
input is refused locally, Set shows the 314 answer, a board knob edit
shows in the panel. Not checked: the mirror's SERIAL tab by pixel (same
handler as the readback lines; the mapping is unit-tested), and the knob
feel by hand (clicks were simulated).
