# Initiative: navigation

Drill-down navigation of the on-screen UI: **RE0 selects** (tab, then
option, then setting), with a dedicated back button (B0) and an inactivity
timeout. **RE2 changes whatever is selected**, live. Replaces today's split
where RE0 switches tabs and RE1 walks focus inside a tab.

**Editing rule (Rafael, 2026-09-26):** "when something is selected and you
turn RE2, that thing changes." Every RE2 detent applies to the hardware
immediately. There's no edit mode, no confirm and nothing to discard. Until
`nav_state_machine/3` lands, RE1 does the selecting (today's focus ring)
and RE2 already edits PWM frequency (fixes/000006) and duty
(`value_editing/2`).

## Input enumeration

The board has 3 rotary encoders, each with its own push-button, plus 4
standalone push-buttons. Names below are the names used everywhere from
now on (code constants, docs, telemetry labels). Hardware is not fully
built yet; this is the set the firmware already reads.

| Name | Kind | MCU pin | Mega pin | `BinaryInputs` index | `btnMap` bit | Role in this initiative |
|---|---|---|---|---|---|---|
| RE0 | encoder A/B | PL6 / PL5 | D43 / D44 | 6 / 7 | -- | **main navigation** |
| pbRE0 | push-button | PL4 | D45 | 8 | 6 | **enter / confirm** |
| RE1 | encoder A/B | PL2 / PL1 | D47 / D48 | 9 / 10 | -- | unassigned |
| pbRE1 | push-button | PL0 | D49 | 11 | 5 | unassigned |
| RE2 | encoder A/B | PC4 / PC5 | D33 / D32 | 12 / 13 | -- | unassigned here; today steps a focused PWM frequency label (fixes/000006, see below) |
| pbRE2 | push-button | PC7 | D30 | 14 | 4 | unassigned |
| B0 | push-button | PA4 | D26 | 0 | 0 | **back** |
| B1 | push-button | PE4 | D2 | 1 | 1 | unassigned |
| B2 | push-button | PF5 | A5 | 2 | 2 | unassigned |
| B3 | push-button | PF6 | A6 | 3 | 3 | unassigned |

`btnMap` is `buildButtonMap()`'s output (`lib/RotaryEncoder/ButtonMap.h`),
active-low. Indices 4/5 (PA0/PA2) are housekeeping inputs, not buttons.
"Unassigned" means no navigation role: RE1/pbRE1 lose their current
focus/activate role once this initiative lands. Roles for them are a
later initiative.

## Navigation model

Three levels. RE0 always moves within the current level. pbRE0 goes one
level deeper. B0 always goes one level up. RE2 edits the selected setting at
L2. There is no separate value level (the earlier L3 VALUE was dropped
2026-09-26, see the editing rule above).

| Level | RE0 turns | pbRE0 click | B0 click | RE2 turns |
|---|---|---|---|---|
| L0 TAB | cycle tabs PWM / SERIAL / Output (screen switches live) | enter L1 on this tab | no-op | nothing |
| L1 OPTION | next/prev option of the tab (e.g. PWM: CH0..CH3; Output: Relay 0..7) | enter L2 on this option | back to L0 | nothing |
| L2 SETTING | next/prev setting of the option (e.g. CH0: Enabled, Inverting, Frequency, Duty) | open question 7 | back to L1 | change the selected setting, live: toggle flips, frequency ±1 fixed step (clamped), duty ±1 % (clamped 0-100) |

**Not yet re-planned:** the `nav_state_machine` task files still describe
the four-level model with L3. Update them to this table when that epic is
picked up.

**Inactivity timeout:** 4 s with no navigation input returns straight to
L0, with the currently active tab kept.

The hierarchy (which options each tab has, which settings each option
has, which Janus widget each maps to) is an **IHM-side table** driven by
an IHM-side state machine in a new `lib/Navigation`. It calls Janus's
existing focus/screen API. It is not declared in the Janus yaml, and
this initiative requests no Janus runtime change.

## Epics

| Epic | Scope |
|---|---|
| `input_map` | Named constants for the enumeration above; a pure click-edge detector. |
| `nav_state_machine` | Pure `Navigator` (levels, timeout), the IHM nav table, wiring into `main.cpp`. |
| `value_editing` | What RE2 does to the selected setting, per kind: toggles, PWM duty, PWM frequency. |

Task order across epics:

```
input_map/1-input-names ─┐
input_map/2-click-edges ─┼─> nav_state_machine/3-wire-into-main ─> value_editing/*
nav_state_machine/1-navigator ─> 2-ihm-nav-table ─┘
```

## Open questions -- each must be decided before the task it blocks is ready

1. **L1 highlight on the PWM tab.** Options there are `row`s, which are not
   focusable Janus widgets. Pick one: wrap each channel row in a `box`
   (yaml change, like `bus_status`), ring the option's first setting
   instead, or ask Janus for row focus (a handoff). Blocks
   `nav_state_machine/2`. CH0/CH1's Enabled/Inverting switches sit
   unlabeled beside the frequency, each in its own `focus_ring` row
   (toggles draw no ring of their own). As of 2026-09-25, RE1 wraps within
   the screen and never reaches the tab strip (main.cpp passes Janus a
   nav-less copy of the app).
2. **Single-setting options** (Relay N has only on/off). Should pbRE0 at L1
   go through L2 with one entry, or should selecting the relay at L1 already
   let RE2 flip it? Blocks `nav_state_machine/2`.
3. ~~Apply and commit in L3.~~ **Answered 2026-09-26: live.** Each RE2
   detent applies to the hardware immediately. With no edit level, there's
   no confirm step and nothing for B0 or the timeout to discard.
4. ~~Duty step size.~~ **Answered 2026-09-26: 1 % per RE2 detent**, clamped
   at 0 and 100.
5. **SERIAL tab.** Should its options (CAN0, CAN1, RS485) be read-only in v1
   (L1 only, pbRE0 does nothing), or editable? Their configs are stored
   but no bus driver consumes them yet. Blocks `nav_state_machine/2`.
6. **What resets the 4 s timer?** Only RE0/pbRE0/B0, or any input
   including the unassigned ones? Blocks `nav_state_machine/1`.
7. **pbRE0 at L2 (SETTING).** With editing on RE2, should pbRE0 on a
   selected setting press it (the way pbRE1 does today: a switch flips, a
   frequency label cycles, a duty bar does nothing), or do nothing? Blocks
   `nav_state_machine/2`.

## Since planned: RE2 frequency step (fixes/000006, 2026-09-26)

Outside this initiative, Rafael had RE2 wired to PWM frequency: RE1
focuses a channel's frequency label (now a focusable `focus_ring` row with
`on_press: cycle_pwm_chN_frequency`), RE2 steps it live (clamped), and
pbRE1 cycles it. It relies on RE1 focus, which `nav_state_machine/3`
removes. **Resolved 2026-09-26:** RE2 *is* the editing knob (see the
editing rule at the top). When the navigator lands, RE2 keeps its role and
only "what is selected" moves from RE1 focus to the navigator's L2
setting. `pwmStepFrequency`/`pwmCycleFrequency` (`PWMTiming.h`, unit-tested)
already exist either way.

## Dependency on serial_commands

This initiative builds on code that exists only on the `serial_commands`
chain today, not on `dev`: the app-level `janus_focus_move(&janus_app, …)`
API, `mirrorPwmToUi`, and `IHM_SIMULATE_BUTTON` (the PC can press pbRE0
and B0, which is the bench path for testing without the knobs). The
navigation branch chain must be created from `dev` **after** serial_commands
has merged into `dev`.

## Bench testing

`mavlink/scripts/sim_input.py --port COM3` (or the companion app) can
turn RE0 and press pbRE0/B0 over MAVLink on Serial2. Every navigation
task's bench step can run from the PC.

## Branch chain

```
dev -> features -> navigation -> epics -> <epic> -> tasks -> <task>
```
