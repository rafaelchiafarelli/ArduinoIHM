# Epic: wire

The PC <-> board messages for everything `config_model` holds. The
generator part already travels in `CAN_SIGNAL_CONFIG` (301) and
`RS485_SIGNAL_CONFIG` (302); a board -> PC readback doesn't exist yet,
and neither does a way for bus initiatives to carry their own settings
(initiative open question 7).

## Tasks

```
1-dialect                 config state (board->PC) + the extension mechanism   (deps: config_model/1; blocked: Q7)
2-board-apply-and-report  MavlinkComms storage + main.cpp apply to SerialConfig, round-robin report   (deps: 1; Q6)
```

## Acceptance gate

Native suite passes; pymavlink on COM3 sees the new telemetry, and a
301/302 sent from the PC changes it.
