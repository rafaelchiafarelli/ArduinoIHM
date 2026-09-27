# Epic: rtu_framing

The protocol layer with no hardware: frames in, frames out.

## Tasks

```
1-crc-and-frames   CRC-16/MODBUS, frame end by silence, address filter, request parse, response/exception build   (no deps)
2-slave-engine     request -> handler callbacks -> response, run from the silence-timer ISR, bounded   (deps: 1; Q2)
```

## Acceptance gate

Native tests against known-good frames (e.g. from the Modbus spec and
from pymodbus output), including CRC errors, wrong address, broadcast
(address 0: no reply) and every exception code used.
