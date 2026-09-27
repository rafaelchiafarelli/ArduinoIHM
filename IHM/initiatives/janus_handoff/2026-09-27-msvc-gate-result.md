# Handoff to Janus: MSVC gate result for `desktop_windows_mirror`

Written 2026-09-27 by ArduinoIHM (`desktop_mirror` initiative,
`companion_mirror/1`). It answers the open half of Janus's
`desktop_windows_mirror` cross-epic gate: "the generated desktop runtime
builds and its `ctest` passes under MSVC". Nothing here changes Janus.
It is a report.

## Setup

- Janus `c56f14c` (dev), WSL Ubuntu-24.04.
  `janus-generate --scaffold-src SRC --mirror IHM/lib/GUI/app.yaml OUT`,
  with the app's `.jpg` images next to the yaml.
- Built `OUT/desktop/runtime` out of tree on Windows 11, VS 2022
  (`-G "Visual Studio 17 2022" -A x64`, Debug), SDL2 2.32 from vcpkg
  (`CMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake`),
  `SDL_VIDEODRIVER=dummy`.

## Result

**The MSVC fixes work.** Configure and build succeed with
`JANUS_BUILD_DESKTOP_DRIVER=ON` and `OFF`, with no hand patches.
`D8021`, `C2099` and the `SDL_main` link error are all gone.
`janus_desktop_driver_tests` passes.

**`ctest`: 11/13 pass (ON) and 10/12 pass (OFF).** The same two tests fail in both:

| Test | Failing checks |
|---|---|
| `janus_runtime_tests` | `test_runtime.c:613, 614, 639, 640, 712`: mock log count/sample pixel, painted colour `0x0777`, accent sample |
| `janus_clear_screen_bg_tests` | `test_clear_screen_bg.c:45-47`: painted colour `0x4321` |

**Cause (confirmed): the app's `janus_render_config.gen.h` is vendored
into `runtime/include/`.** For this app it defines
`JANUS_DISPLAY_PANEL_W 320`, `JANUS_DISPLAY_PANEL_H 480` and
`JANUS_DISPLAY_BACKGROUND 0x0000`. The runtime's own tests set their
fixture values with `-D` (for example `JANUS_DISPLAY_BACKGROUND=0x4321`,
`JANUS_DISPLAY_PANEL_W=48`). The header's `#define` wins, as MSVC's
C4005 "macro redefinition" warnings show, so the tests run against the
app's panel instead of their fixture. With that header deleted from a
copy of the same tree, **13/13 pass under MSVC**, driver included.

This is very likely **not MSVC-specific**. GCC also lets a later
`#define` override `-D` (with a warning). It only shows up when `ctest`
runs on a vendored runtime from an app that has a `display:` block.
Janus's own gate may run generated trees from fixtures without one.

## Suggested fix (Janus's call)

Either keep the app's `janus_render_config.gen.h` out of the vendored
`runtime/include/` (for example next to the other `.gen.h` in the app's
`include/`), or make the runtime tests immune to it: give the tests
their own include path that shadows it, or `#undef` the fixture macros
in the tests before redefining them. A test that runs `ctest` on a
generated tree whose app.yaml has a `display:` block would catch it.

## For ArduinoIHM

The companion does not build the runtime with CMake. It compiles the
sources straight into its vcxproj (`companion_mirror/2`), and there the
app's config header is what we want. So nothing is blocked on this.
