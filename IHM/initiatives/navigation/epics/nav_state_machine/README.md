# Epic: nav_state_machine

**Dropped 2026-09-27, never built.** Rafael kept the controls already on
`dev` (RE0 tabs, RE1 focus, RE2 edits) as the final design. See the
initiative README, "Closing decisions". The plan below is kept as a record.

The L0-L3 drill-down from the initiative README, as an IHM-side state
machine. It is split into three parts so the logic can be unit-tested
without Janus:

- **`Navigator`**: pure C++, knows only counts ("tab 0 has 4 options,
  option 2 has 4 settings"), never widget ids.
- **IHM nav table**: maps each (tab, option, setting) to its Janus widget
  and setting kind. Const data, lives in flash.
- **Wiring**: `main.cpp` feeds RE0 / pbRE0 / B0 / time into the
  `Navigator`, and turns its state changes into
  `janus_switch_screen` / focus calls.

## Tasks

```
1-navigator         pure Navigator + native tests                 (no deps)
2-ihm-nav-table     the IHM table for PWM / SERIAL / Output       (deps: 1; open questions 1, 2, 5)
3-wire-into-main    main.cpp drives Janus from Navigator;         (deps: 1, 2, input_map/1, input_map/2)
                    RE1/pbRE1 lose their nav role
```

## Acceptance gate

- `platformio run` builds; RAM stays within headroom (flag any jump
  above ~3 %, since the table should be PROGMEM).
- `test_native/run_tests.ps1` passes.
- Bench, over `sim_input.py --port COM3`: RE0 cycles tabs; pbRE0 enters
  options, then settings; B0 backs out one level; 4 s idle returns to
  tab selection.
