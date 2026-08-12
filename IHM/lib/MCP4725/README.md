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

Shared I2C/`Wire` bus. `main.cpp` instantiates two: `dac0` at address
`0x62`, `dac1` at address `0x63` -- same bus, address is the only
differentiator.

## Invocation

Not on any timer/tick handler -- called directly from `main()`'s
superloop every pass:

```cpp
dac1.setVoltage(voltage0, false);
dac0.setVoltage(voltage1, false);
```

**Note the crossed naming:** `voltage0` (= `receivedRawData[0]`) goes to
`dac1`, and `voltage1` (= `receivedRawData[1]`) goes to `dac0`. This may
be intentional (matching physical wiring), but it reads like a mix-up and
is worth a deliberate double-check against the actual board wiring before
relying on "channel 0 = `voltage0`."

## Coupling

Depends on [lib/BusIO](../BusIO/README.md) (`I2CDevice`) and `Wire`
(ArduinoLib). Output values, once [Comms](../Comms/README.md)'s wiring
gap is fixed, come straight from the decoded serial frame -- no
clamping/range validation currently happens in `main.cpp` between
`receivedRawData` and `setVoltage()`.
