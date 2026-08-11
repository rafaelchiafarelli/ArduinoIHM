#pragma once
#include <stdint.h>
/**
 * Bit positions within TIMSK2, per the ATmega2560 datasheet (confirmed
 * against the toolchain's iomxx0_1.h): TOIE2=bit0, OCIE2A=bit1, OCIE2B=bit2.
 * Kept as plain integers (not avr/io.h's OCIE2A etc.) so this is natively
 * testable; main.cpp still uses the real macros when it writes TIMSK2 --
 * this only pins down which single bit belongs in it.
 */
#define TIMER2_TOIE2_BIT 0
#define TIMER2_OCIE2A_BIT 1
#define TIMER2_OCIE2B_BIT 2

/**
 * Only TIMER2_COMPA_vect (the ~1ms system tick) has a handler in main.cpp.
 * Compare-B and Overflow are deliberately left disabled here so their
 * vectors never fire -- an earlier version enabled all three anyway and
 * relied on two empty "must be defined here otherwise the system looses
 * itself" ISRs to avoid a reset when the (disabled-by-omission) default
 * handler ran. Enabling only what's handled removes the need for those.
 */
uint8_t timer2InterruptMask();
