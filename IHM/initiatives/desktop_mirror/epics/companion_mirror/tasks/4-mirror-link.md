# Task 4: mirror-link

**Status:** done 2026-09-27 (no board: verified with posted messages, see below).

## Delivered

- Dialect: the companion's `mavlink/ihm_dialect/` re-synced with 308 from
  the `mirror_transport` branch (05b69cb; `mirror_transport` is not in
  `epics` yet, since its bench gate is still open). The CRC extra for 308
  (3) is the same on both sides. Backup: `mavlink/ihm_dialect.mirror-link.bak/`.
  The firmware's `mavlink/README.md` sync note now reads ids 300-308.
- `MavlinkLink.h`: `UiState`, `PackUiState`/`UnpackUiState` (16 bits
  per field, so it fits Win32), and the listener posts `WM_APP_UI_STATE`
  on 308.
- `MirrorView`: `MirrorApplyUiState` -> `janus_remote_state_apply`, which
  marks the link up. `MirrorTick` (main-window timer, 30 ms) runs
  `janus_render_screen_if_dirty` + repaint and brings the overlay back
  after 1.5 s without 308 (3 missed 500 ms heartbeats). `MirrorLinkReset`
  runs on connect and disconnect. The overlay covers the panel ("No board
  UI state" + what to check), so a stale screen is never shown. The
  mirror handles no input.
- `HOW_TO_USE.md`: "Screen mirror" section; window table, wire table
  (308), "When the protocol changes" (Janus regen of `include/`,
  re-copying `board/`), and two troubleshooting rows.
- Backups: `*.mirror-link.bak` for `IHMPCController.cpp`, `MavlinkLink.h`,
  `MirrorView.{h,cpp}`, `HOW_TO_USE.md`.

## Verification

- Debug x64 and Win32 build. `tests/run_mirror_bindings_test.cmd` passes,
  with a new 308 pack/unpack round trip (signed -1, all 16 bits).
- No board: the running app was fed the same `WM_APP_*` messages the
  listener posts (`PostMessage`), with a screenshot per step:
  `4-m1_overlay.png` (overlay before any 308), `4-m2_relay.png` (308
  screen 2 / focus 1 + relays 0x05: Output tab, ring on Relay 1, relays 0
  and 2 lit), `4-m3_pwm.png` (308 screen 0 / focus 0 + a 307 for ch0:
  PWM tab, ring on CH0's frequency label, `F:15625Hz`, enabled, 25 %,
  "inverted"), `4-m4_timeout.png` (overlay back after 2 s without 308).
- Not covered here: the serial decode of a real 308 frame (the generated
  code is round-trip tested in the firmware's native suite) and the
  comparison with the TFT. Both are task 5, on the bench.
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
