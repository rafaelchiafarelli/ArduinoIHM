#pragma once
#include <stdint.h>
/**
 * A zero-overhead handle to a single hardware register.
 *
 * On the AVR target, `addr` points at the real memory-mapped SFR (e.g.
 * `&TCCR3A`); in native unit tests it points at a plain RAM byte standing in
 * for that register. Either way the exact same production code runs, and
 * native tests can assert on the resulting byte directly. There is no
 * virtual dispatch and no state beyond the pointer -- every operation
 * inlines down to a single volatile dereference, identical to writing the
 * register by name.
 */
template <typename T>
struct Reg {
    volatile T* addr;

    inline void operator=(T v) const { *addr = v; }
    inline void operator|=(T v) const { *addr |= v; }
    inline void operator&=(T v) const { *addr &= v; }
    inline operator T() const { return *addr; }
};

using Reg8 = Reg<uint8_t>;
using Reg16 = Reg<uint16_t>;
