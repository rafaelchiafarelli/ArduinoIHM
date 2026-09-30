# Task 1: DAC command, readback and companion panel

**Status:** done 2026-09-29 -- builds and native tests pass; **not
bench-verified** (the DAC outputs haven't been measured on the new board)
**Depends on:** nothing beyond `dev` (MAVLink on Serial2, `lib/MCP4725`)

## Contract

1. **Wire:** `IHM_DAC_COMMAND` (315, PC -> board: `channel` 0-1, `value`
   0-4095; anything else dropped) and `IHM_DAC_STATE` (316, board -> PC:
   `value[2]` last code written, `present` bit i = DAC i ACKed its last
   write), every ~100 ms. `mavlink/generated*` regenerated.
2. **Firmware:** `MavlinkComms` stores the latest command per DAC
   (`takeDacCommand`) and packs 316 (`sendDacState`). `main.cpp` sets a
   Wire timeout, probes both DACs and writes 0 in `setup()`, then writes a
   DAC only when a command for it arrives.
3. **Bench script:** `mavlink/scripts/dac_cmd.py --port COMx --channel N
   --value CODE` sends 315 and prints the 316 readback.
4. **Companion** (`C:\Users\rafae\source\repos\IHMPCController`, not under
   git; backups `*.dac-control.bak`, dialect `mavlink/ihm_dialect.dac-control.bak`):
   `mavlink/ihm_dialect/` re-vendored; `MavlinkLink.h` gains
   `SendDacCommand` and `WM_APP_DAC_STATE`; `IHMPCController.cpp` gains the
   DAC panel below SERIAL (slider + code box + Set per DAC, readback line);
   `HOW_TO_USE.md` has "Driving the DACs".
5. Native tests: `test_native/test_dac_messages.cpp` (ids, round trips).

## Bench steps owed

- Flash the new board, connect the companion on the MAVLink port, set
  DAC0/DAC1 to 0, 2048, 4095 and measure each output (at the DAC pin and
  after the amplifier).
- If a readback says "DAC NOT answering", check the module's address
  (Adafruit-style modules are 0x62/0x63 by A0, others 0x60/0x61).
