# BinaryOutputs

Register-direct GPIO output driver: a fixed, compile-time table of 20 pin
descriptors, each set/cleared with a single register write. This is the
mechanical pattern every other output module builds on ("the
register-direct strategy" elsewhere in these docs), but as of 2026-08-12,
**most of its 20 slots are not actually meant to be driven this way** --
see "What the 20 indices actually are" below before using this class for
anything new. As of 2026-08-13, indices 0-7 (and the bus's control lines,
10/11/14/15) are meant to be driven through
[`MultiplexedBus`](../MultiOutput/README.md), which uses `SetOutput()` as
its own low-level primitive -- `BinaryOutputs::SetOutput()` itself is
unchanged, still a single independent register write with no bus/strobe
awareness.

## API

- `BinaryOutputs()` -- constructs the table (`port_type`, see
  [Ports](../Ports/README.md)) and calls `setup()`.
- `setup() const` -- drives every pin's `DDR` bit high (output) and its
  `PORT` bit low (off).
- `SetOutput(uint8_t i, bool v) const` -- sets or clears output `i`
  (0..19) via a single `*port |= mask` / `*port &= ~mask`, immediately,
  independently, with no coordination between indices.

## What the 20 indices actually are (corrected 2026-08-12)

An earlier version of this README described indices 0-19 as independent
GPIO output pins, each freely assignable to whichever module needed one.
Checked against the user's `IOs IHM.xlsx` and confirmed directly with the
user: that's wrong for indices 0-7 and 8-19 alike, just in different ways.

- **Indices 0-7** (`PC2,PC1,PC0,PD7,PG2,PG1,PG0,PL7`) are the physical
  pins of a **shared 8-bit data bus**, not eight independent outputs.
  `Relay`, `ServoMotor`, and `MotorDC` each sit behind their own
  `74LS373` latch fed by this same bus, captured by a per-device strobe
  line (`dig_0`/`dig_1`/`dig_2`, none of which are in this table -- see
  below). `SetOutput()`'s immediate independent-bit-write model **does
  not implement this protocol** on its own (no bus-settle-then-strobe
  sequence) -- calling it directly on indices 0-7 will not correctly
  drive Relay, Servo, or Motor hardware. As of 2026-08-13,
  [`MultiplexedBus`](../MultiOutput/README.md) is the real driver for
  this protocol, built as a thin layer on top of `SetOutput()` (it calls
  `SetOutput()` once per bit to settle the bus, then again on the target
  device's strobe index); `Relay` uses it, `ServoMotor`/`MotorDC` don't
  yet (`IHM/NEXT-SESSION.md` items 1/2).
- **Indices 8, 9, 12, 13, 16, 17, 18, 19** (`PB7,PB6,PH5,PH4,PE3,PH3,PB5,PL3`)
  are hardware PWM-capable pins (`OC1C/OC1B/OC4C/OC4B/OC3A/OC4A/OC1A/OC5A`),
  direct-to-output with no buffer, exclusively owned by
  [`PWM`](../MultiOutput/README.md). They should never be driven through
  `BinaryOutputs::SetOutput()` -- doing so would fight the timer hardware
  actually generating PWM on those pins.
- **Indices 10, 11, 14, 15** (`PH6,PB4,PG5,PF4`) are the multiplexed bus's
  control lines (`dig_0`, `OUTPUT_EN`, `dig_1`, `dig_2`) -- see
  `IHM/ARCHITECTURE.md`. They need the specific settle-then-strobe timing
  described there, not generic `SetOutput()` calls.

In short: as of this hardware revision, none of `BinaryOutputs`' 20
indices are appropriate for a caller to drive directly with its own
`SetOutput()` one-index-at-a-time model. The class's register table
(which physical pin each index is) is still accurate, and `SetOutput()`
is still the correct low-level primitive -- but indices 0-7 and 10/11/14/15
should only be reached through [`MultiplexedBus`](../MultiOutput/README.md)
now, and indices 8, 9, 12, 13, 16-19 should never be driven through
`BinaryOutputs` at all (owned by `PWM`).

## Hardware resources

20 pins across `PORTB/C/D/E/F/G/H/L`, hardcoded in the `ports{}`
initializer list in `BinaryOutputs.h:27-48`. Index-to-physical-pin mapping
is fixed at compile time; there is no runtime pin configuration.

## Depends on

[Ports.h](../Ports/README.md) for the `port_type` descriptor.
