# Task 1: mode-switch-and-generator

**Status:** planned -- blocked on open question 4 (defaults)
**Depends on:** `uart3_driver/1`, `slave_register_map/2`, `serial_config`'s `config_model/1`

## Contract

1. The RS-485 mode from `SerialConfig` selects what owns USART3: nothing,
   the raw generator, or the Modbus slave engine.
2. Raw generator, event-driven: an `event_timer` expiry every
   `period_ms` starts one TX of 302's `data[0..length)` via the UDRE ISR,
   `repeat_count` times (0 = until disabled). No deadline checks.
3. Modbus slave: address, baud and parity from `SerialConfig`; defaults
   per open question 4.
4. A mode or parameter change re-initialises USART3 cleanly (TX drained,
   DE released).
