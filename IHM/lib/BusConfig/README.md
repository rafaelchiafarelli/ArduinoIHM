# BusConfig

The settings behind the SERIAL tab (initiative `serial_config`): one
`SerialConfig` with a section per bus (CAN0, CAN1, RS-485). Pure C++, no
AVR/UI/MAVLink dependency in the model itself, so it is host-tested
(`test_native/test_serial_config.cpp`).

| File | What it holds |
|---|---|
| `src/SerialConfig.h/.cpp` | The model (`CanBusConfig`, `Rs485BusConfig`, their `gen` generator sections), defaults, range checks, RE2 step helpers (`serialStepValue`, `serialStepEnum`, `serialSetFlag`) and the digit-at-a-time acceleration (`DigitAccel`). |
| `src/SerialConfigWire.h/.cpp` | Adapters to and from `CAN_SIGNAL_CONFIG` (301) and `RS485_SIGNAL_CONFIG` (302). |

This initiative ships the signal-generator settings only. Bus parameters
and protocol settings (CAN bitrate, RS-485 mode/baud/format/address,
protocol modes) belong to the initiative that uses them.

## RE2 editing

Every value is its own RE1-focusable field; RE2 changes the focused one.
Numeric fields step by `unit * digitAccelStep(...)`:

- a step starts at one unit;
- `DIGIT_ACCEL_FAST_RUN` (4) clicks in a row less than
  `DIGIT_ACCEL_FAST_MS` (80 ms) apart raise it one digit (x `base`);
- a pause of `DIGIT_ACCEL_DROP_MS` (300 ms) lowers it one digit;
- `DIGIT_ACCEL_RESET_MS` (1.5 s), or `digitAccelReset()` on a focus
  change, goes back to one unit;
- `maxLevel` caps it at the field's top digit.

RE2 is sampled every 25 ms with one pending direction
(`lib/RotaryEncoder`), so ~15-20 clicks/s is the practical ceiling.

## Adding a setting (extension recipe)

A bus initiative adds a setting in one task of its own:

1. **Model:** a field in its bus's section (`CanBusConfig` /
   `Rs485BusConfig`, after `gen`), a default in `serialConfigDefaults`,
   a range check, and an RE2 rule built from the step helpers above.
2. **EEPROM:** append the field to the image. Fields past the stored
   payload length load as their defaults, so older saved settings are
   kept.
3. **Wire:** a key in the `IHM_SERIAL_SETTING` key table
   (`mavlink/README.md`); no new message.
4. **Screen:** the field in its bus's rows on the SERIAL tab
   (`lib/GUI/bus_status.screen.yaml` header comment has the row rule).
5. **Companion:** the field in the SERIAL panel's bus section and the
   mirror bindings.
6. **Apply:** on the change event (edit, MAVLink, Modbus), calling the
   owning driver. Never re-check the config from the superloop.
