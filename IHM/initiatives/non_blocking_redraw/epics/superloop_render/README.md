# Epic: superloop_render

Use the stepped render from `main.cpp` so no pass blocks more than 20 ms,
then run the initiative's cross-epic gate.

## Tasks

```
1-step-in-superloop   budgeted render step each pass; every blocking call site converted   (deps: janus_render/1, loop_timing/1; open questions 2, 3)
2-bench-gate          the initiative's cross-epic gate on the bench                          (deps: 1)
```

## Acceptance gate

The initiative's cross-epic gate (this epic is the last to land).
