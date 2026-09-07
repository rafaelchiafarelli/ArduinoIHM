# Task 5: pwm-tab-reflects-mavlink

**Status:** not started
**Branch:** `5-pwm-tab-reflects-mavlink` (from `tasks`)
**Depends on:** task 3 (the receive+apply path this hooks into)

## Contract

### In

The per-channel dirty-config loop added to `src/main.cpp` by task 3, and
the generated `pwm_instance` / `pwm_dirty` structs
(`lib/GUI/include/janus_bindings.gen.h`).

### Delivers

After task 3's loop applies a config to the timers, it also mirrors that
config into `pwm_instance` and sets the matching `pwm_dirty.*` bits, then
triggers a repaint of the PWM screen -- so the on-screen PWM tab tracks
configs pushed from the PC, not just ones entered on the encoders.

1. **Field mirror** (`main.cpp`, in the same `switch (ch)`), using
   `PwmResolvedArgs a`:

   | channel | `pwm_instance` fields to set | `pwm_dirty` bits |
   |---|---|---|
   | 0 | `ch0_enabled = a.enabled[0]`, `ch0_duty_percent = a.duty_percent[0]`, `ch0_inverting = a.inverting[0]` | `ch0_enabled`, `ch0_duty_percent`, `ch0_inverting` |
   | 1 | `ch1_*` likewise | `ch1_*` likewise |
   | 2 | `ch2_a_enabled`/`ch2_a_duty_percent`, `ch2_b_*`, `ch2_c_*` | matching bits |
   | 3 | `ch3_a_*`, `ch3_b_*`, `ch3_c_*` | matching bits |

   `pwm_instance` has **no** `inverting` field for complex channels and
   **no** `f_selector` field for any channel -- mirror only what exists.

2. **`chN_state_label`** (`const char*`): map `a.f_selector` to a short
   static string (`PROGMEM` table or a `switch` returning string
   literals) -- e.g. `"62.5kHz"`, ..., `"VARIABLE"`. Set
   `pwm_instance.chN_state_label` + its dirty bit. If the implementing
   session judges the label mapping is more than ~15 lines or wants a
   format the current UI can't show, set the enabled/duty/inverting
   mirror only and leave `state_label` untouched -- note it in the
   commit, don't force it.

3. **Repaint.** A config can arrive on any superloop pass, not just the
   ~100 ms telemetry tick. Follow the relay action path in `main.cpp`
   (`janus_handle_action` -> `janus_render_screen(screen)`): after the
   dirty loop, if any channel was updated **and** the active screen is
   the PWM screen, call `janus_render_screen` for it (or
   `janus_render_widget` per changed row if that's already the finer
   pattern in use). Do not add a second unconditional full-screen redraw
   to the ~100 ms block -- that regression is called out in `main.cpp`
   and `IHM/NEXT-SESSION.md`.

## Dependencies

Task 3 merged into `tasks`.

## Pre-work

None.

## Definition of done

- `platformio run` builds, RAM within headroom (flag a material jump --
  the `state_label` strings add a little Flash/PROGMEM).
- `test_native/run_tests.ps1` passes.
- On a connected board: a `pwm_config.py` send visibly updates the PWM
  tab (LED / duty bar / label) while that tab is showing. Rafael's bench
  step.
- Task file marked done in the same commit.

## Notes

- This task touches **only** `src/main.cpp` glue. Do not hand-edit
  anything under `lib/GUI/include/*.gen.h` or `lib/GUI/src/*.gen.c` --
  those are Janus output.
- If the mirror needs a `pwm_instance` field that doesn't exist (e.g.
  per-channel frequency, complex-channel inverting), that's a Janus
  re-run (`lib/GUI/*.screen.yaml` + `app.yaml` edit, then regenerate) --
  out of scope here. Stop and flag it as a separate task.
- Conventional-commit: `feat(ui): reflect MAVLink PWM configs on the PWM tab`.
