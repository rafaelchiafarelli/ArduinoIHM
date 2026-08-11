#include "PWMLabelFormat.h"
#include <stdio.h>
#include <string.h>

namespace {
// Truncated integer Hz for each fixed PWMFrequency, in enum order (see
// PWMTiming.h). Precomputed to avoid pulling in avr-libc's float-capable
// printf just to show a duty-cycle-generator-style status label.
const uint32_t FIXED_FREQUENCY_HZ[] = {
    62500, 31250, 15625, 7812, 3906, 1953, 976, 488, 244, 122, 61, 30, 15
};

void copyLiteral(char* buf, const char* text) {
    strncpy(buf, text, PWM_LABEL_BUFFER_SIZE);
    buf[PWM_LABEL_BUFFER_SIZE - 1] = '\0';
}
}  // namespace

void formatDutyCycleLabel(char* buf, uint8_t dutyCyclePercent) {
    snprintf(buf, PWM_LABEL_BUFFER_SIZE, "%u%%", (unsigned int)dutyCyclePercent);
}

void formatModeLabel(char* buf, bool enabled, PWMFrequency f) {
    if (!enabled) {
        copyLiteral(buf, "Mode:Desl.");
    } else {
        copyLiteral(buf, (f == frequency_variable) ? "Mode:Variavel" : "Mode:Fixa");
    }
}

void formatFrequencyLabel(char* buf, PWMFrequency f, uint16_t variableTopValue) {
    uint32_t hz = (f == frequency_variable)
                      ? (16000000UL / ((uint32_t)variableTopValue + 1))
                      : FIXED_FREQUENCY_HZ[f];
    snprintf(buf, PWM_LABEL_BUFFER_SIZE, "F:%luHz", (unsigned long)hz);
}

void formatStatusLabel(char* buf, bool enabled) {
    copyLiteral(buf, enabled ? "Status:Ativo" : "Status:Inativo");
}

void formatShortStatusLabel(char* buf, bool enabled) {
    copyLiteral(buf, enabled ? "Ativo" : "Desl.");
}
