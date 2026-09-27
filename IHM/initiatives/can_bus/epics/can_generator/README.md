# Epic: can_generator

`CAN_SIGNAL_CONFIG` (301) is stored on the board today but nothing
transmits it. This epic makes the per-bus generator real.

## Tasks

```
1-periodic-tx   each period is an event_timer expiry that queues the frame; no deadline checks   (deps: event_timer/1, mcp2515_driver/2, mcp2515_driver/3)
```

## Acceptance gate

Bench: a config sent from the companion or pymavlink appears on the bus
(the other module or a USB-CAN adapter) at the set period and count, and
`enable = 0` stops it.
