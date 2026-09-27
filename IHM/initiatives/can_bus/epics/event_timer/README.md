# Epic: event_timer

The one time source for the serial part (strictly event-driven rule):
periods and timeouts are one-shot hardware compare interrupts, never
checked deadlines. Shared with `rs485_modbus` (its 1.5/3.5-character
silence timer).

## Tasks

```
1-one-shot-compare-queue   a hardware compare channel + a sorted software timer queue; arm/cancel from ISR or superloop   (blocked: Q8)
```

## Acceptance gate

Native tests for the queue (ordering, cancel, re-arm from a callback,
wrap of the time base); bench: a 1.75 ms one-shot measured on a scope
pin is within one timer step, and a 100 ms periodic re-arm shows no drift
over 60 s.
