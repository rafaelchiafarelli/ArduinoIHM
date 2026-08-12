# src

The composition root. `main.cpp` owns every top-level module instance and
the one hardware tick that drives most of the project's scheduling.

## The scheduling model

`ISR(TIMER2_COMPA_vect)` fires roughly every ~1.008ms (Timer2, Fast PWM
mode, prescaler 128, `OCR2A = 125`; see `Timer2Config.h/.cpp` for which
sub-interrupt is enabled -- only Compare-A, deliberately). Inside it:

- **Every tick (~1ms):** `userInputs.fast_handler()` (`BinaryInputs` --
  see [lib/BinaryInput](../lib/BinaryInput/README.md)).
- **Every 10th tick (~10ms):** counter increments, currently no handlers
  attached (placeholder comment in `main.cpp` for future `ten_ms_handler`
  calls).
- **Every 25th tick (~25ms):** `multiOutput.slow_handler()` (->
  `Relay::ultra_slow_handler()`), `comms.fast_handler(...)` (see
  [lib/Comms](../lib/Comms/README.md) -- naming is misleading, this is
  the 25ms cadence not the 1ms one), `rotaryEncoders.ms_handler(bMap)`
  (see [lib/RotaryEncoder](../lib/RotaryEncoder/README.md) -- same
  naming caveat).

`ISR(TIMER1_COMPA_vect)` exists but is empty (the one call it could make,
`multiOutput.timer_handler()`, is commented out) -- see
[lib/MultiOutput/README.md](../lib/MultiOutput/README.md) for why this
is also the wrong timer vector for what it would need to drive.

Everything else (`GUI::update()`, the two `MCP4725::setVoltage()` calls,
`buildButtonMap()`, `rotaryEncoders.getDirection()`) runs directly in
`main()`'s `while(1)` superloop, once per pass, with no fixed cadence --
this is the "foreground" work, gated only by how long the loop body
itself takes.

## Timer2Config.h/.cpp

Deliberately minimal and host-testable: `timer2InterruptMask()` returns
just the `OCIE2A` bit as a plain `uint8_t`, so `main.cpp`'s `setup()` can
write `TIMSK2 |= timer2InterruptMask()` without hardcoding the bit
itself. The header's bit-position constants are plain ints, not
`avr/io.h` macros, specifically so this logic can be tested off-device.
Replaces an earlier design that had empty ISRs for Compare-B/Overflow
just to avoid an uninitialized-vector reset.

## Global state

`newDataAvailable`, `timeStatistics`, `timeCounter`, `bMap` are all
`volatile`, written in the Timer2 ISR and read in `main()` -- this is the
project's only cross-context shared state, and it's already correctly
marked volatile throughout.
