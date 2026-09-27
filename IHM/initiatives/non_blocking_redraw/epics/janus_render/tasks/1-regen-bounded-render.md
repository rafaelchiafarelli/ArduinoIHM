# Task 1: regen-bounded-render

**Status:** planned -- waiting on Janus (handoff
`initiatives/janus_handoff/2026-09-27-bounded-nonblocking-render.md`)
**Depends on:** a Janus commit that meets the handoff's requirements.

## Contract

### Delivers

1. **`lib/GUI/app.yaml`**: the new render mode enabled (the exact key is
   whatever Janus ships).
2. **`lib/GUI` regenerated** with that Janus commit (procedure: WSL janus +
   copy `embedded_c`, then delete `.pio/libdeps/megaatmega2560`).
   `main.cpp` is untouched here: the blocking calls keep working, since the
   handoff requires the blocking API to stay.
3. **Companion**: `IHMPCController/include/` regenerated from the same
   commit (desktop target, `--mirror`), `include/JANUS_COMMIT` updated,
   `.bak` of the old folder kept.
4. RAM before/after recorded here. Flag a jump > 3 %.

## Definition of done

`platformio run` builds; `test_native/run_tests.ps1` passes; companion
builds x64 and x86 Debug; the board and the mirror look unchanged (the
blocking path is still in use); task file marked done in the same commit.
