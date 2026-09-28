# Task 1: settings

**Status:** planned -- blocked on open questions 4 (defaults), 8 (format field) and 9 (optional settings)
**Depends on:** `serial_config` (all epics: the framework and its extension contract)

## Contract

The RS-485 settings added to `serial_config` through its extension
contract (`initiatives/serial_config/README.md`), end to end:

1. **Model:** in `SerialConfig`'s RS-485 section, with defaults per open
   question 4, validation and RE2 step rules:
   - **mode**: off / raw generator / Modbus slave (enum; master later);
   - **baud**: 9600 / 19200 / 38400 / 57600 / 115200 (enum);
   - **format** per open question 8;
   - **slave address**: 1-247 (0 = broadcast and 248-255 reserved, never
     valid as our own address);
   - whatever open question 9 adds (write protect, response delay).
2. **EEPROM:** appended per `config_model/2`'s append-only rule.
3. **Wire:** per `serial_config`'s open question 7 (keys or messages).
4. **Screen:** the RS-485 row per `serial_config`'s row layout rule,
   mode-dependent: in Modbus mode it shows mode, baud, format and
   address; in raw mode the generator fields stay. Baud, format and
   address apply on leaving the field (`serial_config` Q3), not on every
   RE2 step, so sweeping a value doesn't reconfigure the port each step.
5. **Companion:** the fields in the SERIAL panel's RS-485 section and the
   mirror bindings.
6. **Apply:** stored only in this task. Task 2 makes the mode and port
   settings act on USART3.
7. Native tests for the model, EEPROM append and wire mapping.
