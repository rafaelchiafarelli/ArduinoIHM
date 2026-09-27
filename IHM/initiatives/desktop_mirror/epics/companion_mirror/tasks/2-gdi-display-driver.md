# Task 2: gdi-display-driver

**Status:** done 2026-09-27.

## Delivered

- New companion files: `JanusGdiDriver.{h,c}` (C; the three contract
  functions into a 320x480 RGB565 buffer, clipped, with a dirty flag) and
  `MirrorView.{h,cpp}` (window class `IHMJanusMirror`: `StretchDIBits`
  with `BI_BITFIELDS` 565 masks, top-down; integer scale with
  `COLORONCOLOR` when the panel fits, aspect-kept `HALFTONE` downscale
  otherwise; `MirrorRenderBoot()` = the board's `setup()` render order;
  `MirrorPresentIfDirty()`).
- `IHMPCController.cpp`: the placeholder `STATIC` is now the mirror view,
  moved to its own full-height column right of the PWM panel
  (`kMirrorX`). The default window size fits it at 1:1
  (`AdjustWindowRect`). WM_CREATE renders the boot screen. The dead
  `WM_CTLCOLORSTATIC` placeholder branch is removed.
- vcxproj: the 7 runtime `.c` and 5 generated `.gen.c` are compiled as C
  in all 4 configurations (include dirs `include/include`,
  `include/runtime/include`), plus a "janus" filter. No SDL.
- Backups: `IHMPCController.cpp.mirror-gdi.bak`,
  `IHMPCController.vcxproj{,.filters}.mirror-gdi.bak`.

## Verification

Debug x64 and Win32 build. A full x64 rebuild shows no warnings from the
new or Janus files. Screenshot `2-boot-render.png`: the PWM screen,
status bar and nav strip render in the mirror column with default
(zero) bindings, before any board data. Side-by-side with the TFT is
task 5.
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
