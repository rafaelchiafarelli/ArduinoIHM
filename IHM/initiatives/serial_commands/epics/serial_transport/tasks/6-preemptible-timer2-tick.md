# Task 6: preemptible-timer2-tick

**Status:** done
**Branch:** `6-preemptible-timer2-tick` (from `tasks`)
**Depends on:** task 4

## Why

Bench, 2026-09-25 (old-revision board, PL2303 on COM3): about 60 % of
PC -> board frames were lost. A debug build showed lost frames matched
`uart2::rxLineErrorCount()` increments, and every frame that did arrive
was applied. The Timer2 ISR ran with interrupts off for up to ~216 us
before task 4's `sei()`: `multiOuput`/`userInputs` fast handlers ~116 us
every tick, plus relay 52 / encoders 40 / comms 8 us every 25 ms. At
250000 baud a byte is 40 us and USART2 buffers ~3, so RX overran
whenever a frame landed in that window. The parser can then swallow the
next good frame too.

## Contract

### Delivers

1. **`src/main.cpp` `ISR(TIMER2_COMPA_vect)`**: `sei()` as the first
   statement, so the USART2 RX ISR can preempt the whole tick. Rafael's
   rule: the Timer2 handler must never exceed its ~1 ms slot, so there is
   no re-entry guard. Task 4's `mavlinkBusy` flag and its `sei()`/`cli()`
   pair around `mavlinkComms.fast_handler()` are removed.
2. Docs: the ISR comment, `lib/MavlinkComms` / `lib/Uart2` README and
   `ARCHITECTURE.md` wherever they describe "interrupts re-enabled only
   for the MAVLink handler".

Short critical sections that already have their own `ATOMIC_BLOCK`
(`MultiplexedBus::write`, the `MavlinkComms` copy-outs) are unchanged.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; bench: 10/10
simulated pbRE1 presses toggle a relay, and a sustained 10 s command
stream on COM3 shows 0 bad telemetry frames; task file marked done in the
same commit.

## Result (2026-09-25)

AVR build RAM 4618 B (56.4%), Flash 72048 B (28.4%); native tests 117/117.
Bench on COM3: 10/10 pbRE1 presses toggled the relay (1.6 s apart), and a
10 s stream of 400 command frames gave 0 bad telemetry frames. A debug
build showed 0 overrun and 0 framing errors. Presses 0.7 s apart still merge
in pairs: a full-screen redraw after an action blocks the superloop for
> 0.7 s. That is a UI issue, flagged for the navigation initiative, not a
serial one.
