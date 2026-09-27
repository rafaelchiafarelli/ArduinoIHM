# Epic: companion_panel

IHMPCController (not under git; task files are the record, `.bak` next
to each edited file): a SERIAL panel, and the screen mirror's SERIAL tab
fed from the board's config telemetry instead of generated defaults
(closes `desktop_mirror` open question 3).

## Tasks

```
1-serial-panel      CAN0/CAN1/RS-485 controls; send 301/302 + bus params; show readback   (deps: wire/2)
2-mirror-bindings   bus_status bindings in the mirror from the config telemetry          (deps: wire/2, board_editing/1)
```

## Acceptance gate

A config sent from the panel shows on the TFT and in the readback; an
edit made on the board with the knobs shows in the panel and the mirror.
