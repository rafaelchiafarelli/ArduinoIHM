# src

The composition root. `main.cpp` owns every top-level module instance and
the two hardware interrupts that drive most of the project's scheduling.

## The scheduling model

`ISR(TIMER2_COMPA_vect)` fires roughly every ~1.008ms (Timer2, Fast PWM
mode, prescaler 128, `OCR2A = 125`; see `Timer2Config.h/.cpp` for which
sub-interrupt is enabled -- only Compare-A, deliberately). Inside it:

- **Every tick (~1ms):** `userInputs.fast_handler()` (`BinaryInputs` --
  see [lib/BinaryInput](../lib/BinaryInput/README.md)), and
  `multiOutput.fast_handler()` (-> `MotorDC::fast_handler()`, stepper
  commutation / DC software-PWM duty cycling -- see
  [lib/MultiOutput](../lib/MultiOutput/README.md)).
- **Every 10th tick (~10ms):** counter increments, currently no handlers
  attached (placeholder comment in `main.cpp` for future `ten_ms_handler`
  calls).
- **Every 25th tick (~25ms):** `multiOutput.slow_handler()` (->
  `Relay::ultra_slow_handler()`), `comms.fast_handler(...)` (see
  [lib/Comms](../lib/Comms/README.md) -- naming is misleading, this is
  the 25ms cadence not the 1ms one), `rotaryEncoders.ms_handler(bMap)`
  (see [lib/RotaryEncoder](../lib/RotaryEncoder/README.md) -- same
  naming caveat).

`ISR(ADC_vect)` fires on every completed ADC conversion (~104-200us
apart, independent of the Timer2 tick) and calls
`analogInputs.isr_handler()`, which caches the result and immediately
starts the next channel's conversion -- a continuous, self-sustaining
round-robin scan across all 5 channels. `AnalogInputs::read()` just
returns the cached value, non-blocking. See
[lib/AnalogInput/src/AnalogInput.h](../lib/AnalogInput/src/AnalogInput.h).

Everything else runs directly in `main()`'s `while(1)` superloop, once
per pass, with no fixed cadence -- this is the "foreground" work, gated
only by how long the loop body itself takes: `buildButtonMap()` /
`rotaryEncoders.getDirection()`, the Janus UI (encoder-driven
screen-switch and focus/activate, `janus_handle_action()`,
`janus_render_*` -> `Display`; see `ARCHITECTURE.md`'s "The UI is
generated" section), `mavlinkComms.poll()`, a ~100ms telemetry/bus-status
block, and the two `MCP4725::setVoltage()` calls (currently commented out
-- see `NEXT-SESSION.md`).

## Timer2Config.h/.cpp

Deliberately minimal and host-testable: `timer2InterruptMask()` returns
just the `OCIE2A` bit as a plain `uint8_t`, so `main.cpp`'s `setup()` can
write `TIMSK2 |= timer2InterruptMask()` without hardcoding the bit
itself. The header's bit-position constants are plain ints, not
`avr/io.h` macros, specifically so this logic can be tested off-device.
Only Compare-A is enabled -- Compare-B and Overflow are left masked, no
handlers.

## Global state

`newDataAvailable`, `timeStatistics`, `timeCounter`, `bMap` are all
`volatile`, written in the Timer2 ISR and read in `main()`. `AnalogInputs`
has its own equivalent cross-context state (`results[]`, written in
`ISR(ADC_vect)` via `isr_handler()`, read in `main()` via `read()`) --
same volatile pattern, encapsulated inside the class instead of living as
a `main.cpp` global.
