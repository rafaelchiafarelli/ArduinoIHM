# BusIO

Vendored Adafruit BusIO: generic I2C/SPI transport (`I2CDevice`,
`I2CRegister`, `SPIDevice`, `BusIO_Register`, `GenericDevice`). Not
project-specific logic -- included here only because it's a real,
shared dependency of two otherwise-unrelated modules:

- [MCP4725](../MCP4725/README.md) uses `I2CDevice` for the DAC bus.
- `lib/Display`'s `GFX.h` includes `SPIDevice.h`/`I2CDevice.h`, though
  the actually-used parallel-TFT driver doesn't exercise the SPI path
  (see [Display/README.md](../Display/README.md)) -- `SPIDevice` is the
  piece that will matter once the planned SPI-display hardware revision
  happens.

`SPIDevice.cpp` already auto-selects direct port-register I/O
(`BUSIO_USE_FAST_PINIO`) over `digitalWrite` for the SCK/MOSI/MISO lines
on AVR targets -- the CS pin and `begin()`'s pin-mode setup are the
remaining `digitalWrite`/`pinMode` call sites in this file, worth
revisiting when the SPI display work actually starts.
