# Task 1: screen-fields

**Status:** planned -- blocked on initiative open question 1 (row layout rule)
**Depends on:** `config_model/1` (field list)

## Contract

1. `lib/GUI/bus_status.screen.yaml`: each bus's row laid out per the row
   layout rule (open question 1, including where data bytes and repeat
   count go), with every editable generator value in its own
   `focus_ring` row, so RE1 can select it; `align: center` rows as today.
2. New `bus_status` binding fields for period, repeat, and anything the
   row rule moves (data bytes).
3. The row rule written down where bus initiatives will find it (the
   screen yaml's header comment), including the space left for their
   fields and how a mode-dependent row is built.
4. `lib/GUI` regenerated (procedure in the Janus-regen notes); the
   companion's `include/` re-vendored from the same Janus commit.
5. Widest row stays inside the 320 px panel (check the generated
   geometry).
