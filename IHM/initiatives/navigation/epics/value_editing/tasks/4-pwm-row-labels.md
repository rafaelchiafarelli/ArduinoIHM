# Task 4: pwm-row-labels

**Status:** done 2026-09-27, as **`fixes/00000a/pwm-ch01-labels`** off
`tasks`, merged back into `tasks`. It was created as `fixes/000009/...`, but
another session had already used 000009 on `dev`
(`fixes/000009/janus-shared-field-dirty`), so it was renamed before the
first push. Commit c3c4541 and merge be65aa0 still say 000009. Rafael chose a fix branch over a task
branch. This file is the record the workflow skill asks for when that
happens.
**Depends on:** `value_editing/2` (the duty bars' focus rings, which this
builds on).

Rafael, 2026-09-27, after the companion's VARIABLE slider reached 8 MHz:
"add another 2 characters in the label for pwm0 and 1 and put one fixed
label in front of the duty cycle: Duty: for pwm0 and 1; don't change
anything on the other pwms."

## Delivered (`lib/GUI/pwm.screen.yaml`, regenerated with Janus dev c56f14c)

1. **CH0/CH1 frequency label 90 -> 110 px**: 11 characters at the medium
   font, enough for `F:8000000Hz`. Anything above 99 999 Hz used to be
   cut off.
2. **"Duty:" caption** (static label, 50 px) in front of the CH0/CH1 duty
   bars. The duty bar's own focus ring is unchanged, so the ring still
   surrounds only the bar RE2 edits.
3. CH2/CH3 unchanged. Generated geometry: CH0's Inverting ring ends at
   x 314 (panel width 320), and row heights are unchanged (CH1 still
   starts at y 134).

## Checks

`platformio run` builds (RAM 59.0 %); 132 native tests pass; flashed on the
bench board. Rafael's on-screen check is still to do.
