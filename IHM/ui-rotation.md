# UI rotation (portrait/landscape) -- planning notes

Not started yet. Written 2026-08-15 to seed a future session -- the user
wants to pick up the design decision (where the buffer/scheduler logic
lives) fresh, not settle it in the same session this was scoped in.

## The feature

The board has a housekeeping input pin that indicates physical mounting
orientation. The UI needs to match: portrait or landscape, and it should be
able to change **while the board is running**, not just read once at boot --
reorienting a live device (someone rotating it mid-use) is a real,
explicitly-wanted case, not just an install-time setting.

All 4 cardinal rotations are in scope (0/90/180/270), matching
`Display`/`GFX`'s existing `setRotation(uint8_t x)` API (`lib/Display/src/GFX.h`
-- already implemented, takes 0-3, computes `_width`/`_height` accordingly).
This also matches the shape of `GUI.h`'s old `RotateAngle` enum
(`PORTRAIT`/`LANDSCAPE`/`PORTRAIT_SIDEWAYS`/`LANDSCAPE_SIDEWAYS`) and its
`rotateGUI()` method -- both deleted earlier in the 2026-08-15 session as
confirmed-dead code (empty body, never called) during the broader GUI
simplification pass. That deletion was correct at the time (it really was
dead), but the user then recalled this is a real, still-wanted feature --
the enum/method shape was a genuine placeholder for exactly this, just never
implemented. Worth reviving that shape (or a redesigned equivalent) rather
than starting the naming from scratch.

## Hard constraint: no blocking redraws

This project has a standing rule (`ARCHITECTURE.md`): no `delay()`, no
blocking work in the superloop -- a hardware timer tick drives scheduling,
not wait loops. A rotation change means clearing and re-laying-out the whole
screen, which is expensive on this parallel-TFT driver (comparable to the
full draw that already happens once at cold boot). Doing that in one shot
would stall `main()`'s `while(1)` loop for its whole duration, freezing
encoder input and MAVLink polling for however long it takes.

**Agreed approach:** don't do the redraw in one shot. When a rotation-change
event fires, schedule it and execute it incrementally -- a few lines/rows at
a time, spread across multiple superloop passes -- the same way the rest of
this codebase avoids blocking work, just applied to a bigger job than any
existing one.

**Open problem, not yet designed:** repeated/rapid rotation events. The pin
could in principle flip back and forth (jitter, or someone genuinely
spinning the device) faster than one incremental redraw finishes. The
scheduler needs a defined behavior for "a new rotation request arrives while
the previous one is still mid-redraw" -- most likely superseding the
in-flight job rather than queuing multiple, but this hasn't been decided.
Worth deciding early since it shapes the whole data structure (a simple FIFO
of draw-steps won't handle supersession cleanly).

## What exists today (see `docs/gui-render-pipeline.drawio`)

A focused diagram (not the whole-project `docs/architecture.drawio`) was
built this session to reason about where the new logic should live --
descriptive of the current system only, deliberately not annotated with a
proposed design, so the placement decision stays open. Key facts from it:

- `GUI::update()` runs once per `main()` `while(1)` superloop pass, at
  whatever rate the loop happens to cycle -- the only "steady heartbeat"
  available in the foreground path. There's no ISR-driven UI cadence today
  (Timer2's 1ms/25ms branches drive input sampling and a few `*_handler()`
  calls, not `GUI::update()` itself).
- Existing widgets (`Label`/`LED`/`Icon`, `lib/Elements`) already do
  dirty-flag-gated partial draws (`isShown`, and `BlinkAnimator` for
  blinking states -- both from this session's earlier GUI-simplification
  pass) -- but that's single-tick, per-widget granularity. **Nothing in this
  codebase today spreads one multi-step job across multiple superloop
  passes.** Whatever gets built for rotation is a new pattern for this
  project, not a slot into an existing convention.
- The rotation pin is bit 4 of `bMap` (`BinaryInputs`' raw 16-bit input
  snapshot, AVR pin PA0, tagged "house-keeping" in `BinaryInput.h`'s pin
  table). It's already extracted every superloop pass today:
  `rotation = (bMap>>4) & 0x01` in `main.cpp`. Bit 5 (PA2) is a second,
  unrelated house-keeping input currently read as `charging`.
- That `rotation` value currently has exactly one consumer: it's bundled
  into the outgoing `IHM_BOARD_STATE` MAVLink message every ~100 ticks
  (`mavlinkComms.sendBoardState(...)`), read by the PC app
  (`workspace/IHM-PCApp`). **It never reaches `Display`/the on-board screen
  at all today.**
- `tft->setRotation(0)` is called exactly once, in `GUI::setup()`, at boot.
  Nothing else in the codebase calls `setRotation()` again after that -- the
  entire runtime rotation-switching path is unbuilt, not just the
  buffering/incrementalism part.
- Today's `rotation` bit is only 0/1 (one physical sensor bit), but the
  feature wants 4 angles. How a single bit maps to 4 rotation states isn't
  resolved -- worth clarifying whether there's a second sensor/bit not yet
  wired into `bMap`, or whether the mapping is meant to be something else
  (e.g. bit value + some other signal, or the hardware only genuinely
  distinguishes 2 states and "4 angles" means the *software* should support
  all 4 for flexibility even though only 2 are reachable via this pin
  today). Needs the user's input, not an assumption.

## Also unresolved, deferred from this session

Every screen's current layout (`TabSelector`, `PWMScreen`, `RelayScreen`,
`BusStatusScreen`, `StatusBar`) is hardcoded for one ~320-wide canvas (what
`setRotation(0)` produces). A true landscape (or any non-0) layout needs
each screen re-flowed for that shape, not just a coordinate transform --
this was raised earlier in the same conversation and explicitly deferred
pending the "once vs. live" decision (which resolved to "live"). Still
needs its own scoping pass: does every angle get a fully custom layout, or
is there a cheaper shared approach? Not decided.

## Suggested starting point for the next session

1. Resolve the bit-to-4-angles mapping question above with the user first --
   it affects everything downstream.
2. Decide the supersession behavior for rapid repeated rotation events.
3. Then place the incremental-redraw scheduler using
   `docs/gui-render-pipeline.drawio` as the reference for where it plugs
   into the existing superloop/`GUI::update()` flow.
4. Only then design the per-angle screen layouts (item above) -- pointless
   to do before the mechanism that will drive them is settled.
