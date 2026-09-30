# MCP4725

Adafruit-derived 12-bit I2C DAC driver (vendored, lightly project-specific).

## API

- `begin(uint8_t i2c_address, TwoWire *wire = &Wire)` -- constructs an
  `I2CDevice` ([lib/BusIO](../BusIO/README.md)) and probes it. Called
  once from `setup()`.
- `setVoltage(uint16_t output, bool writeEEPROM, uint32_t
  dac_frequency = 400000)` -- temporarily bumps the I2C clock to
  `dac_frequency`, writes a 3-byte packet (command byte + 12-bit value
  split across 2 bytes), then resets I2C speed back to 100kHz.

## Hardware resources

Shared I2C/`Wire` bus. `main.cpp` instantiates two, `dac[0]` at address
`0x62` (header DAC0) and `dac[1]` at `0x63` (header DAC1) -- same bus,
address is the only differentiator. `setup()` sets
`Wire.setWireTimeout(5000 us, reset)` first: without it, a DAC that
doesn't answer hung `twi.c`'s wait loops forever (why these calls were
commented out until `dac_control`).

## Invocation

Not on any timer/tick handler. `setup()` probes each DAC and writes code 0;
after that the superloop writes a DAC only when an `IHM_DAC_COMMAND` (315)
arrives for it (initiative `dac_control`), never per pass. Each write's
result sets or clears that DAC's bit in `dacPresent`, reported in
`IHM_DAC_STATE` (316). Channel index = `dac[]` index = header number; the
old `voltage0`/`voltage1` path from `SerialCommunication` no longer drives
the DACs.

## Coupling

Depends on [lib/BusIO](../BusIO/README.md) (`I2CDevice`) and `Wire`
(stock Arduino framework). Output values come from `IHM_DAC_COMMAND`, range-checked (0-4095) in
`MavlinkComms::dispatch()`.
