# Task 3: bus-config-regs

**Status:** planned -- blocked on open question 6 (commit rule)
**Depends on:** task 2, `rs485_modes/1` (RS-485 settings)

## Contract

1. The `SerialConfig` fields as holding registers: the generator fields
   (`serial_config`), the RS-485 settings (`rs485_modes/1`), and any CAN
   bus/protocol settings that exist by then (added by `can_bus`), each
   using its owner's validation (`lib/BusConfig`).
2. The commit rule from open question 6, so a Modbus master can't cut
   its own link by accident.
3. Changes persist per the SERIAL save rule (EEPROM written when a bus's
   enable switch toggles), unless open question 6's commit rule says
   otherwise.
