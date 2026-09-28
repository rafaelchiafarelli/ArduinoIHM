# Epic: slave_register_map

What the board exposes over Modbus and how writes land.

## Tasks

```
1-map-document     the register map: addresses, types, scaling, access, as docs/MODBUS_MAP.md   (blocked: Q3)
2-read-and-write   callbacks: snapshots for reads; queued writes applied by the superloop      (deps: 1, rtu_framing/2; Q2, Q5)
3-bus-config-regs  serial_config settings as holding registers, with a commit rule             (deps: 2, rs485_modes/1, serial_config/config_model; blocked: Q6)
```

## Acceptance gate

A pymodbus bench script (`mavlink/scripts`-style, e.g.
`scripts/modbus_probe.py`) reads every block and writes a relay, a PWM
channel and a bus setting; each shows on the TFT, in the companion, and in
the MAVLink telemetry. MAVLink and Modbus writes follow last-writer-wins.
