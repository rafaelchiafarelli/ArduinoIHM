# Task 1: settings

**Status:** planned -- blocked on open questions 4 (defaults), 8 (format field), 9 (optional settings) and 10 (when port settings apply)
**Depends on:** nothing left (`serial_config`'s framework is done: `lib/BusConfig`)

## Contract

The RS-485 settings added to `SerialConfig` through the extension recipe
(`lib/BusConfig/README.md`), end to end:

1. **Model:** in `SerialConfig`'s RS-485 section, with defaults per open
   question 4, validation and RE2 step rules:
   - **mode**: off / raw generator / Modbus slave (enum; master later);
   - **baud**: 9600 / 19200 / 38400 / 57600 / 115200 (enum);
   - **format** per open question 8;
   - **slave address**: 1-247 (0 = broadcast and 248-255 reserved, never
     valid as our own address);
   - whatever open question 9 adds (write protect, response delay).
2. **EEPROM:** appended per `config_model/2`'s append-only rule.
3. **Wire:** keys in the `IHM_SERIAL_SETTING` key table
   (`mavlink/README.md`), read back by `IHM_SERIAL_SETTING_STATE`.
4. **Screen:** the RS-485 rows per the row rule
   (`lib/GUI/bus_status.screen.yaml` header), mode-dependent: in Modbus
   mode they show mode, baud, format and address; in raw mode the
   generator fields stay. When a port setting takes effect per open
   question 10.
5. **Companion:** the fields in the SERIAL panel's RS-485 section and the
   mirror bindings.
6. **Apply:** stored only in this task. Task 2 makes the mode and port
   settings act on USART3.
7. Native tests for the model, EEPROM append and wire mapping.
