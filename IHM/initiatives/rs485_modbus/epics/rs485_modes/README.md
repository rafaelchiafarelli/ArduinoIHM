# Epic: rs485_modes

RS-485 runs in exactly one mode: off, raw generator, or Modbus slave.

## Tasks

```
1-mode-switch-and-generator   mode from SerialConfig; raw generator (302) on Serial3; Modbus slave on/off; address/baud/parity   (deps: uart3_driver/1, slave_register_map/2, serial_config config_model/1; blocked: Q4)
```

## Acceptance gate

Bench: switching the mode (from the board, the companion, or Modbus if
allowed) changes what's on the wire, and nothing from the other mode
leaks; the raw generator sends `RS485_SIGNAL_CONFIG`'s bytes at its period.
