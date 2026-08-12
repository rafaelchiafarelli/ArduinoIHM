# Next session

Picking up from 2026-08-11. Full detail on what was done is in
`IHM/CHANGELOG.md` -- this file is just the "where things stand / what's
next" handoff.

## Where things stand

Everything is merged into `dev` and pushed to `origin`. The 2026-08-10 PWM
stack (`fix/pwm-state-machine` through `demo/pwm-outputs-walkthrough`) was
fast-forwarded into `dev`; `chore/restore-kicad-pcb-placement` was merged
in with a merge commit (it had diverged independently). All 7 feature
branches were deleted, local and remote, after merging -- `dev` is the only
branch with any of this work on it now. The 2026-08-11
`SerialCommunication` fix (see "Known follow-ups" below) was committed
directly to `dev`, no feature branch.

No PRs were opened for any of this -- it was merged directly by fast-
forward/merge commit with the user's confirmation, since everything landed
without conflicts.

After item 1 (fixed) and scoping item 2 (killed, see below), the session
continued into item 3: Relay is now wired into the Output tab (see
`CHANGELOG.md`'s 2026-08-11 "Wire Relay outputs into the UI" entry). Servo
and motor outputs are still not wired -- same pattern would apply, but each
peripheral has different hardware behavior (Timer4 single-compare for
servos, Timer1 fast-PWM + a stepper mode for DC motors) so it's not a
copy-paste of the Relay work. Nothing is mid-flight; `dev` is clean and
everything above is pushed.

**Worth knowing before continuing:** wiring just Relay's 8 rows took RAM
from 61.9% to 75.0% (+1071 B). There's 25% headroom (2048 B) left, but
servo (8 channels, per the updated hardware model below, not 10) and
motor screens will eat into that further -- check the RAM number after
each one, don't assume there's room for both.

**2026-08-12 session:** no code changes. Two things happened: (1) a full
documentation pass -- `IHM/ARCHITECTURE.md`, a `README.md` in every
`lib/*` module and `src/`, and `IHM/docs/architecture.drawio` -- covering
the whole codebase's module map, scheduling model, and known gaps. (2)
While investigating a suspected Timer4 register conflict for item 1 below,
cross-checking against the user's `IOs IHM.xlsx` and the KiCad schematic
uncovered that `Relay`/`ServoMotor`/`MotorDC` don't drive independent GPIO
pins at all -- they share an addressed 8-bit bus behind three `74LS373`
latches. This is a bigger, more foundational finding than the Timer4
question it started as; see item 0 below for the full protocol. **Also
worth checking:** since `BinaryOutputs::SetOutput()` doesn't implement
this bus protocol, `Relay`'s existing UI wiring (done 2026-08-11, believed
complete) may not actually drive real hardware correctly -- verify against
a real board before trusting it.

## Immediate next steps (pick up here)

