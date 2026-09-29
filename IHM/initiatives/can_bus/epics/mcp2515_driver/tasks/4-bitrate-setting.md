# Task 4: bitrate-setting

**Status:** planned -- blocked on can_bus open question 9 (values, default)
**Depends on:** task 1 (`serial_config`'s framework is done: `lib/BusConfig`)

## Contract

The CAN bitrate per bus, added to `SerialConfig` through the extension
recipe (`lib/BusConfig/README.md`), end to end:

1. **Model:** a bitrate enum in each CAN bus's section of `SerialConfig`
   (values and default per open question 9), with validation and an RE2
   enum-cycle rule.
2. **EEPROM:** appended per `config_model/2`'s append-only rule.
3. **Wire:** a key in the `IHM_SERIAL_SETTING` key table
   (`mavlink/README.md`), read back by `IHM_SERIAL_SETTING_STATE`.
4. **Screen:** in each CAN row per the row rule
   (`lib/GUI/bus_status.screen.yaml` header), shown as text ("250k");
   when a change takes effect per can_bus open question 9.
5. **Companion:** in the SERIAL panel's CAN sections and the mirror
   bindings.
6. **Apply:** on the change event, the module is re-initialised with
   task 1's `mcp2515BitTiming` for the new rate (TX queue flushed, error
   state reset). Never re-checked from the superloop.
7. Native tests for the model, EEPROM append and wire mapping.
