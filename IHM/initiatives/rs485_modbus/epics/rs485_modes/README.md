# Epic: rs485_modes

RS-485 runs in exactly one mode: off, raw generator, or Modbus slave. This
epic also owns the RS-485 settings (added to `serial_config` through its
extension contract).

## Tasks

```
1-settings                    RS-485 mode, baud, format, slave address (+ Q9) as SerialConfig extension: model, EEPROM, wire, screen, companion   (deps: serial_config; blocked: Q4, Q8, Q9)
2-mode-switch-and-generator   mode selects the USART3 owner; raw generator (302) on Serial3; Modbus slave on/off; port params applied   (deps: 1, uart3_driver/1, slave_register_map/2)
```

## Acceptance gate

Bench: the RS-485 settings edited from the board and the companion show on
both and survive a power cycle; switching the mode (from the board, the
companion, or Modbus if allowed) changes what's on the wire, and nothing
from the other mode leaks; the raw generator sends
`RS485_SIGNAL_CONFIG`'s bytes at its period.