0. **Write the multiplexed-output-bus driver -- blocks items 1 and 2, do
   this first.** Superseded 2026-08-12: an earlier version of this item
   described a "Timer4 conflict" between PWM channel 3 and `ServoMotor`.
   That framing was based on a wrong hardware model (that `Relay`/
   `ServoMotor`/`MotorDC` each drive independent GPIO pins via
   `BinaryOutputs`). Checked directly against the user's `IOs IHM.xlsx`
   (`v0` sheet, current/authoritative) and confirmed with the user: **the
   real hardware is nothing like that.**

   `Relay`, `ServoMotor`, and `MotorDC` share one 8-bit data bus (`O0-O7`,
   AVR pins `PC2/PC1/PC0/PD7/PG2/PG1/PG0/PL7`) feeding three separate
   `74LS373` latches, one per device, each selected by its own strobe
   line: `dig_0` (`O10`/`PH6`) = Servo, `dig_1` (`O14`/`PG5`) = Relay,
   `dig_2` (`O15`/`PF4`) = Motor. `OUTPUT_EN` (`O11`/`PB4`) is shared
   tri-state control across all three latches, not part of the write
   sequence. A `74LS373` is a *transparent* latch (not edge-triggered):
   outputs follow the inputs continuously while its enable line is high,
   and hold whatever value was present the instant that line falls. So
   the correct write sequence is: **settle the 8-bit bus -> raise the
   target device's `dig_X` -> drop it again** -- the *falling* edge of
   `dig_X` is what actually captures the byte.

   The 8 hardware PWM pins (`O8/O9` = `OC1C/OC1B`, `O12/O13` = `OC4C/OC4B`,
   `O16-O19` = each timer's `OC*A`) are confirmed direct-to-output, no
   buffer, and are **not** part of this bus at all -- under this model,
   Servo and Motor never touch a PWM-capable pin or timer register, so
   the original Timer4-vs-PWM-channel-3 conflict is moot, and so is the
   `BinaryOutputs` index-overlap gotcha below (both were derived from the
   wrong model).

   `BinaryOutputs::SetOutput()` today does immediate, independent per-pin
   GPIO writes -- it does not implement this bus/address/strobe protocol
   at all, for any of indices 0-7. This needs a real driver rewrite before
   `Relay` (already wired to indices 0-7 today, and likely not actually
   working correctly against real hardware as a result -- worth checking)
   or `ServoMotor`/`MotorDC` can be correct. Each device also needs to
   keep its own current 8-bit state in RAM and rewrite the *whole* byte on
   every change (the bus is shared and byte-wide, not individually
   addressable per bit).

   **Open sub-questions for whoever picks this up:** exact bit layout
   `MotorDC` wants within its one byte (confirmed 1 device address,
   `dig_2`, but not the per-bit meaning); and an atomicity concern --
   the bus is physically shared, so a write from `ServoMotor`'s ISR
   context racing a write from `Relay`/`MotorDC`'s foreground context
   could corrupt either write unless the new driver brackets the
   bus-write-then-strobe sequence against interrupts (`cli()`/`sei()` or
   equivalent).

1. **Servo motor UI wiring** -- blocked on item 0. Also has a real bug,
   independent of the bus rewrite: one `load()` overload clamps against
   `MIN_POSITION` where it should clamp against `MAX_POSITION` (the array
   overload right above it does it correctly) -- fix while wiring, same as
   PWM's "found while fixing the rest" bugs.
2. **DC/stepper motor UI wiring** -- blocked on item 0. `MotorDC.h`'s
   `setMotorA`/`setMotorB` have `analogWrite(...)` commented out
   ("uncomment when using with Arduino") -- speed control was never ported
   to this codebase's direct-register style, unlike `PWM.cpp`. The stepper
   mode (`fast_handler()`) is fully written but never called from
   anywhere. `MotorDC.cpp` is a 0-byte empty file -- this one needs
   writing, not just wiring.

All three are real multi-session efforts -- don't assume any is quick.

(Display-glue native tests, formerly #2, was killed -- see follow-up
below.)

## Known follow-ups

From `CHANGELOG.md`'s "Known follow-ups" section:

- Relay, servo, and DC/stepper-motor outputs (`lib/MultiOutput/src/
  Relay.*`, `ServoMotor.*`, `MotorDC.*`) are still not wired into the UI --
  same as before this session's work, untouched by any of the 5 branches.
  **Relay done 2026-08-11** (see `CHANGELOG.md`); **servo and motor still
  not started.**
  ~~**Pin-index gotcha found while investigating:** `BinaryOutputs`'s pin
  table is one hardcoded 20-slot array, indexed by position, shared across
  `Relay`/`MotorDC`/`ServoMotor`/`MultiOutput`, with `MotorDC` (8-13) and
  `ServoMotor` (8-17) overlapping.~~ -- **superseded 2026-08-12.** That
  analysis assumed `Relay`/`MotorDC`/`ServoMotor` each drive independent
  GPIO pins through `BinaryOutputs`. The real hardware is a shared 8-bit
  bus behind three `74LS373` latches (see item 0 above for the full
  protocol) -- there's no per-pin index overlap to resolve because none of
  these three devices own individual pins at all. The actual blocker is
  writing a driver for the bus/strobe protocol, not repartitioning an
  index range.
- ~~`SerialCommunication::receive()` has an unbounded `rcv_counter`~~ --
  **fixed 2026-08-11**, committed directly to `dev`. See `CHANGELOG.md`.
- ~~`PWMSimplex`/`PWMComplex`/`GUI.cpp`'s Display-facing glue is verified
  by the AVR build and by inspection only, not by native unit tests~~ --
  **killed 2026-08-11, won't do.** Looked into it first: `Element` (base
  class of `PWMSimplex`/`PWMComplex`) holds a concrete `Display*`, not an
  interface, and `Display`/`GFX.h` pull in
  `Arduino.h`/`Print.h`/`I2CDevice.h`/`SPIDevice.h` -> `avr/io.h`, same as
  `SerialCommunication.h` before it, with `Display.cpp`+`GFX.cpp` alone
  ~6,371 lines (Adafruit-GFX-style parallel-TFT driver). Testing it
  natively would need a full Arduino/Display compatibility shim, or a
  production-code interface refactor with real regression risk -- neither
  is "add a test file." Decision: not worth it, because the plan is to
  switch to an SPI display next hardware revision, at which point this
  driver gets replaced anyway and any test investment here would be
  thrown away. Revisit test coverage for the *new* display's glue code
  once that swap happens, not this one.
  Note: the pure logic `PWMSimplex`/`PWMComplex` delegate to
  (`PWMChannelConfig`, `PWMLabelFormat`, `PWMTiming`) already has native
  coverage from the 2026-08-10 stack -- it was only the
  `tft->drawRect(...)`/`tft->print(...)` call sites themselves that were
  ever untested, and that gap is now accepted, not fixed.

