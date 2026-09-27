# Task 3: bus-config-regs

**Status:** planned -- blocked on open question 6 (commit rule)
**Depends on:** task 2, `serial_config`'s `config_model/1`

## Contract

1. The `SerialConfig` fields as holding registers (CAN generator + bus
   parameters + protocol mode; RS-485 settings), using `config_model`'s
   validation.
2. The commit rule from open question 6, so a Modbus master can't cut
   its own link by accident.
3. Changes persist per `serial_config`'s save policy.
