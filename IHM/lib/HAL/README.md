# HAL

`RegisterIO.h` defines `Reg8`/`Reg16`: a zero-overhead handle to a single
hardware register.

```cpp
template <typename T>
struct Reg {
    volatile T* addr;
    inline void operator=(T v) const { *addr = v; }
    inline void operator|=(T v) const { *addr |= v; }
    inline void operator&=(T v) const { *addr &= v; }
    inline operator T() const { return *addr; }
};
```

On the AVR build, `addr` points at the real SFR (e.g. `&TCCR3A`). In
native unit tests it points at a plain RAM byte standing in for that
register, so the exact same production code runs on-device and on the
host, and tests can assert on the resulting byte directly -- no mocking
framework, no virtual dispatch.

This is what makes `PWMChannelConfig`/`PWMTiming`/`applySimplexPWMConfig`/
`applyComplexPWMConfig` (`lib/MultiOutput`) natively testable despite
writing real AVR timer registers: the register-selection logic takes
`Reg8`/`Reg16` handles as parameters rather than naming `TCCR1A` etc.
directly, so a test can hand it fake backing bytes.

See [Ports/README.md](../Ports/README.md) for how this differs from
`port_type` (single register vs. a full pin).