Found during the 2026-08-12 documentation pass (`IHM/ARCHITECTURE.md` has
full detail on each; not asked for, not fixed, listed so they aren't lost):

- **`SerialCommunication::receive()` is never called by anything.** It's
  the only entry point that feeds bytes into the framing/checksum state
  machine, but the real `ISR(USART0_RX_vect)`
  (`lib/ArduinoLib/src/HardwareSerial0.cpp`) only fills the standard
  Arduino `Serial` ring buffer -- it doesn't forward to `receive()`.
  Result: `able_to_parse` can never become true, `fast_handler()` always
  returns false, and `voltage0`/`voltage1` (fed to the two `MCP4725`
  DACs) never update from real serial input today. Needs either a
  `USART0_RX_vect` override calling `comms.receive()`, or main-loop
  polling of `Serial.available()`/`Serial.read()` feeding it.
- **`lib/StateMachine/StateMachine.hpp`/`.cpp` doesn't compile and isn't
  used.** References an undefined type (`StateMachineStates` vs. the
  actual enum `FunctionalStates`); nothing includes it (`PWMStateMachine.h`,
  same folder, is what's actually used). Safe to delete.
- **`BinaryInput`'s `MCUCR |= ~(1<<PUD);`** doesn't do what its comment
  ("ensure pull-ups aren't globally disabled") says -- it sets every
  *other* `MCUCR` bit while leaving `PUD` itself untouched. Probably meant
  `MCUCR &= ~(1<<PUD)`.
- **`MCP4725` dac0/dac1 vs. voltage0/voltage1 naming looks crossed** in
  `main.cpp`: `dac1.setVoltage(voltage0,...)`, `dac0.setVoltage(voltage1,...)`.
  May be intentional (matching board wiring) -- worth a deliberate check
  against the actual hardware before assuming "channel 0 = voltage0."
- **`lib/Display/SPITFT.cpp`/`GrayOLED.cpp`** are vendored but entirely
  unreferenced by the actual display path (`Display`/`GFX`/
  `mcufriend_shield.h`) -- dead weight from the library import.

Full architecture writeup, per-module `README.md`s, and a diagram now
exist: `IHM/ARCHITECTURE.md`, `IHM/lib/*/README.md`,
`IHM/docs/architecture.drawio`. Check those before re-deriving module
structure from scratch.

None of these were asked for beyond the `SerialCommunication` fix -- listed
here so they don't get mistaken for "already done" or lost track of.

## How to pick up dev work

- Native unit tests: `IHM/test_native/run_tests.ps1` (PowerShell; drives
  MSVC directly since no gcc is installed on this machine -- see that
  script's header comment). 96 tests, all passing as of the last commit.
- AVR build: `platformio run` from `IHM/` (or `-d` pointed at it). Verified
  after every commit; last known state RAM 75.0% (6144/8192 B), Flash 19.5%
  (49476/253952 B) -- RAM headroom is getting less comfortable than it
  looks (see the note above on servo/motor RAM cost).
- Demo: `IHM/demo/run_demo.ps1` -- prints the register-level walkthrough.
  `IHM/demo/HARDWARE_RUNBOOK.md` has the equivalent checks for real
  hardware.
- Adding a new native-testable file: it needs adding to the allowlists near
  the top of `run_tests.ps1` (`$libAllowlist` for include dirs,
  `$prodSourceAllowlist` for production `.cpp` files to link in) -- these
  are explicit allowlists, not globs, because blindly including everything
  under `lib/` pulls in AVR-only headers that don't compile natively (see
  the comment in `run_tests.ps1` for the specific `lib/ArduinoLib/src/new`
  collision that motivated this).

## Notes for whoever/whatever resumes this

- GitHub push access needed a key added to the `rafaelchiafarelli` account
  mid-session; that's resolved now (push succeeded), no action needed
  unless it regresses.
- Early in this session, a `git reset --hard` run while re-basing a branch
  discarded unrelated uncommitted changes (a KiCad PCB edit and a
  `.vscode/extensions.json` edit) that predated the session. The PCB edit
  was recovered from a backup zip and is now `chore/restore-kicad-pcb-
  placement`; the `extensions.json` edit was not recoverable. Be careful
  with `git reset --hard` / `git checkout` across branches with uncommitted
  changes present -- stash *everything* first, not just the files you think
  are relevant.
