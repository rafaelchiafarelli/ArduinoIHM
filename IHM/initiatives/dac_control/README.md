# Initiative: dac_control

The two `MCP4725` analog outputs (headers DAC0/DAC1), driven from the PC
over MAVLink, for bringing up the new hardware revision.

Asked by Rafael 2026-09-29 as bench test code ("add the DAC output back,
and change the PC companion so I can load a value in the DAC"), with
everything else on the board already connected and left as it is. One
epic, one task; no on-screen (TFT) control -- that would be a new task.

## Decisions (made while implementing, 2026-09-29 -- check them)

1. **Channel i = `dac[i]` = header DACi**, at 0x62 / 0x63 (the addresses
   the old commented-out code used). The old crossed wiring
   (`dac1 <- voltage0`) is gone, and the unused `voltage0/1` path from
   `SerialCommunication` no longer drives the DACs.
2. **Raw 12-bit code on the wire**, not volts: the board doesn't know its
   DAC supply or what the amplifier after it does. The companion shows
   volts assuming a 5 V supply, at the DAC pin.
3. **Register only, never the DAC's EEPROM**; the board writes 0 to both
   at boot, so a reset always starts the outputs at 0 V.
4. **`Wire.setWireTimeout(5 ms, reset)`** instead of the old bypass, so a
   missing DAC fails its write and is reported ("not answering") instead
   of hanging `setup()`.
