# Task 1: bound-the-loop

**Status:** planned -- blocked on can_bus open question 4 (timing strategy)
**Depends on:** nothing (but `mcp2515_driver/2` covers the ISR side)

## Contract

1. If Janus non-blocking render is chosen: `app.yaml`
   `display.render_mode: non_blocking`, `main.cpp` calls
   `janus_render_poll()` each pass and uses the `*_async_start` calls;
   regen `lib/GUI` and re-vendor the companion.
2. Measure and record the worst-case loop period before and after.
3. Side effect to check: `IHM_UI_STATE` (308) no longer pauses during
   screen switches (the 1.97 s gap seen on 2026-09-27).
