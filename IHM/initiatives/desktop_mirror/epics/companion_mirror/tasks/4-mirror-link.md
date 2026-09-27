# Task 4: mirror-link

**Status:** planned
**Depends on:** task 3, `mirror_transport/1` (message 308 in the dialect;
the companion's `mavlink/` headers are re-synced per `HOW_TO_USE.md`'s
dialect-sync procedure)

## Contract

1. On `IHM_UI_STATE` (308), the UI thread fills a `janus_remote_state_t`
   and calls `janus_remote_state_apply(&janus_app, &s)` (a no-op when
   unchanged, by Janus's contract).
2. A UI timer (~30 ms) calls `janus_render_screen_if_dirty` on the active
   screen, then invalidates the placeholder if the driver's dirty flag is
   set. This is the companion's version of the SDL mirror `main.c` loop.
3. Before the first 308 after connect, and after 3 missed heartbeats,
   the placeholder shows a "no board UI state" overlay instead of a
   stale screen.
4. Nothing in the mirror area takes input.
5. `HOW_TO_USE.md`: a "Screen mirror" section (what it shows, where the
   data comes from, the SERIAL-tab caveat, the no-signal overlay).
