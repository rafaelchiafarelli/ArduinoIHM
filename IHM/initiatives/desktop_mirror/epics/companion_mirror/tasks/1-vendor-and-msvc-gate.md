# Task 1: vendor-and-msvc-gate

**Status:** done 2026-09-27.

## Delivered

- Generated with WSL Janus c56f14c from `lib/GUI` (yaml + `.jpg` images
  copied to a scratch dir; without the images Janus bakes magenta
  placeholders, and the output would differ from the firmware's). The
  generated `include/`, `src/` and runtime are **byte-identical** to the
  firmware's committed `lib/GUI` (and the desktop tree's screens equal
  the embedded_c tree's).
- Companion: `include/` replaced by the desktop output. Old tree kept as
  `include.2026-09-19.bak/`. `include/JANUS_COMMIT` records the source.
  `include/mirror_scaffold_reference/` holds the scaffolded SDL `main.c`,
  `mirror_link.c` and `CMakeLists.txt`, for reference only. The vcxproj
  and filters' six `ClInclude` paths now point at `include\include\*.gen.h`
  (backups `*.mirror-vendor.bak`). MSBuild Debug|x64 builds.
- MSVC gate: build OK with the driver ON and OFF. `ctest` passes 11/13
  and 10/12. The 2 failures are caused by the app's
  `janus_render_config.gen.h` shadowing the runtime tests' `-D` fixtures,
  not by MSVC: with it removed, 13/13 pass. Reported in
  `initiatives/janus_handoff/2026-09-27-msvc-gate-result.md`.
**Depends on:** nothing

## Why

The companion's `include/` is a 2026-09-19 desktop output. It predates
focus rings, the dirty sweep, `janus_remote`, and the MSVC fixes. Janus's
`desktop_windows_mirror` gate is also still open on its Windows half:
"the consumer must confirm the generated desktop runtime builds and
passes `ctest` on Windows 11 / VS 2022".

## Contract

1. Generate the `desktop` target from `lib/GUI/app.yaml` with the WSL
   Janus at **the same commit as `lib/GUI`'s last regen** (c56f14c as of
   2026-09-27; check `git log -- lib/GUI` first). Use `--mirror`. The
   scaffolded SDL `main.c` / `mirror_link.c` are for reference only and
   are not vendored (initiative decision 1).
2. Replace the companion's stale `include/` tree with the generated
   headers, sources and runtime. Back up the old tree as
   `include.2026-09-19.bak/`. Record the Janus commit in a
   `include/JANUS_COMMIT` file.
3. **MSVC gate for Janus:** configure + build the vendored runtime with
   CMake (VS 2022 generator) with `JANUS_BUILD_DESKTOP_DRIVER=ON` (SDL2
   from vcpkg, as in the 2026-09-20 handoff), and run `ctest`. Then do
   the same with `OFF`. Write the result (pass/fail per test, any hand
   patch needed) to
   `initiatives/janus_handoff/<date>-msvc-gate-result.md`. A failure is
   reported, not patched in the vendored copy.
4. The companion itself still builds unchanged (nothing links the new
   sources yet).
