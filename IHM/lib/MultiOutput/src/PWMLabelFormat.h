#pragma once
#include <stdint.h>
#include "PWMChannelConfig.h"
/**
 * Formats PWMChannelConfig state into the short strings PWMSimplex/
 * PWMComplex (lib/Elements) put on screen. Pure text formatting -- no
 * Display, no AVR dependency -- so it's natively testable, and avoids
 * floating-point formatting entirely (no avr-libc float-printf flash cost):
 * fixed frequencies come from a precomputed integer-Hz table, and the
 * variable-frequency case uses plain integer division (16MHz / (TOP+1)).
 */

// Matches Elements/src/Element.h's LABEL_STRING_SIZE. Every formatted string
// here (including the null terminator) fits within this many bytes -- Label
// null-terminates via strncpy only when the source is shorter than the
// buffer, so overrunning it would corrupt the on-screen text.
#define PWM_LABEL_BUFFER_SIZE 15

void formatDutyCycleLabel(char* buf, uint8_t dutyCyclePercent);

// enabled=false always renders "Mode:Desl." regardless of f. Both channel
// kinds have a channel-level off state now (PWMChannelConfig::enabled and
// PWMComplexChannelConfig::enabled), so both pass their real `enabled` here.
void formatModeLabel(char* buf, bool enabled, PWMFrequency f);

void formatFrequencyLabel(char* buf, PWMFrequency f, uint16_t variableTopValue);
void formatStatusLabel(char* buf, bool enabled);

// Same as formatStatusLabel, but without the "Status:" prefix -- used by
// PWMComplex, where three outputs share one narrow on-screen column.
void formatShortStatusLabel(char* buf, bool enabled);
