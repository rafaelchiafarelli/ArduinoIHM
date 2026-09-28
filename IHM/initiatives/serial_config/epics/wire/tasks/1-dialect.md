# Task 1: dialect

**Status:** planned -- blocked on initiative open question 7 (how extension settings travel)
**Depends on:** `config_model/1`

## Contract

1. `mavlink/ihm_dialect.xml`: a board -> PC message carrying one bus's
   generator config, sent round-robin like `IHM_PWM_STATE`. PC -> board
   generator settings stay on 301/302. Next free id: check `dev` (309+).
2. The extension mechanism per open question 7: either the generic
   key/value pair (PC -> board set, board -> PC readback) with the key
   table documented in `mavlink/README.md` for bus initiatives to add to,
   or, if (a) is chosen, only a documented recipe for adding a message
   pair.
3. Regenerate `mavlink/generated` and `generated_py`; update
   `mavlink/README.md` (message table, TX budget per tick).
4. Native pack/decode round-trip tests.
