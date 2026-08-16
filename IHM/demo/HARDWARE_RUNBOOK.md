# Hardware runbook: PWM subsystem

Companion to `demo/demo_pwm_walkthrough.cpp`. That program proves the PWM
register math is correct by running the real production code against fake
registers on a PC. This runbook is the same checks, but on the real board,
where a bad COM-bit or a clobbered register would actually show up on a
scope/LED/motor instead of in a printed hex byte.

Requires: Arduino Mega 2560 flashed with this firmware, the display/rotary
encoder hardware wired per the schematic, and (for the PWM checks) an
oscilloscope or logic analyzer on the relevant pin -- a multimeter can
confirm duty cycle roughly via average DC voltage but won't show frequency
or edge polarity.

## Pin reference

| Channel | Timer  | Pin(s)          | Notes |
|---------|--------|-----------------|-------|
| 0       | Timer3 | OC3A            | simplex |
| 1       | Timer5 | OC5A            | simplex -- OC5B/OC5C are claimed by the rotary encoders (PL4/PL5), so only OC5A is free |
| 2       | Timer1 | OC1A / OC1B / OC1C | complex, 3 outputs |
| 3       | Timer4 | OC4A / OC4B / OC4C | complex, 3 outputs |

Check your board variant's pinout diagram (`ELEC-MEGA2560-PINOUT.webp` in
the repo root) for which physical header pin each OCnX maps to.

## UI navigation (see PWMScreen.h / GUI.cpp)

1. Power on. Rotate rot0 (encoder 0) until the PWM tab is highlighted.
2. Rotate rot1 (encoder 1) to move the highlight between the 4 channel
   boxes.
3. Press rot1's button to drill into the highlighted channel -- rot1 now
   moves between that channel's fields (Mode, Frequency, Duty Cycle, Edge,
   and for complex channels, per-output Activate/Duty/Edge for A/B/C).
4. Rotate rot2 (encoder 2) to change the highlighted field's value. Every
   change is applied to the real timer immediately -- there is no separate
   confirm step.
5. Press rot1's button again to back out to channel selection.

## Checks

### 1. Every fixed frequency actually appears (channels 0 and 1)

For channel 0 (or 1): cycle Mode to Fixed, then step Frequency through all
13 fixed entries. At each step, probe OC3A (or OC5A) and confirm the scope
shows the frequency printed in the demo log / on screen (e.g. `F:62500Hz`).
Pay particular attention to the 9-bit and 10-bit entries (frequencies
31250Hz, 15625Hz, 3906Hz, 1953Hz, 488Hz, 244Hz, 122Hz, 61Hz, 30Hz, 15Hz) --
these are exactly the cases where the pre-fix firmware produced no output
at all (missing COMnX1). If any of these show nothing on the scope, that's
a regression of the branch 2 fix.

### 2. Edge polarity (rising vs falling)

At any fixed frequency with duty cycle set away from 50% (so rising/falling
look visibly different), toggle the Edge field and confirm the scope shows
the duty-high portion moving from the start of the period (rising/
non-inverting) to the end (falling/inverting).

### 3. Duty cycle sweep

Step Duty Cycle from 0% to 100% and confirm the scope's measured duty cycle
tracks it. Check at least one 8-bit frequency (e.g. 62500Hz), one 9-bit
(31250Hz), one 10-bit (15625Hz), and Variable mode -- these each scale the
0-100% UI value against a different raw TOP (255/511/1023/your chosen
Variable Top), per `pwmResolutionTop()`.

### 4. Variable frequency mode

Cycle Mode to Variable, then adjust the Frequency field (now the raw TOP
register instead of a fixed-frequency list -- see the on-screen Hz readout,
computed as `16MHz / (TOP+1)`). Confirm the scope frequency matches the
on-screen readout at a couple of different TOP values.

### 5. Complex channel independence (channels 2 and 3)

With channel 2 (or 3) selected: enable output A at some duty cycle, probe
OC1A. Then, *without touching A's fields again*, enable output B at a
different duty cycle and inverted edge, and re-probe OC1A -- it must be
unchanged. Then enable output C and re-probe both OC1A and OC1B -- both
unchanged. This is the same sequence `runComplexIndependenceProof()` runs
against fake registers in the demo; on hardware it's the same proof against
a scope instead of a printed byte.

### 6. Mode field / Off state

For a simplex channel, cycle Mode to Off and confirm the pin goes idle
(steady low, no toggling) -- this is the case that used to have no explicit
"disabled" concept at all (see PWMChannelConfig's 3-way mode cycle).

## Out of scope for this pass

The relay and DC/stepper-motor outputs (`lib/MultiOutput/src/Relay.*`,
`MotorDC.*`) and the serial command protocol
(`lib/Comms/src/SerialCommunication.*`) are **not** exercised by this demo
or wired into the UI -- exactly as found in the original review, and
unchanged by any of the 5 branches in this pass (their scope was PWM
configuration, encoder/button input, and the Timer2 ISR setup only).
`MultiOutput::slow_handler()`/`fast_handler()` run every main-loop tick
already, but nothing ever calls `Relay::enableRelay()`,
`MotorDC::setMotorA/B()`, or feeds bytes into
`SerialCommunication::receive()`. (Servo output was a third item here
originally -- `ServoMotor` is deleted as of 2026-08-16, servo control
isn't part of the IHM solution.) Wiring those into the UI (and, for serial,
fixing the unbounded `rcv_counter` growth in `receive()` noted in the
original review) is natural follow-up work, but doing it here would mean
demoing code nobody asked to have fixed yet.
