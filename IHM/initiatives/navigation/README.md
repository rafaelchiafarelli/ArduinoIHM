# Initiative: navigation

**Status: closed 2026-09-27.** `value_editing` is done. `input_map` and
`nav_state_machine` were dropped without being built (see "Closing
decisions" below). The on-screen controls on `dev` are the final design.

## Final controls

What was planned as a drill-down (RE0 selects tab, option, then setting;
pbRE0 enters, B0 goes back, 4 s timeout) was replaced by what the board
already did once `value_editing` landed. Rafael, 2026-09-27: "pwm is
working fine now, we will take it as the standard... once user select the
nav with RE0, RE1 will surf the selectables in that nav, one after the
other and circle back to the first."

| Input | Role |
|---|---|
| RE0 | cycle tabs PWM / SERIAL / Output (screen switches live) |
| RE1 | move the focus ring through the current tab's selectables, wrapping at both ends; never onto the tab strip |
| pbRE1 | press the focused control: a switch flips, a frequency label steps one lower (15 Hz wraps to 62500 Hz), a duty bar does nothing |
| RE2 | change the focused control, live, no confirm: frequency ±1 fixed step (clamped), duty ±1 % (clamped 0-100), switch CW = on / inverting, CCW = off / non-inverting |
| pbRE0, pbRE2, B0-B3 | no UI role (still read, and reported in `IHM_BOARD_STATE`) |

Every tab follows the same rule: RE1 selects whatever is editable, RE2
changes it. The SERIAL tab has nothing focusable yet; its fields become
editable in the `serial_config` initiative, with this same pattern.
`demo/HARDWARE_RUNBOOK.md` "UI navigation" is the user-facing description.

## Input enumeration

Names for the board's inputs. `input_map` would have turned them into
code constants. It was dropped, so the code still says `dir[0..2]` and
`BTN_MASK_ROT0..2`.

| Name | Kind | MCU pin | Mega pin | `BinaryInputs` index | `btnMap` bit |
|---|---|---|---|---|---|
| RE0 | encoder A/B | PL6 / PL5 | D43 / D44 | 6 / 7 | -- |
| pbRE0 | push-button | PL4 | D45 | 8 | 6 |
| RE1 | encoder A/B | PL2 / PL1 | D47 / D48 | 9 / 10 | -- |
| pbRE1 | push-button | PL0 | D49 | 11 | 5 |
| RE2 | encoder A/B | PC4 / PC5 | D33 / D32 | 12 / 13 | -- |
| pbRE2 | push-button | PC7 | D30 | 14 | 4 |
| B0 | push-button | PA4 | D26 | 0 | 0 |
| B1 | push-button | PE4 | D2 | 1 | 1 |
| B2 | push-button | PF5 | A5 | 2 | 2 |
| B3 | push-button | PF6 | A6 | 3 | 3 |

`btnMap` is `buildButtonMap()`'s output (`lib/RotaryEncoder/ButtonMap.h`),
active-low. Indices 4/5 (PA0/PA2) are housekeeping inputs, not buttons.

## Epics

| Epic | Status | Scope |
|---|---|---|
| `value_editing` | **done** (4/4) | What RE2 does to the focused control, per kind: toggles, PWM duty, PWM frequency. |
| `input_map` | **dropped** 2026-09-27 | Named input constants; a click-edge helper. |
| `nav_state_machine` | **dropped** 2026-09-27 | Pure `Navigator` (levels, timeout), IHM nav table, wiring into `main.cpp`. |

The dropped epics' task files are kept, marked dropped, as the record of
what was planned.

## Closing decisions (Rafael, 2026-09-27)

The open questions of the drill-down plan, as closed:

1. **L1 highlight on the PWM tab.** Moot. There is no L1; the PWM tab as
   it is (one focus ring per selectable) is the standard.
2. **Single-setting options (relays).** Moot. The Output tab stays exactly
   as it is.
3. Apply and commit: **live** (answered 2026-09-26).
4. Duty step: **1 % per RE2 detent**, clamped 0-100 (answered 2026-09-26).
5. **SERIAL tab.** Same logic as the PWM tab: RE1 focuses whatever is
   editable, RE2 changes it. Making the fields editable is
   `serial_config`'s job (its decision 2).
6. **Inactivity timeout.** None. Focus stays where RE1 left it.
7. **pbRE0 at L2.** Moot. pbRE0 has no UI role; pbRE1 keeps today's press.

Also dropped with `input_map`: renaming `dir[i]` / `BTN_MASK_ROT*` in the
code, `sim_input.py` name aliases, and the `ButtonClicks` edge detector
(`main.cpp` keeps its inline `prevBtnMap` edge for pbRE1).

## Not carried over

- **Full-screen redraw blocks the superloop > 0.7 s** after an on-screen
  action (`NEXT-SESSION.md`). It was flagged for this initiative, but none
  of its tasks addressed it. It is still an open item there, not owned by
  any initiative.

## History

- 2026-09-26: Rafael made RE2 the editing knob ("when something is
  selected and you turn RE2, that thing changes"), live, with no edit
  level. `fixes/000006` shipped RE2 frequency stepping outside the plan
  (recorded in `value_editing/3`).
- 2026-09-27: parked with a partial merge to `dev` so this clone could
  start `desktop_mirror`; later that day, closed with the decisions above.
