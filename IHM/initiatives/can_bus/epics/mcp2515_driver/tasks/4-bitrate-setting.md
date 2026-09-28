# Task 4: bitrate-setting

**Status:** planned -- blocked on can_bus open question 9 (values, default)
**Depends on:** task 1, `serial_config` (all epics: the framework and its extension contract)

## Contract

The CAN bitrate per bus, added to `serial_config` through its extension
contract (`initiatives/serial_config/README.md`), end to end:

1. **Model:** a bitrate enum in each CAN bus's section of `SerialConfig`
   (values and default per open question 9), with validation and an RE2
   enum-cycle rule.
2. **EEPROM:** appended per `config_model/2`'s append-only rule.
3. **Wire:** per `serial_config`'s open question 7 (key or message).
4. **Screen:** in each CAN row per `serial_config`'s row layout rule,
   shown as text ("250k"); applied on leaving the field
   (`serial_config` Q3), not on every RE2 step.
5. **Companion:** in the SERIAL panel's CAN sections and the mirror
   bindings.
6. **Apply:** on the change event, the module is re-initialised with
   task 1's `mcp2515BitTiming` for the new rate (TX queue flushed, error
   state reset). Never re-checked from the superloop.
7. Native tests for the model, EEPROM append and wire mapping.
