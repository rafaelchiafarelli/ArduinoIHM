# Changelog

## 2026-08-11 -- Wire Relay outputs into the UI (Output tab)

First of the three items in "Relay, servo, DC/stepper-motor outputs... not
wired into the UI" (see 2026-08-10's Known follow-ups) -- scoped to Relay
only; servo/motor are separate future work, see `NEXT-SESSION.md`. Followed
the same pattern already proven for PWM: a pure/testable config class, thin
Display-facing glue, a screen container, then GUI wiring.

Investigated first and confirmed pin safety: `BinaryOutputs`'s hardcoded
20-slot pin table (`lib/BinaryOutputs/src/BinaryOutputs.h`) has `Relay` using
indices 0-7 exclusively; `MotorDC` (8-13) and `ServoMotor` (8-17) overlap
*each other* (pre-existing, untouched, out of scope) but neither touches
Relay's range, so wiring Relay in doesn't risk double-driving any pin.

- `lib/MultiOutput/src/RelayConfig.h` -- new, pure on/off struct (mirrors
  `PWMChannelConfig`'s shape, trivial: one bool + `toggle()`). Natively
  tested (`test_native/test_relay_config.cpp`, 2 tests).
- `lib/Elements/src/RelayElement.h` / `RelayScreen.h` -- new, Display-facing
  glue: one row per relay (an `LED` on/off indicator + a `Label` reusing
  `PWMLabelFormat::formatShortStatusLabel`'s "Ativo"/"Desl." text -- that
  function is generic on/off text, not PWM-specific, so no duplicate
  formatter was needed). `RelayScreen` mirrors `PWMScreen` but needs no
  `PWMStateMachine`-style field-cursor, since a relay is only ever on/off.
  Verified by AVR build + inspection only, same accepted gap as PWM's glue
  (see the killed Display-glue-testing follow-up above).
- `lib/MultiOutput/src/MultiOutput.h` -- added `getRelays()` accessor and
  made `setup()` enable all 8 relays (still start off). Deliberately *not*
  a second standalone `Relay` instance the way PWM has two disconnected
  `PWM` objects (`main.cpp`'s standalone one wired to `GUI`, plus an unused
  one inside `MultiOutput`) -- `MultiOutput::slow_handler()` already calls
  `relays.ultra_slow_handler()` every ~25ms to re-assert relay state, so a
  duplicate would mean the UI-controlled relays never get that refresh.
  `GUI` now takes the same `Relay*` `MultiOutput` owns.
- `lib/GUI/GUI.h`/`.cpp`, `src/main.cpp` -- Output tab wired: rot1 rotates
  through the 8 relays, rot1's button (same bit `PWM_SELECTED` uses for its
  own tab-local meaning, safe to reuse since it's scoped to this `case`)
  toggles the highlighted one -- applied to the real relay immediately, no
  separate confirm step, same UX as PWM fields.
- `Relay.h`/`Relay.cpp` themselves: **untouched**. `setRelay`/`enableRelay`
  were already public and exactly right.

**96 native tests total, all passing.** AVR build verified
(`platformio run`, `megaatmega2560` env) -- RAM jumped from 61.9% to **75.0%**
(5073 -> 6144 / 8192 B, +1071 B) for 8 new UI rows, each an `LED`+`Label`
pair; Flash barely moved (18.8% -> 19.5%). Flagged to the user as a bigger
jump than expected for "8 toggles" -- 25% RAM headroom remains, not
dangerous, but worth knowing before adding servo/motor screens on top.

No physical hardware was available this session to verify an actual relay
click -- verified by AVR build + native tests + manual trace only, same
caveat as prior AVR-build-only verifications.

## 2026-08-11 -- Fix unbounded `rcv_counter` in `SerialCommunication::receive()`

Follow-up from the 2026-08-10 pass below, which flagged this bug but left it
unfixed. `receive()` wrote every incoming byte to `ReceivedBytes[rcv_counter]`
after unconditionally incrementing `rcv_counter`, with no bound tied to the
array's size (256 bytes). If a frame's `HEADER`/`FOOTER`/`TERMINATOR` never
showed up in the stream (desync, noise, or a `Found` state with an
unusually long/malformed payload), `rcv_counter` grew past the array end,
corrupting adjacent class members (`ParseBuffer`, `OutBuffer`, etc.).

Fixed by checking `rcv_counter` against `sizeof(ReceivedBytes) - 1` *before*
each increment/write; on overflow, resets to `Searching` (drops the
buffered bytes and starts looking for a fresh `HEADER`) instead of writing
past the buffer. `lib/Comms/src/SerialCommunication.cpp`.

Not covered by a native unit test: `SerialCommunication.h` pulls in
`HardwareSerial.h`/`avr/io.h`/`avr/interrupt.h` directly, so it can't compile
for the host target without mocking the whole AVR serial stack -- the same
disproportionate-effort call made for the Display glue in the entry below.
Verified instead by AVR build (`platformio run`, unchanged RAM/Flash: 61.9%
/ 18.8%) and by tracing the state machine by hand for both the `Searching`
and `Found` overflow paths.

## 2026-08-10 -- PWM subsystem: state machine, register bugs, UI wiring, input fixes

Follow-up to a full-codebase review (see conversation history / PR
descriptions for the original findings). Delivered as 5 stacked branches off
`dev` (each branches from the previous, sharing the native test harness
added in the first one -- review/merge in order), one independent branch,
and a final demo branch stacked on the tip. All branches build the real AVR
firmware (`platformio run`) and pass the full native test suite at every
step; see `IHM/test_native/` and `IHM/demo/`.

### Branch stack (merge in this order)

| # | Branch | Commit | Summary |
|---|--------|--------|---------|
| 1 | `fix/pwm-state-machine` | `5e00764` | Fixed `PWMStateMachine.h`'s broken `#include <events.h` (unterminated, file doesn't exist) and completed the class -- it was a stub with no transition logic. Added channel selection and field-cursor cycling for both simplex and complex channel UIs. Added `test_native/`, a host-native unit test harness (MSVC-driven; no gcc on this machine, PlatformIO's native platform needs one). **15 tests.** |
| 2 | `fix/pwm-com-bits` | `8f62ecd` | `PWM.cpp` (1109 lines, hand-duplicated across 4 channels x 14 frequencies) had the missing-`COMnX1`-bit bug in every 9/10-bit mode, a `=`-instead-of-`\|=` clobbering bug in both 3-output channels, a stray extra bit copy-pasted into two cases, a clock-select macro typo, and -- found while fixing the rest -- channel 1 configured the wrong compare register entirely (`COM5C*`/writes to `OCR5A`) and could never have produced output at all. Rewrote the register math as a small, pure, fully-tested core (`PWMTiming.h/.cpp`, `PWMConfig.cpp`) behind a zero-overhead `Reg8`/`Reg16` register-handle HAL (`lib/HAL`), reducing `PWM.cpp` itself to four one-line AVR-bound wrappers. **31 tests.** |
| 3 | `feat/wire-pwm-screen-to-hw` | `3328d2a` | Nothing tracked "what is this PWM channel currently configured to do" anywhere in the firmware -- the UI showed static placeholder text and `PWM pwm;` was never called. Added `PWMChannelConfig`/`PWMComplexChannelConfig` (UI-editable state + pure conversion to the exact register-call arguments) and `PWMLabelFormat` (on-screen text, no float-printf). Wired `PWMSimplex`/`PWMComplex`/`PWMScreen`/`GUI.cpp` so every field edit applies live to the real timer -- no separate confirm step. Renamed `frequency_16_625HZ` to `frequency_15_625HZ` (its real output is 15625Hz) since the whole point of the new frequency label is to show the user the truth. All of this runs from the main loop via `GUI::update()`, same as before -- nothing added here runs from an ISR or blocks. **37 tests.** |
| 4 | `fix/encoder-bitmap-volatile` | `630ec87` | Rot1's encoder/button pins were swapped: `RotaryEncoder.h` read its own push-button pin (index 11) as a quadrature signal, while `main.cpp`'s `rot1btn` extraction read what should have been that signal pin (index 10) as the button. Fixed both, extracted the button-bitmap construction into a tested pure function (`ButtonMap.h/.cpp`), and marked `bMap`/`newDataAvailable`/`timeStatistics`/`timeCounter` `volatile` (written in `TIMER2_COMPA_vect`, read from `main()`). **10 tests.** |
| 5 | `fix/timer2-isr-cleanup` | `c2ec0c7` | `setup()` enabled all three Timer2 interrupts but only Compare-A had a handler; Compare-B/Overflow firing would vector to the reset vector, so two empty ISRs existed just to catch that. Root-caused it: only enable Compare-A, delete both empty ISRs. Also corrected two stale comments in the same block (prescaler is /128 not /256; tick period is ~1.008ms not 256us) found while verifying the fix. **2 tests.** |

**94 native tests total, all passing.** AVR build verified after every
branch (`platformio run`, `megaatmega2560` env) -- final state RAM 61.9%
(5073/8192 B), Flash 18.8% (47840/253952 B).

### Independent branch

| Branch | Commit | Summary |
|--------|--------|---------|
| `chore/restore-kicad-pcb-placement` | `43db406` | Restores `ElectricalProject/ArduinoIHM.kicad_pcb`'s 76-footprint placement, which was uncommitted work-in-progress from around 2026-01-17 that a `git reset --hard` (run while re-basing branch 2 onto branch 1, without first stashing *unrelated* uncommitted changes) accidentally discarded mid-session. Recovered from a KiCad backup zip still on disk. Branched from `dev` directly -- unrelated to the firmware stack above. |

### Demo

| Branch | Commit | Summary |
|--------|--------|---------|
| `demo/pwm-outputs-walkthrough` | `422b0bc` | `demo/demo_pwm_walkthrough.cpp` drives the real production code (the same functions `PWMSimplex`/`PWMComplex` call on-device) against fake registers and prints the resulting register bytes for every channel x frequency x edge combination -- readable proof the fixes above do what they claim (e.g. the 9-bit inverting case now produces `TCCRxA=0xC2`, not the old `0x42`). `demo/HARDWARE_RUNBOOK.md` is the same checks for real hardware, plus an explicit note on what's still out of scope: relay/servo/motor outputs and the serial protocol were never wired into the UI and remain that way -- untouched by any of the 5 branches above. |

### Known follow-ups (not done in this pass)

- Relay, servo, DC/stepper-motor outputs (`lib/MultiOutput/src/Relay.*`,
  `ServoMotor.*`, `MotorDC.*`) and the serial command protocol
  (`lib/Comms/src/SerialCommunication.*`) are still not wired into the UI.
- `SerialCommunication::receive()`'s unbounded `rcv_counter` growth (no
  header found within the buffer) is unfixed -- noted in the original
  review, out of scope for this pass. **Fixed 2026-08-11, see entry above.**
- The demo's `PWMSimplex`/`PWMComplex`/`GUI.cpp` glue is verified by the AVR
  build and by inspection, not by native unit tests -- it's Display-coupled
  and mocking the display stack was judged out of proportion to this fix
  (same reasoning applied to `PWM.cpp`'s thin AVR wrappers in branch 2).
  **Won't fix, decided 2026-08-11** -- the plan is to switch to an SPI
  display next hardware revision, which replaces this driver outright, so
  test investment in the current parallel-TFT glue would be thrown away.
  See `NEXT-SESSION.md` for the full reasoning.
