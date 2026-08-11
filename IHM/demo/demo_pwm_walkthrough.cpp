#include <stdio.h>
#include <string.h>
#include "PWM.h"
#include "PWMChannelConfig.h"
#include "PWMLabelFormat.h"
/**
 * Host-side walkthrough of every PWM channel/frequency/edge combination,
 * driving the *real* production code (PWMChannelConfig's edit methods,
 * computeSimplexCallArgs/computeComplexCallArgs, applySimplexPWMConfig/
 * applyComplexPWMConfig) exactly as PWMSimplex::editField/PWMComplex::
 * editField do on the real device -- the only thing standing in for
 * hardware is where the Reg8/Reg16 handles point: a plain RAM byte here,
 * the real TCCRnA/TCCRnB/OCRnX/ICRn on-device (see PWM.cpp). Nothing about
 * the logic under test differs between this run and the real firmware.
 *
 * This is not a pass/fail test suite (see test_native/ for that) -- it's a
 * readable log proving what each fix actually produces, for a human to
 * read alongside the hardware runbook (demo/HARDWARE_RUNBOOK.md).
 */

namespace {

struct FakeSimplexRegs {
    uint8_t tccrA = 0, tccrB = 0;
    uint16_t icr = 0, ocrA = 0;
    SimplexPWMRegisters handles() { return {{&tccrA}, {&tccrB}, {&icr}, {&ocrA}}; }
};

struct FakeComplexRegs {
    uint8_t tccrA = 0, tccrB = 0;
    uint16_t icr = 0, ocrA = 0, ocrB = 0, ocrC = 0;
    ComplexPWMRegisters handles() { return {{&tccrA}, {&tccrB}, {&icr}, {&ocrA}, {&ocrB}, {&ocrC}}; }
};

void printSectionHeader(const char* title) {
    printf("\n=== %s ===\n", title);
}

const char* timerNameFor(int channelIndex) {
    switch (channelIndex) {
        case 0: return "Timer3 (OC3A)";
        case 1: return "Timer5 (OC5A)";
        case 2: return "Timer1 (OC1A/B/C)";
        default: return "Timer4 (OC4A/B/C)";
    }
}

void logSimplexLine(int channelIndex, const PWMChannelConfig& cfg, const FakeSimplexRegs& regs) {
    char modeBuf[PWM_LABEL_BUFFER_SIZE], freqBuf[PWM_LABEL_BUFFER_SIZE],
         dutyBuf[PWM_LABEL_BUFFER_SIZE], statusBuf[PWM_LABEL_BUFFER_SIZE];
    formatModeLabel(modeBuf, cfg.enabled, cfg.frequency);
    formatFrequencyLabel(freqBuf, cfg.frequency, cfg.variableTopValue);
    formatDutyCycleLabel(dutyBuf, cfg.dutyCyclePercent);
    formatStatusLabel(statusBuf, cfg.enabled);
    printf("  [ch%d %-18s] %-13s %-11s duty=%-4s %-7s edge=%-7s -> TCCRxA=0x%02X TCCRxB=0x%02X OCRxA=%u%s\n",
           channelIndex, timerNameFor(channelIndex), statusBuf, modeBuf, dutyBuf, freqBuf,
           cfg.inverting ? "falling" : "rising", regs.tccrA, regs.tccrB, regs.ocrA,
           (cfg.frequency == frequency_variable) ? " (TOP=ICRx)" : "");
}

void runSimplexWalkthrough(int channelIndex) {
    printSectionHeader(channelIndex == 0 ? "Channel 0 -- every frequency, simplex"
                                          : "Channel 1 -- every frequency, simplex");
    PWMChannelConfig cfg;

    // Off state first.
    FakeSimplexRegs offRegs;
    applySimplexPWMConfig(offRegs.handles(), computeSimplexCallArgs(cfg).frequency, false, false, 0, 0);
    logSimplexLine(channelIndex, cfg, offRegs);

    cfg.cycleModeNext();  // Off -> Fixed, lands on frequency_62_500HZ
    for (int f = 0; f < (int)NUMBER_OF_PWM_FREQUENCIES; f++) {
        cfg.frequency = (PWMFrequency)f;
        cfg.dutyCyclePercent = 37;
        cfg.inverting = (f % 2) == 1;
        cfg.variableTopValue = 20000;

        FakeSimplexRegs regs;
        SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
        applySimplexPWMConfig(regs.handles(), args.frequency, args.inverting, args.enabled,
                               args.rawFrequency, args.rawDutyCycle);
        logSimplexLine(channelIndex, cfg, regs);
    }
}

void runComplexIndependenceProof(int channelIndex) {
    printSectionHeader(channelIndex == 2 ? "Channel 2 -- per-output independence proof, complex"
                                          : "Channel 3 -- per-output independence proof, complex");
    printf("  Proves the anti-clobbering fix directly: configuring B must not\n");
    printf("  disturb A's already-written bits, and configuring C must not\n");
    printf("  disturb A or B's -- each line below shows TCCRxA growing by\n");
    printf("  exactly one output's bits, never losing the previous ones.\n");

    PWMComplexChannelConfig cfg;
    cfg.frequency = frequency_31_250HZ;  // 9-bit: the width where COMnX1 used to be dropped
    FakeComplexRegs regs;

    auto apply = [&]() {
        ComplexPWMCallArgs args = computeComplexCallArgs(cfg);
        applyComplexPWMConfig(regs.handles(), args.frequency, args.rawFrequency,
                               args.invertingA, args.enabledA, args.invertingB, args.enabledB,
                               args.invertingC, args.enabledC, args.rawDutyCycleA, args.rawDutyCycleB,
                               args.rawDutyCycleC);
    };

    cfg.outputA.toggleEnabled();
    cfg.outputA.dutyCyclePercent = 20;
    apply();
    printf("  [ch%d %-18s] after enabling A only:      TCCRxA=0x%02X (bits 7:6 = A's COM bits)\n",
           channelIndex, timerNameFor(channelIndex), regs.tccrA);
    uint8_t afterA = regs.tccrA;

    cfg.outputB.toggleEnabled();
    cfg.outputB.toggleEdge();
    cfg.outputB.dutyCyclePercent = 60;
    apply();
    printf("  [ch%d %-18s] after also enabling B(inv): TCCRxA=0x%02X (bits 7:6 unchanged from above: %s)\n",
           channelIndex, timerNameFor(channelIndex), regs.tccrA,
           ((regs.tccrA & 0xC0) == (afterA & 0xC0)) ? "yes" : "NO -- REGRESSION");
    uint8_t afterB = regs.tccrA;

    cfg.outputC.toggleEnabled();
    cfg.outputC.dutyCyclePercent = 90;
    apply();
    printf("  [ch%d %-18s] after also enabling C:      TCCRxA=0x%02X (bits 7:4 unchanged from above: %s)\n",
           channelIndex, timerNameFor(channelIndex), regs.tccrA,
           ((regs.tccrA & 0xF0) == (afterB & 0xF0)) ? "yes" : "NO -- REGRESSION");
    printf("  Final: OCRxA=%u (A, 20%%) OCRxB=%u (B, 60%%) OCRxC=%u (C, 90%%) of TOP=511 (9-bit)\n",
           regs.ocrA, regs.ocrB, regs.ocrC);
}

void runDutyCycleResolutionProof() {
    printSectionHeader("Duty-cycle-to-raw-value scaling -- same 50% at every resolution");
    struct Case { PWMFrequency f; const char* width; };
    Case cases[] = {
        {frequency_62_500HZ, "8-bit,  TOP=255"},
        {frequency_31_250HZ, "9-bit,  TOP=511"},
        {frequency_15_625HZ, "10-bit, TOP=1023"},
        {frequency_variable, "variable, TOP=user-chosen"},
    };
    for (auto& c : cases) {
        PWMChannelConfig cfg;
        cfg.enabled = true;
        cfg.frequency = c.f;
        cfg.dutyCyclePercent = 50;
        cfg.variableTopValue = 40000;
        SimplexPWMCallArgs args = computeSimplexCallArgs(cfg);
        printf("  %-28s 50%% -> raw OCRxA = %u\n", c.width, args.rawDutyCycle);
    }
}

}  // namespace

int main() {
    printf("PWM subsystem walkthrough -- exercises the real production code\n");
    printf("(PWMChannelConfig / PWMComplexChannelConfig / computeSimplexCallArgs /\n");
    printf("computeComplexCallArgs / applySimplexPWMConfig / applyComplexPWMConfig)\n");
    printf("against fake registers standing in for the real TCCRnA/TCCRnB/OCRnX/ICRn.\n");

    runSimplexWalkthrough(0);
    runSimplexWalkthrough(1);
    runComplexIndependenceProof(2);
    runComplexIndependenceProof(3);
    runDutyCycleResolutionProof();

    printf("\nDone. See demo/HARDWARE_RUNBOOK.md for the same checks on real hardware.\n");
    return 0;
}
