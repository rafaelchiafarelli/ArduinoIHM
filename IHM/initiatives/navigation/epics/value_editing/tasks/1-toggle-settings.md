# Task 1: toggle-settings

**Status:** done 2026-09-26. All 4 items delivered as written; 132 native
tests pass; RAM 59.0 %. Bench DoD passed on the old board revision over COM3
with simulated encoders, 8/8: CH0 Enabled CW -> 1, CW again -> 1, CCW -> 0;
CH0 Inverting CW -> 1, CCW -> 0; relay 0 CW -> 1, CW again -> 1, CCW -> 0.
**Defect found by Rafael after delivery, fixed on this branch:** the screen
lagged. Switches (CH0/CH1) or LEDs (CH2/CH3, relays) only repainted once
focus moved. Cause: a Janus runtime dirty-flag bug with two widgets on one
field (`initiatives/janus_handoff/2026-09-26-dirty-flag-shared-field.md`).
Worked around with a full `janus_render_screen()` after an RE2 switch change.
The readback was never wrong, which is why the 8/8 check missed it.
**Workaround removed 2026-09-26:** lib/GUI regenerated with Janus dev
c56f14c (`shared_field_dirty`, runtime files only). RE2 edits use
`janus_render_screen_if_dirty` again.
**Branch:** `1-toggle-settings` (from `tasks`)
**Depends on:** nothing unmerged. It uses today's RE1 focus as "selected".
It needs `fixes/000008` (no phantom RE2 step at boot, which could otherwise
switch an output on); that fix was synced down the chain on 2026-09-26.

Re-planned for the initiative's editing rule (select it, turn RE2, it
changes, live). Rafael, 2026-09-26: RE2 on a switch is **directional, not a
flip**: **CW = on / inverting, CCW = off / non-inverting**. The same rule
applies to **every switch, relays included**. A step that asks for the
state the switch is already in does nothing.

## Contract

### Delivers

1. **`pwmWireSetOutputBit(w, out, inverting, value)`**
   (`lib/MultiOutput/src/PWMWireConfig.h`), pure: sets output `out`'s
   enable bit (or inverting bit) to `value` and reports whether anything
   changed. Unit-tested in `test_native/test_pwm_wire_config.cpp`.
2. **RE2 on a focused switch** (`src/main.cpp` RE2 branch, plus
   `src/janus_actions.cpp`): `janusSetSwitch(action, on)` resolves any
   `toggle_*` action (the 10 PWM switches and the 8 relays) and sets it to
   `on` (CW -> true, CCW -> false). It returns false for anything that isn't
   a switch, so the frequency/duty handling keeps working.
   - PWM: `pwmSetOutputBit(ch, out, inverting, on)` in `main.cpp`
     re-applies through `applyPwmChannel()` only when the bit changed. The
     tab and `IHM_PWM_STATE` follow.
   - Relays: the same path as the press action (`relayState[]`,
     `multiOuput.getRelays()->setRelay`, `relay_instance` + dirty), but set
     rather than flipped. The switch, its LED and `IHM_RELAY_STATE` follow.
3. **pbRE1 on a switch keeps flipping it** (unchanged; pbRE0's role at L2
   is still open question 7).
4. **`demo/HARDWARE_RUNBOOK.md`** "UI navigation": switches follow RE2
   (CW on/inverting, CCW off/non-inverting), relays included.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes. Bench, over
COM3 with simulated encoders:
- With CH0 Enabled focused, RE2 CW gives `out1_enabled = 1`; a second CW
  changes nothing; CCW gives 0.
- The same for CH0 Inverting and for relay 0 (via `IHM_RELAY_STATE`).

The task file is marked done in the same commit.
