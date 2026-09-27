# Epic: wire

The PC <-> board messages for everything `config_model` holds. The
generator part already travels in `CAN_SIGNAL_CONFIG` (301) and
`RS485_SIGNAL_CONFIG` (302); bus parameters, protocol mode and a
board -> PC readback don't exist yet.

## Tasks

```
1-dialect                 new ids for bus params (PC->board) and config state (board->PC)   (deps: config_model/1)
2-board-apply-and-report  MavlinkComms storage + main.cpp apply to SerialConfig, round-robin report   (deps: 1; Q6)
```

## Acceptance gate

Native suite passes; pymavlink on COM3 sees the new telemetry and a sent
bus-params message changes it.
