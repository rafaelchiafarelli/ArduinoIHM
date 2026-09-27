# Task 2: gdi-display-driver

**Status:** planned
**Depends on:** task 1

## Contract

1. `JanusGdiDriver.{h,c}` in the companion implements the fixed runtime's
   driver contract (`draw_area_sync`, `draw_area_async`, `display_busy`,
   as declared in `janus_runtime.h`) into a
   `JANUS_DISPLAY_WIDTH x JANUS_DISPLAY_HEIGHT` (320x480) RGB565 buffer.
   `draw_area_async` completes synchronously and `display_busy` returns
   false. It also has a dirty-rect flag so the host knows to repaint.
2. The placeholder `STATIC` becomes an owner-drawn child (or a small
   custom window class) that paints the buffer with `StretchDIBits`
   (RGB565 via `BI_BITFIELDS`), scaled to fit, aspect kept, integer scale
   if it fits.
3. The vcxproj compiles the vendored runtime `.c` files, the generated
   screen `.gen.c` files and `janus_remote.c` as C, with no SDL.
   Bindings instances are the generated defaults.
4. At startup the companion renders screen 0 + status bar + nav bar once,
   so the placeholder shows the board's boot screen with default values.

## Verification

Build x64 + x86. A DPI-aware screenshot of the window, driven as in
`serial_commands/epics/pc_companion/tasks/3`, shows screen 0 next to the
TFT's boot screen.
