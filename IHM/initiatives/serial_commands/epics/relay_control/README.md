# Epic: relay_control

PC observes the board's 8 relay outputs over MAVLink, so a bench tool
(the PC-side controller app) can show live on/off state for each relay
without a physical look at the board.

## Scope of this first cut: telemetry only, not command

Relays are toggled two ways already -- the physical buttons and the
on-screen Relay tab (`lib/GUI/relay.screen.yaml`, `janus_actions.cpp`) --
both of which flip `main.cpp`'s `relayState[NUMBER_OF_RELAYS]` directly.
There is no PC -> board relay command today, and this epic's first task
does not add one: it only adds the reverse direction, board -> PC, so a
PC-side observer can see ground truth (including relay changes made
locally, not just ones it caused itself) the same way `IHM_BOARD_STATE`
already lets the PC observe buttons/encoders it doesn't control either.

PC -> board relay commands (actual "control") are a plausible follow-up
task once this telemetry direction exists, but are explicitly out of
scope here -- see `initiatives/serial_commands/README.md`'s original
framing of this epic as "relay on/off over MAVLink" for that larger,
not-yet-scoped goal.

## The one message

`IHM_RELAY_STATE`, id 304, board -> PC. Mirrors how `IHM_BOARD_STATE`'s
`buttons` field already packs multiple booleans into one bitmask byte.

| Field | Type | Meaning |
|---|---|---|
| `relays` | `uint8_t` | Bitmask, bit *i* = relay *i* (0-7), 1 = on. Mirrors `main.cpp`'s `relayState[NUMBER_OF_RELAYS]`. |

1-byte payload -- cheapest message in the dialect.

## Tasks

```
1-relay-state-message   dialect + regenerate + main.cpp send   (no deps)
```

Sent on the same ~100ms cadence as `IHM_BOARD_STATE` (see the
`timeCounter>=100` block in `main.cpp`'s loop) -- no new timing
mechanism needed.

## Acceptance gate

- `platformio run` (env `megaatmega2560`) builds, RAM within headroom.
- `test_native/run_tests.ps1` passes.
- A PC-side MAVLink listener on a connected board sees `IHM_RELAY_STATE`
  frames and the bitmask matches the board's actual relay state after
  toggling a relay from the physical UI.
