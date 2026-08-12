# BinaryOutputs

Register-direct GPIO output driver: a fixed, compile-time table of 20 pin
descriptors, each set/cleared with a single register write. This is the
mechanical pattern every other output module builds on ("the
register-direct strategy" elsewhere in these docs), but as of 2026-08-12,
**most of its 20 slots are not actually meant to be driven this way** --
see "What the 20 indices actually are" below before using this class for
anything new.

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
  not implement this protocol** (no bus-settle-then-strobe sequence) --
  using it as-is on indices 0-7 will not correctly drive Relay, Servo, or
  Motor hardware. See [MultiOutput/README.md](../MultiOutput/README.md)
  and `IHM/ARCHITECTURE.md`'s "multiplexed output bus" section for the
  full protocol; a real driver for it doesn't exist yet
  (`IHM/NEXT-SESSION.md` item 0).
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
indices are actually appropriate to drive with its own `SetOutput()`
one-index-at-a-time model. The class's register table (which physical pin
each index is) is still accurate; the *access pattern* built on top of it
needs to change for indices 0-7 (bus+strobe) and stop entirely for indices
8-19 (owned by `PWM` or the bus's own control lines).

## Hardware resources

20 pins across `PORTB/C/D/E/F/G/H/L`, hardcoded in the `ports{}`
initializer list in `BinaryOutputs.h:27-48`. Index-to-physical-pin mapping
is fixed at compile time; there is no runtime pin configuration.

## Depends on

[Ports.h](../Ports/README.md) for the `port_type` descriptor.
