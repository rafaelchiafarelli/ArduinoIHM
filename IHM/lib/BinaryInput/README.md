# BinaryInput

`BinaryInputs` (`.cpp` just includes the header) reads 15 digital inputs
across 5 AVR ports and packs them into one `uint16_t bMap`: bit 15 is
always set (sentinel, lets consumers distinguish "no input has ever been
read" from "all zero"), bits 0-14 are the live pin states.

This is the **single source of truth for pin-index-to-physical-pin
mapping** that [RotaryEncoder](../RotaryEncoder/README.md) and
`ButtonMap` both hardcode bit positions against (documented in comments
in both those files) without holding a live reference to this class.

## Index table (`BinaryInput.h:71-87`)

| bMap bit | AVR pin | Role |
|---|---|---|
| 0 | PA4 | push-button |
| 1 | PE4 | push-button |
| 2 | PF5 | push-button |
| 3 | PF6 | push-button |
| 4 | PA0 | house-keeping |
| 5 | PA2 | house-keeping |
| 6 | PL6 | rotary-encoder |
| 7 | PL5 | rotary-encoder |
| 8 | PL4 | push-button |
| 9 | PL2 | rotary-encoder |
| 10 | PL1 | rotary-encoder |
| 11 | PL0 | push-button |
| 12 | PC4 | rotary-encoder |
| 13 | PC5 | rotary-encoder |
| 14 | PC7 | push-button |
| 15 | -- | sentinel, always 1 |

## API

- `fast_handler()` -- snapshots all 5 ports' `PIN` registers, then packs
  each of the 15 logical pins into `bMap` via `get_pin()`. Called every
  tick inside `ISR(TIMER2_COMPA_vect)` -- the true ~1ms fast path (unlike
  `Comms`/`RotaryEncoder`, which despite similar naming actually run on
  the 25ms branch).
- `get_pin(uint8_t index) const` -- decodes one logical pin from the
  cached port snapshot.

## Hardware resources

15 GPIO input pins across `PORTA` (PA0,PA2,PA4), `PORTC` (PC4,PC5,PC7),
`PORTE` (PE4), `PORTF` (PF5,PF6), `PORTL` (PL0,PL1,PL2,PL4,PL5,PL6).
Configured as inputs with pull-ups in the constructor via the
[Ports.h](../Ports/README.md) `port_type` table (`DDRx` cleared, `PORTx`
bits set).

## Pull-up bit fix (already applied)

The constructor also does:

```cpp
MCUCR &= ~(1<<PUD);
```

This clears just the `PUD` bit, matching the comment's intent ("ensure
pull-ups aren't globally disabled"). An earlier version used
`MCUCR |= ~(1<<PUD);`, which set every *other* `MCUCR` bit while leaving
`PUD` itself untouched -- fixed in `39bf816`.
