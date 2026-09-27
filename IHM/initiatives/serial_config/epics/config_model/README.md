# Epic: config_model

The pure model behind every SERIAL setting, with no AVR, UI or MAVLink
dependencies, so it is native-testable like `PWMWireConfig`.

## Tasks

```
1-config-struct-and-steps   SerialConfig fields, ranges, RE2 step/cycle rules   (blocked: Q5)
2-eeprom-image              versioned + CRC EEPROM image, load/save, defaults    (deps: 1; blocked: Q4)
```

## Acceptance gate

`test_native/run_tests.ps1` passes with the new tests; `platformio run` builds.
