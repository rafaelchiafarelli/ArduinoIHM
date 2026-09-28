# Task 1: screen-fields

**Status:** done (2026-09-27)
**Depends on:** `config_model/1` (field list)

## Contract

1. `lib/GUI/bus_status.screen.yaml`: two rows per bus (initiative
   question 1), every editable generator value in its own `focus_ring`
   row so RE1 can select it; the enable switch is a `toggle`; `align:
   center` rows as today.
2. New `bus_status` binding fields for period, repeat, byte index and
   byte value; the CAN ID becomes `int64` (a 29-bit ID doesn't fit AVR's
   16-bit `int`).
3. The row rule written down where bus initiatives will find it (the
   screen yaml's header comment), including the space left for their
   fields and how a mode-dependent row is built.
4. `lib/GUI` regenerated (procedure in the Janus-regen notes); the
   companion's `include/` re-vendored from the same Janus commit.
5. Widest row stays inside the 320 px panel (check the generated
   geometry).

## Result

- Two rows per bus, 22 focusable fields (actions `toggle_<bus>_enabled`,
  `edit_<bus>_{id,dlc,extended,period,repeat,byte_index,byte_value}`,
  `edit_rs485_length`). Widths had to be tight: each `focus_ring` wrapper
  adds 12 px and each gap 4 px, so the CAN row 2 texts are `EXT1`,
  `100ms`, `x5` / `x inf`, `B7`, `=ff` (318 px); the first try with full
  captions was 362 px and Janus refused it.
- Bindings: CAN ID and period are `int64` (AVR `int` is 16-bit); repeat is
  a string (`x inf` for 0). Row rule written in the yaml header.
- `lib/GUI` regenerated with Janus fcc4ed3 (unchanged runtime); only the
  SERIAL screen, bindings, actions and `.harpia` changed. `main.cpp`'s
  `mirrorSerialToUi()` fills the new fields; the byte index is UI state
  (`serialByteIndex[]`), clamped into DLC / LEN.
- RAM 61.1 % -> 62.3 % (+96 B), flash 31.5 % -> 32.9 %. 174 native tests
  pass.
- **Companion** (`C:\Users\rafae\source\repos\IHMPCController`, not under
  git): `include/include/janus_actions.gen.h`,
  `include/include/janus_bindings.gen.h`,
  `include/src/busstatus_screen.gen.c`, `include/janus_generated.harpia`
  replaced from the same Janus run (desktop target, `--mirror`; the
  embedded output of that run was byte-identical to `lib/GUI`), and
  `include/JANUS_COMMIT` updated. Backups: `*.serial-config.bak` beside
  each. x64 and x86 Debug compile and link (checked into a scratch output
  dir: the running companion held `x64\Debug\IHMPCController.exe`). The
  mirror's SERIAL tab shows generated defaults until `companion_panel/2`.
