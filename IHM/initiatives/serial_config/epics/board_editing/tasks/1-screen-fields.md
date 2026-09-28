# Task 1: screen-fields

**Status:** planned
**Depends on:** `config_model/1` (field list)

## Contract

1. `lib/GUI/bus_status.screen.yaml`: two rows per bus (initiative
   question 1), every editable generator value in its own `focus_ring`
   row so RE1 can select it; the enable switch is a `toggle`; `align:
   center` rows as today.
2. New `bus_status` binding fields for period, repeat, byte index and
   byte value; the CAN ID becomes `int64` (a 29-bit ID doesn't fit AVR's
   16-bit `int`).
3. The row rule written down where bus initiatives will find it (the
   screen yaml's header comment), including the space left for their
   fields and how a mode-dependent row is built.
4. `lib/GUI` regenerated (procedure in the Janus-regen notes); the
   companion's `include/` re-vendored from the same Janus commit.
5. Widest row stays inside the 320 px panel (check the generated
   geometry).
