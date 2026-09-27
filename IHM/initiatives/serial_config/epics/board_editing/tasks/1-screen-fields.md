# Task 1: screen-fields

**Status:** planned -- blocked on initiative open question 1 (what fits on a row)
**Depends on:** `config_model/1` (field list)

## Contract

1. `lib/GUI/bus_status.screen.yaml`: each bus's row (plus whatever open
   question 1 settles for data bytes / repeat count) with every editable
   value in its own `focus_ring` row, so RE1 can select it; `align:
   center` rows as today.
2. New `bus_status` binding fields for bitrate, protocol, J1939 source
   address, period, repeat, and string fields where a value is shown as
   text (e.g. "250k", "J1939").
3. `lib/GUI` regenerated (procedure in the Janus-regen notes); the
   companion's `include/` re-vendored from the same Janus commit.
4. Widest row stays inside the 320 px panel (check the generated
   geometry).
