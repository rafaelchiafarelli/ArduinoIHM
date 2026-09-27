# Epic: mcp2515_driver

A driver for both CAN modules. Pure parts (bit timing, frame ring,
filter maths) are host-tested; register access goes through
`spi_sharing`.

## Tasks

```
1-bit-timing-and-init   CNF1-3 from crystal + bitrate, reset, modes, loopback test   (deps: spi_sharing/1; blocked: Q1)
2-int-driven-io         INT ISR: RX handed to a per-bus consumer callback, TX-complete sends the next queued frame, errors   (deps: 1; Q4, Q7)
3-tx-and-filters        TX via the 3 buffers, acceptance filters/masks, error counters   (deps: 1)
```

## Acceptance gate

Native tests (bit timing for 125/250/500/1000k at the confirmed crystal;
ring wrap/overflow); bench: loopback mode on both modules, then CAN-1 <->
CAN-2 wired together exchange frames at 250 and 500 kbit/s with no loss
at a sustained rate the gate records.
