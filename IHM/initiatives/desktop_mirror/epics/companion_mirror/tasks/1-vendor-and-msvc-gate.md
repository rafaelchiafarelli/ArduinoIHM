# Task 1: vendor-and-msvc-gate

**Status:** planned
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
