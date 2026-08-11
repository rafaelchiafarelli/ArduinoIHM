#include "mini_test.h"
#include "PWMLabelFormat.h"
#include <string.h>

TEST(FormatDutyCycleLabel, FormatsWholePercentages) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatDutyCycleLabel(buf, 0);
    CHECK_EQ(strcmp(buf, "0%"), 0);
    formatDutyCycleLabel(buf, 50);
    CHECK_EQ(strcmp(buf, "50%"), 0);
    formatDutyCycleLabel(buf, 100);
    CHECK_EQ(strcmp(buf, "100%"), 0);
}

TEST(FormatModeLabel, DistinguishesFixedFromVariableWhenEnabled) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatModeLabel(buf, true, frequency_62_500HZ);
    CHECK_EQ(strcmp(buf, "Mode:Fixa"), 0);
    formatModeLabel(buf, true, frequency_variable);
    CHECK_EQ(strcmp(buf, "Mode:Variavel"), 0);
}

TEST(FormatModeLabel, ShowsOffRegardlessOfFrequencyWhenDisabled) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatModeLabel(buf, false, frequency_62_500HZ);
    CHECK_EQ(strcmp(buf, "Mode:Desl."), 0);
    formatModeLabel(buf, false, frequency_variable);
    CHECK_EQ(strcmp(buf, "Mode:Desl."), 0);
}

TEST(FormatFrequencyLabel, ReportsTheRealValueForEveryFixedFrequency) {
    // These must match the *actual* hardware frequency (see PWMTiming.cpp),
    // not the enum name -- frequency_16_625HZ was renamed to
    // frequency_15_625HZ specifically because its real output is 15625Hz,
    // and this label is what a user would use to sanity-check the output.
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatFrequencyLabel(buf, frequency_62_500HZ, 0);
    CHECK_EQ(strcmp(buf, "F:62500Hz"), 0);
    formatFrequencyLabel(buf, frequency_15_625HZ, 0);
    CHECK_EQ(strcmp(buf, "F:15625Hz"), 0);
    formatFrequencyLabel(buf, frequency_15_2587890625HZ, 0);
    CHECK_EQ(strcmp(buf, "F:15Hz"), 0);
}

TEST(FormatFrequencyLabel, VariableModeComputesFrequencyFromTop) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    // 16MHz / (999+1) = 16000 Hz
    formatFrequencyLabel(buf, frequency_variable, 999);
    CHECK_EQ(strcmp(buf, "F:16000Hz"), 0);
    // smallest legal TOP (1): 16MHz / 2 = 8,000,000 Hz -- must still fit the buffer
    formatFrequencyLabel(buf, frequency_variable, 1);
    CHECK_EQ(strcmp(buf, "F:8000000Hz"), 0);
    CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
}

TEST(FormatStatusLabel, DistinguishesEnabledFromDisabled) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatStatusLabel(buf, true);
    CHECK_EQ(strcmp(buf, "Status:Ativo"), 0);
    formatStatusLabel(buf, false);
    CHECK_EQ(strcmp(buf, "Status:Inativo"), 0);
}

TEST(FormatShortStatusLabel, DistinguishesEnabledFromDisabled) {
    char buf[PWM_LABEL_BUFFER_SIZE];
    formatShortStatusLabel(buf, true);
    CHECK_EQ(strcmp(buf, "Ativo"), 0);
    formatShortStatusLabel(buf, false);
    CHECK_EQ(strcmp(buf, "Desl."), 0);
}

TEST(LabelFormatters, EveryFormattedStringFitsTheLabelBuffer) {
    // Every Element::setLabel() copies via strncpy(dest, src, LABEL_STRING_SIZE)
    // without guaranteeing null-termination if src is >= that size -- so
    // every string these formatters can ever produce must be strictly
    // shorter than PWM_LABEL_BUFFER_SIZE.
    char buf[PWM_LABEL_BUFFER_SIZE];
    for (int i = 0; i < (int)NUMBER_OF_PWM_FREQUENCIES; i++) {
        formatFrequencyLabel(buf, (PWMFrequency)i, 1);
        CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
        formatModeLabel(buf, true, (PWMFrequency)i);
        CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
        formatModeLabel(buf, false, (PWMFrequency)i);
        CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
    }
    formatDutyCycleLabel(buf, 100);
    CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
    formatStatusLabel(buf, false);
    CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
    formatShortStatusLabel(buf, false);
    CHECK_TRUE(strlen(buf) < PWM_LABEL_BUFFER_SIZE);
}
