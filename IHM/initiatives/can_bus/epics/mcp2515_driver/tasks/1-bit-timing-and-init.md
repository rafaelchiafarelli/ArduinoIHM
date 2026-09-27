# Task 1: bit-timing-and-init

**Status:** planned -- blocked on can_bus open question 1 (module, crystal)
**Depends on:** `spi_sharing/1`

## Contract

1. Pure `mcp2515BitTiming(crystalHz, bitrate) -> {cnf1, cnf2, cnf3}` for
   125/250/500/1000 kbit/s (sample point ~75-87.5 %, J1939 wants ~87.5 %),
   with native tests.
2. `Mcp2515` class per module (CS pin, INT pin): reset, configure,
   normal / listen-only / loopback modes, read status.
3. Bench: both modules pass a loopback self-test at boot; a failure is
   reported (debug Serial0), not a hang (lesson from the DAC `twi` hang).
