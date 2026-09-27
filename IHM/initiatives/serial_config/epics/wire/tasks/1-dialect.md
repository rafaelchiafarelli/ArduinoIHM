# Task 1: dialect

**Status:** planned
**Depends on:** `config_model/1`

## Contract

1. `mavlink/ihm_dialect.xml`: a PC -> board message for one bus's
   parameters (bus id, bitrate/baud, parity, protocol mode, J1939
   source address) and a board -> PC message carrying one bus's full
   config (parameters + generator), sent round-robin like
   `IHM_PWM_STATE`. Next free ids (309+; 308 is `IHM_UI_STATE` on the
   desktop_mirror branches, so coordinate if that hasn't reached `dev`).
2. Regenerate `mavlink/generated` and `generated_py`; update
   `mavlink/README.md` (message table, TX budget per tick).
3. Native pack/decode round-trip tests.
