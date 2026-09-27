# Epic: mirror_transport

Board -> PC transport for the mirror's UI state. Bound values already
travel in 304/307 (initiative decision 2). This epic adds only the
Janus remote UI state.

## Tasks

```
1-ui-state-message   dialect id 308 + MavlinkComms::sendUiState       (no deps)
2-ui-state-send      main.cpp: get -> send on change + heartbeat       (deps: 1; open question 1)
```

## Acceptance gate

- `platformio run` builds; `test_native/run_tests.ps1` passes.
- `mavlink/scripts` (pymavlink) on COM3 shows `IHM_UI_STATE` changing
  as RE0 switches screens and RE1 moves focus, plus the heartbeat while idle.
