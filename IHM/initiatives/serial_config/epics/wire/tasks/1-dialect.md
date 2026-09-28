# Task 1: dialect

**Status:** done (2026-09-27) -- 174 native tests pass (5 new); `platformio run` RAM 59.3 % -> 59.8 % (+40 B, MAVLink CRC table entries).
**Depends on:** `config_model/1`

## Contract

1. `mavlink/ihm_dialect.xml`: `IHM_CAN_SIGNAL_STATE` (311) and
   `IHM_RS485_SIGNAL_STATE` (312), board -> PC, the generator config
   applied to one bus, same fields as 301/302 (as 307 mirrors 303). PC ->
   board generator settings stay on 301/302. 310 is left free for
   `non_blocking_redraw`'s proposed `IHM_LOOP_STATS`.
2. The generic extension pair (initiative question 7):
   `IHM_SERIAL_SETTING` (313, PC -> board: bus, key, int32 value) and
   `IHM_SERIAL_SETTING_STATE` (314, board -> PC: bus, key, value, status),
   with the (empty) key table documented in `mavlink/README.md` for bus
   initiatives to add to.
3. Regenerate `mavlink/generated` and `generated_py`; update
   `mavlink/README.md` (message table, TX budget per tick).
4. Native pack/decode round-trip tests.
