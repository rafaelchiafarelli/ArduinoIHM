# Ports

One type, no logic: `port_type` (`Ports.h`) is a plain struct pairing a
`PIN`/`PORT`/`DDR` register triple with a set mask and reset mask:

```c
typedef struct {
    volatile uint8_t *pins;
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    const uint8_t mask_set;
    const uint8_t mask_reset;
} port_type;
```

This is the "one output pin, fully described" shape used by
[BinaryOutputs](../BinaryOutputs/README.md) (20-entry output table) and
`BinaryInput` (15-entry input table) to build compile-time pin tables that
get walked generically instead of hand-written per-pin code.

Contrast with [HAL/RegisterIO.h](../HAL/README.md)'s `Reg8`/`Reg16`: that
wraps a *single register* (used for PWM timer registers, which aren't
naturally a "pin"); `port_type` wraps a full *pin* (register + bit
position) as one unit, which is what a GPIO table needs. Same underlying
idea (direct pointer to the real SFR, no Arduino pin-number indirection),
different granularity for different callers.
