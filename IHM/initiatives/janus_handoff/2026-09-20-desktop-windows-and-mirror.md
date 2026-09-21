# Handoff to Janus: desktop target on Windows + "mirror" mode

Written 2026-09-20. Same purpose as [README.md](README.md): items for a
session working in Janus directly. Nothing here changes ArduinoIHM firmware.

Source of findings: the desktop target's output vendored into the PC
companion (`C:\Users\rafae\source\repos\IHMPCController\include\`), built on
Windows 11 / VS 2022 (MSVC) / SDL2 2.32.10 from vcpkg. **Note:** the
`C:\Users\rafae\workspace\janus` checkout on this machine is at `53076f1`
(2026-08-22) and predates the desktop target (no `driver/`, no
`host_mock/`), so it is NOT the Janus that produced that output -- locate
the templates in whichever checkout you generate from.

## 1. Desktop runtime does not build/test on Windows (3 issues) -- patched in the vendored copy only

After the three fixes below, a clean out-of-tree build passes 12/12 ctest
(including `janus_desktop_driver_tests` under `SDL_VIDEODRIVER=dummy`). The
fixes were applied by hand to the vendored copy, so **the next
`janus-generate` will overwrite them** unless the generator is fixed.

1. **GCC-only warning flags.** `runtime/CMakeLists.txt` has 14 x
   `target_compile_options(<t> PRIVATE -Wall -Wextra)`. MSVC fails with
   `D8021: invalid numeric argument '/Wextra'` (a hard error, not a
   warning) for `janus_runtime` and `janus_clear_screen_bg_tests`, so
   nothing builds. Fix: `$<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall;-Wextra>`
   (optionally `/W3` for MSVC). Same pattern exists in the embedded_c
   CMakeLists and `examples/host_demo`.
2. **Non-constant static initializer.** `tests/test_runtime.c` ~line 469
   (`test_switch_screen_erase_covers_gaps_and_ragged_edges`):
   `static const janus_widget_desc_t s1_widgets[] = { s1_a, s1_b };`
   copies other const objects into a static initializer -- a GCC
   extension; MSVC C2099. Fix: write the two initializers inline (or make
   `s1_a/s1_b` macros).
3. **SDL_main clash.** `tests/test_desktop_driver.c` defines `int main`
   after `#include <SDL.h>`; on Windows SDL.h `#define`s `main` to
   `SDL_main`, so the exe fails to link (`LNK2019 unresolved main`).
   Fix: `#define SDL_MAIN_HANDLED` before `#include <SDL.h>` (or link
   `SDL2::SDL2main`). Same applies to any generated desktop demo `main`.

Also: `find_package(SDL2 REQUIRED)` is unconditional, so even the mock-only
tests need SDL2 installed. Consider making the driver/its test optional
(`JANUS_BUILD_DESKTOP_DRIVER`) so the runtime + mock tests build without it.

## 2. Requested: "mirror" mode for the desktop target

**Requirement (from the user):** the desktop screen must *mirror what the
physical board is showing*. Keyboard/mouse input on the PC must NOT change
the screen. The PC app is a viewer plus a command sender; the board is the
single source of UI truth (which screen is active, which widget has focus,
what the bound values are).

What Janus would need to offer (design is yours; this is the shape we need):

- A desktop mode/flag where the SDL driver's input events (keyboard, mouse,
  touch) are **not** routed into `janus_focus_move` / `janus_focus_activate`
  / `janus_switch_screen`. Closing the window etc. still works.
- A small public "apply remote state" surface so a host can drive the same
  runtime from outside: set active screen, set focus (by widget id/index),
  push new bound-struct values and re-render. `janus_switch_screen`,
  `janus_set_focus` and the bindings struct already exist; the gap is a
  supported, documented way to call them from a non-input source without
  side effects (e.g. no local `janus_handle_action` firing).
- Rendering must stay a pure function of (active screen, focus, bound
  values) so the mirror is pixel-comparable to the board's TFT.

ArduinoIHM will own the transport: a new board -> PC MAVLink message with the
active screen and focus (today telemetry carries neither), plus the bound
values. Please tell us what shape of "remote state" Janus wants so we
serialize exactly that.
