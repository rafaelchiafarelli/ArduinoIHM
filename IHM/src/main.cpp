#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#include "SerialCommunication.h"
#include "BinaryOutputs.h"
#include "BinaryInput.h"
#include <MCP4725.h>
#include <Arduino.h>
#include <Display.h>
#include <BinaryInput.h>
#include <BinaryOutputs.h>
#include <MultiOutput.h>
#include <RotaryEncoder.h>
#include <ButtonMap.h>
#include "HardwareSerial.h"
#include "Timer2Config.h"
#include "AnalogInput.h"
#include "MavlinkComms.h"
#include "PWMWireConfig.h"
#include "PWMLabelFormat.h"

// Janus-generated UI (lib/GUI) -- plain C, so every header/declaration that
// crosses into this .cpp translation unit needs extern "C" linkage to match
// how the vendored runtime and generated .gen.c files are actually compiled
// (as C, unmangled) -- otherwise the linker looks for C++-mangled symbol
// names that don't exist.
extern "C" {
#include "janus_runtime.h"
#include "janus_input_focus.h"
#include "janus_remote.h"
#include "janus_bindings.gen.h"
#include "janus_actions.gen.h"
#include "pwm_screen.gen.h"
#include "busstatus_screen.gen.h"
#include "relay_screen.gen.h"
extern janus_app_t janus_app;
}

#define VOLTAGE_REGULATOR_0_ADDRESS 0
#define VOLTAGE_REGULATOR_1_ADDRESS 1

#define TEN_MS_T0_TICKS 10 //
#define TWENTY_FIVE_MS_T0_TICKS 25 //
uint8_t counterT0 = 0;  // ISR-local only: never read outside TIMER2_COMPA_vect
uint8_t counterT1 = 0;  // ISR-local only: never read outside TIMER2_COMPA_vect

// Written in TIMER2_COMPA_vect, read from main() -- must be volatile so the
// compiler neither caches a stale value across main()'s loop body nor
// reorders these reads/writes relative to the ISR.
volatile bool newDataAvailable = false;
volatile uint16_t timeStatistics = 0;
volatile uint8_t timeCounter = 0;

SerialCommunication comms;
MultiOutput multiOuput;
MCP4725 dac1,dac0;
uint16_t voltage0 = 0;
uint16_t voltage1 = 0;
Display tft; // Instantiate the display object
PWM pwm;
MavlinkComms mavlinkComms; // Serial2 via lib/Uart2 -- Serial0 is debug-only

uint16_t receivedRawData[10];
BinaryInputs userInputs;
// Written in TIMER2_COMPA_vect, read from main() -- see comment above.
volatile uint16_t bMap = 0;
RotaryEncoder rotaryEncoders(&userInputs);
AnalogInputs analogInputs;

// Relay class (lib/MultiOutput/src/Relay.h) has no getter for a relay's
// current on/off state -- it only exposes setRelay()/enableRelay() -- so
// the Janus action handler (janus_actions.c) needs this to know what to
// flip *to*. Owned here, not in janus_actions.c, since it's plain C and
// can't hold a bool array any more naturally than main.cpp already does.
bool relayState[NUMBER_OF_RELAYS] = {false, false, false, false, false, false, false, false};

// ---------------------------------------------------------------- Janus --
// driver contract implementations (janus_runtime.h) -- vendor-provided,
// never defined by the fixed runtime itself. draw_area_sync/async stream
// an RGB565 tile through the existing mcufriend-style parallel driver's
// own bulk-write primitive (setAddrWindow + pushColors) rather than one
// drawPixel() call per pixel.
extern "C" void display_driver_init(void){
    uint16_t id = tft.readID();

    tft.begin(id);
    tft.setRotation(0);
    tft.fillScreen(0x0000); // black
}

extern "C" void draw_area_sync(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pixels){
    tft.setAddrWindow((int16_t)x, (int16_t)y, (int16_t)(x + w - 1), (int16_t)(y + h - 1));
    tft.pushColors(const_cast<uint16_t*>(pixels), (int16_t)(w * h), true);
}

// This driver has no real async/DMA transfer -- pushColors() is a
// synchronous bit-banged write -- so "async" here just means the runtime's
// draw queue still hands us one small op at a time (via janus_render_poll)
// instead of a single giant blocking pass; display_busy() always reporting
// false is what tells the runtime this op already completed.
extern "C" bool draw_area_async(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pixels){
    draw_area_sync(x, y, w, h, pixels);
    return true;
}

extern "C" bool display_busy(void){
    return false;
}

// RAM-resident text for the CH0/CH1 inverting indicator (chN_inverting_label):
// the runtime reads bound strings through a plain RAM pointer, never PROGMEM.
static char pwmInvertedText[] = "inverted";
static char pwmNonInvertedText[] = "non-inverted";
static const char* pwmInvertingText(bool inverting) { return inverting ? pwmInvertedText : pwmNonInvertedText; }

static void initPwmDefaults(); // below mirrorPwmToUi -- seeds pwmLast[] and the PWM tab

// Focuses the active screen's first focusable widget -- never the tab strip:
// rot0 owns the tabs (see the rot0 branch in loop()). Clears the strip's
// preview first: janus_switch_screen clears widget focus but not the
// strip's, and on a screen with no focusable widget (SERIAL since
// fixes/000010) janus_focus_move(&janus_app, 0) put focus on the strip and
// left it there, so later screens reported widget AND strip focus at once
// (fixes/000012). Same strip-less copy as the rot1 branch.
static void focusFirstOnScreen()
{
    janus_set_nav_focus(&janus_app, -1);
    janus_app_t screenOnly = janus_app;
    screenOnly.nav_tabs = NULL;
    screenOnly.nav_tab_count = 0;
    janus_focus_move(&screenOnly, 0);
}

void setup()
{
    Serial.begin(250000);   // debug port only
    mavlinkComms.begin(MAVLINK_SERIAL_BAUD);   // protocol port: Serial2, interrupt-driven

    //dac0.begin(0x62);
    
    //dac1.begin(0x63);
    
    cli();

    // setting system timer
    GTCCR = 0B10000000;
    TCCR2A = 0;
    TCCR2A |= (1 << WGM21) | (1 << WGM20); // Fast PWM Mode
    TCCR2B = 0;
    TCCR2B |= (1 << WGM22) | (1 << CS22) | (1 << CS20);   // Prescaler 128, WGM22 bit SET (Fast PWM Mode with adjustable frequency)
    OCR2A = 125;                          // Compare value for ~1.008ms at 16MHz with prescaler 128 ((125+1) * 128 / 16MHz)
    TIMSK2 = 0;
    TCNT2 = 0;                           // Initialize counter value to 0
    // Only Compare-A is enabled: it's the only vector with a handler below.
    // Compare-B/Overflow are deliberately left disabled (see Timer2Config.h).
    TIMSK2 |= timer2InterruptMask();

    sei();
    
    multiOuput.setup();
    

    display_driver_init();
    
    initPwmDefaults();

    const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);

    janus_render_screen(screen);
    janus_render_status_bar(&janus_app);   // app-level status band -- no-op if app.yaml had no `status:`
    janus_render_nav_bar(&janus_app);   // app-level PWM/SERIAL/Output tab strip -- no-op if app.yaml had no `nav:`

    focusFirstOnScreen();   // establish initial focus

}

// RAM-resident text for pwm_instance.chN_state_label (the runtime reads
// bound strings through a plain RAM pointer -- a PROGMEM literal would be
// read as garbage on AVR, same bug class as the janus_handoff notes).
static char pwmStateLabel[4][PWM_LABEL_BUFFER_SIZE];

// Mirrors one PC-applied PWM channel into the Janus-bound pwm_instance and
// marks the touched fields dirty, so the PWM tab shows what the PC set.
// pwm_instance has no per-channel inverting for complex channels and no
// frequency field -- only what exists is mirrored (the frequency goes into
// the state label). Display-only: the hardware was already configured.
static void mirrorPwmToUi(uint8_t ch, const PWMChannelConfig* simplex, const PWMComplexChannelConfig* complex)
{
    PWMFrequency f = simplex ? simplex->frequency : complex->frequency;
    uint16_t top = simplex ? simplex->variableTopValue : complex->variableTopValue;
    formatFrequencyLabel(pwmStateLabel[ch], f, top);
    switch (ch) {
        case 0:
            pwm_instance.ch0_enabled = simplex->enabled;
            pwm_instance.ch0_duty_percent = simplex->dutyCyclePercent;
            pwm_instance.ch0_inverting = simplex->inverting;
            pwm_instance.ch0_inverting_label = pwmInvertingText(simplex->inverting);
            pwm_instance.ch0_state_label = pwmStateLabel[0];
            pwm_dirty.ch0_enabled = pwm_dirty.ch0_duty_percent = pwm_dirty.ch0_inverting = pwm_dirty.ch0_inverting_label = pwm_dirty.ch0_state_label = true;
            break;
        case 1:
            pwm_instance.ch1_enabled = simplex->enabled;
            pwm_instance.ch1_duty_percent = simplex->dutyCyclePercent;
            pwm_instance.ch1_inverting = simplex->inverting;
            pwm_instance.ch1_inverting_label = pwmInvertingText(simplex->inverting);
            pwm_instance.ch1_state_label = pwmStateLabel[1];
            pwm_dirty.ch1_enabled = pwm_dirty.ch1_duty_percent = pwm_dirty.ch1_inverting = pwm_dirty.ch1_inverting_label = pwm_dirty.ch1_state_label = true;
            break;
        case 2:
            pwm_instance.ch2_a_enabled = complex->outputA.enabled; pwm_instance.ch2_a_duty_percent = complex->outputA.dutyCyclePercent;
            pwm_instance.ch2_b_enabled = complex->outputB.enabled; pwm_instance.ch2_b_duty_percent = complex->outputB.dutyCyclePercent;
            pwm_instance.ch2_c_enabled = complex->outputC.enabled; pwm_instance.ch2_c_duty_percent = complex->outputC.dutyCyclePercent;
            pwm_instance.ch2_state_label = pwmStateLabel[2];
            pwm_dirty.ch2_a_enabled = pwm_dirty.ch2_a_duty_percent = pwm_dirty.ch2_b_enabled = pwm_dirty.ch2_b_duty_percent = true;
            pwm_dirty.ch2_c_enabled = pwm_dirty.ch2_c_duty_percent = pwm_dirty.ch2_state_label = true;
            break;
        case 3:
            pwm_instance.ch3_a_enabled = complex->outputA.enabled; pwm_instance.ch3_a_duty_percent = complex->outputA.dutyCyclePercent;
            pwm_instance.ch3_b_enabled = complex->outputB.enabled; pwm_instance.ch3_b_duty_percent = complex->outputB.dutyCyclePercent;
            pwm_instance.ch3_c_enabled = complex->outputC.enabled; pwm_instance.ch3_c_duty_percent = complex->outputC.dutyCyclePercent;
            pwm_instance.ch3_state_label = pwmStateLabel[3];
            pwm_dirty.ch3_a_enabled = pwm_dirty.ch3_a_duty_percent = pwm_dirty.ch3_b_enabled = pwm_dirty.ch3_b_duty_percent = true;
            pwm_dirty.ch3_c_enabled = pwm_dirty.ch3_c_duty_percent = pwm_dirty.ch3_state_label = true;
            break;
    }
}

// Last config applied to each channel, from the PC (PWM_CHANNEL_CONFIG) or an
// on-screen switch. A switch press flips one bit of this and re-applies it.
static PwmWireConfig pwmLast[4];

// Applies one validated config to the timers and mirrors it onto the PWM
// tab (marks the widgets dirty; the caller decides when to repaint). Duty
// goes through compute*CallArgs because setupPWMChannelN takes RAW OCR
// counts, not percent.
static void applyPwmChannel(const PwmWireConfig& w)
{
    uint8_t ch = w.channel;
    if (ch < 2) {
        PWMChannelConfig sc = pwmWireToSimplex(w);
        SimplexPWMCallArgs a = computeSimplexCallArgs(sc);
        if (ch == 0) pwm.setupPWMChannel0(a.frequency, a.inverting, a.enabled, a.rawFrequency, a.rawDutyCycle);
        else         pwm.setupPWMChannel1(a.frequency, a.inverting, a.enabled, a.rawFrequency, a.rawDutyCycle);
        mirrorPwmToUi(ch, &sc, NULL);
    } else {
        PWMComplexChannelConfig cc = pwmWireToComplex(w);
        ComplexPWMCallArgs a = computeComplexCallArgs(cc);
        if (ch == 2) pwm.setupPWMChannel2(a.frequency, a.rawFrequency,
                         a.invertingA, a.enabledA, a.invertingB, a.enabledB, a.invertingC, a.enabledC,
                         a.rawDutyCycleA, a.rawDutyCycleB, a.rawDutyCycleC);
        else         pwm.setupPWMChannel3(a.frequency, a.rawFrequency,
                         a.invertingA, a.enabledA, a.invertingB, a.enabledB, a.invertingC, a.enabledC,
                         a.rawDutyCycleA, a.rawDutyCycleB, a.rawDutyCycleC);
        mirrorPwmToUi(ch, NULL, &cc);
    }
    pwmLast[ch] = w;
}

// Boot state (Rafael, 2026-09-25): 62500 Hz, 50 % duty, every output off and
// non-inverting, so a first Enabled press gives a visible square wave.
// Only mirrored onto the PWM tab here; the timers are untouched until a
// switch press or a PWM_CHANNEL_CONFIG applies a channel.
static void initPwmDefaults()
{
    for (uint8_t ch = 0; ch < 4; ch++) {
        PwmWireConfig w = { ch, (uint8_t)frequency_62_500HZ, 0, { { 0, 0, 50 }, { 0, 0, 50 }, { 0, 0, 50 } } };
        pwmLast[ch] = w;
        if (ch < 2) { PWMChannelConfig sc = pwmWireToSimplex(w); mirrorPwmToUi(ch, &sc, NULL); }
        else        { PWMComplexChannelConfig cc = pwmWireToComplex(w); mirrorPwmToUi(ch, NULL, &cc); }
    }
}

// On-screen PWM switch (pbRE1 on a focused toggle, via janus_actions.cpp):
// flip output `out` (0-2 = A-C)'s enable or inverting bit and re-apply the
// channel immediately.
void pwmToggleOutput(uint8_t ch, uint8_t out, bool inverting)
{
    if (ch > 3 || out > 2) return;
    PwmWireConfig w = pwmLast[ch];
    uint8_t& bit = inverting ? w.out[out].inverting : w.out[out].enabled;
    bit = bit ? 0 : 1;
    applyPwmChannel(w);
}

// RE2 on a focused PWM switch (via janusSetSwitch in janus_actions.cpp):
// sets -- not flips -- output `out`'s enable or inverting bit to `on` and
// re-applies the channel only if that changed it.
void pwmSetOutputBit(uint8_t ch, uint8_t out, bool inverting, bool on)
{
    if (ch > 3) return;
    PwmWireConfig w = pwmLast[ch];
    if (pwmWireSetOutputBit(w, out, inverting, on)) applyPwmChannel(w);
}

// On-screen frequency edit: re-applies channel `ch` at frequency `f`,
// keeping its outputs' enable/inverting/duty. No-op if unchanged.
static void pwmSetFrequency(uint8_t ch, PWMFrequency f)
{
    if (ch > 3 || pwmLast[ch].f_selector == (uint8_t)f) return;
    PwmWireConfig w = pwmLast[ch];
    w.f_selector = (uint8_t)f;
    applyPwmChannel(w);
}

// pbRE1 on a focused frequency label (via janus_actions.cpp): one step
// lower, wrapping from 15 Hz back to 62500 Hz.
void pwmCycleChannelFrequency(uint8_t ch)
{
    if (ch > 3) return;
    pwmSetFrequency(ch, pwmCycleFrequency((PWMFrequency)pwmLast[ch].f_selector));
}

// On-screen duty edit: re-applies channel `ch` with output `out` (0-2 =
// A-C) at `percent`, keeping everything else. No-op if unchanged.
static void pwmSetDuty(uint8_t ch, uint8_t out, uint8_t percent)
{
    if (ch > 3 || out > 2 || pwmLast[ch].out[out].duty_percent == percent) return;
    PwmWireConfig w = pwmLast[ch];
    w.out[out].duty_percent = percent;
    applyPwmChannel(w);
}

// Which duty bar an action belongs to (channel, output 0-2 = A-C); false
// for any other action.
static bool pwmDutyActionTarget(janus_action_t action, uint8_t* ch, uint8_t* out)
{
    switch (action) {
        case JANUS_ACTION_EDIT_PWM_CH0_DUTY:   *ch = 0; *out = 0; return true;
        case JANUS_ACTION_EDIT_PWM_CH1_DUTY:   *ch = 1; *out = 0; return true;
        case JANUS_ACTION_EDIT_PWM_CH2_A_DUTY: *ch = 2; *out = 0; return true;
        case JANUS_ACTION_EDIT_PWM_CH2_B_DUTY: *ch = 2; *out = 1; return true;
        case JANUS_ACTION_EDIT_PWM_CH2_C_DUTY: *ch = 2; *out = 2; return true;
        case JANUS_ACTION_EDIT_PWM_CH3_A_DUTY: *ch = 3; *out = 0; return true;
        case JANUS_ACTION_EDIT_PWM_CH3_B_DUTY: *ch = 3; *out = 1; return true;
        case JANUS_ACTION_EDIT_PWM_CH3_C_DUTY: *ch = 3; *out = 2; return true;
        default: return false;
    }
}

// janus_actions.cpp: sets the switch behind a toggle_* action (PWM switch
// or relay) to `on`; false if the action isn't a switch.
bool janusSetSwitch(janus_action_t action, bool on);
// janus_actions.cpp: drives relay `index` to `on` -- hardware, relayState[]
// and the Output tab's switch + LED (marked dirty, not repainted).
void relaySet(uint8_t index, bool on);

// Which channel's frequency label an action belongs to; -1 for any other.
static int8_t pwmFrequencyActionChannel(janus_action_t action)
{
    switch (action) {
        case JANUS_ACTION_CYCLE_PWM_CH0_FREQUENCY: return 0;
        case JANUS_ACTION_CYCLE_PWM_CH1_FREQUENCY: return 1;
        case JANUS_ACTION_CYCLE_PWM_CH2_FREQUENCY: return 2;
        case JANUS_ACTION_CYCLE_PWM_CH3_FREQUENCY: return 3;
        default: return -1;
    }
}

ISR(TIMER2_COMPA_vect){ /*~1.008ms system tick*/
    // Interrupts back on first thing: the tick's handlers take up to ~220 us,
    // and at 250000 baud USART2 overruns after ~3 byte times (~120 us) if its
    // RX ISR can't get in (serial_transport task 6). No re-entry guard: the
    // tick must always finish inside its ~1 ms slot -- keep it that way.
    sei();

    // RULE (Rafael, 2026-09-27): the MAVLink fast handler is always the FIRST
    // call after sei() -- nothing goes between them. It drains the Serial2 RX
    // ring (bounded) and decodes complete frames; the superloop is its only
    // consumer (take*/consume*/get*), so nothing below depends on it.
    mavlinkComms.fast_handler();

    multiOuput.fast_handler();
    bMap = userInputs.fast_handler();
    counterT0++;
    if (counterT0 >= TEN_MS_T0_TICKS) { //~10ms elapsed
        counterT0 = 0;
        // Call the handler functions of various modules every 1ms
        // Example:
        // module1.ten_ms_handler();
        // module2.ten_ms_handler();
        //userInputs.slow_handler();

    }
    counterT1++;
    if (counterT1 >= TWENTY_FIVE_MS_T0_TICKS) { //~25ms elapsed
        counterT1 = 0;
        multiOuput.slow_handler();
        // Call the handler functions of various modules every 10ms
        // Example:
        // module1.ten_ms_handler();
        // module2.ten_ms_handler();
        newDataAvailable = comms.fast_handler(receivedRawData,10);
        rotaryEncoders.ms_handler(bMap);
    }

    timeStatistics += TCNT2;
    timeCounter+=1;
    //TCNT2 = 0; //reset the T0 timer to the next interrupt point taking into account the drift;
}

// Fires on every completed ADC conversion (~104-200us apart) -- keeps
// AnalogInputs' round-robin channel scan running continuously so
// analogInputs.read() is always non-blocking. See AnalogInput.h.
ISR(ADC_vect){
    analogInputs.isr_handler();
}

// Reflects the CAN0/CAN1/RS-485 signal-generator config MavlinkComms has
// received from the PC app into bus_status_instance -- same data the old,
// now-deleted Elements/BusStatusScreen.h read, just pushed into Janus's
// bound struct instead of a hand-written widget. Read-only tab, so this is
// the only writer of these fields.
static void refreshBusStatusInstance(){
    mavlink_can_signal_config_t can;
    if(mavlinkComms.getCanSignalConfig(0, &can)){
        bus_status_instance.can0_enabled = can.enable ? 1 : 0;
        bus_status_instance.can0_id = (int)can.can_id;
        bus_status_instance.can0_dlc = can.dlc;
        bus_status_instance.can0_extended = can.extended_id ? 1 : 0;
    }

    if(mavlinkComms.getCanSignalConfig(1, &can)){
        bus_status_instance.can1_enabled = can.enable ? 1 : 0;
        bus_status_instance.can1_id = (int)can.can_id;
        bus_status_instance.can1_dlc = can.dlc;
        bus_status_instance.can1_extended = can.extended_id ? 1 : 0;
    }

    mavlink_rs485_signal_config_t rs485;
    if(mavlinkComms.getRs485SignalConfig(&rs485)){
        bus_status_instance.rs485_enabled = rs485.enable ? 1 : 0;
        bus_status_instance.rs485_length = rs485.length;
        bus_status_instance.rs485_period_ms = rs485.period_ms;
    }
}

int main()
{
    init();
    setup();

    // Edge-detected each pass below -- BinaryInputs/ButtonMap's convention
    // is active-low (a bit reads 0 while the button is held), so this is
    // "was rot1's button already down last pass" for a single-fire click.
    // Seeded from the real inputs, not 0xFF ("nothing held"): a button
    // already down at power-up must not count as a press (fixes/000008 --
    // the bench board's pbRE1 reads held at rest, which fired one phantom
    // press at every boot). The Timer2 tick has been sampling bMap all
    // through setup(), so it's a real read by now.
    uint8_t prevBtnMap = buildButtonMap(bMap);

    while (1)
    {
        // rotaryEncoders.getDirection() is edge-triggered and consumes the
        // latch on read (see RotaryEncoder.h) -- read each encoder exactly
        // once per pass and reuse the value everywhere it's needed below
        // (Janus dispatch here, MAVLink telemetry further down), never
        // call it a second time for the same encoder in the same pass.
        DIRECTION_TYPE dir[MAX_NUMBER_EMCODERS];
        for(int i = 0; i < MAX_NUMBER_EMCODERS; i++){
            dir[i] = rotaryEncoders.getDirection(i);
            // A simulated turn (IHM_SIMULATE_ENCODER, PC -> board) only
            // takes effect when the real hardware read was idle this
            // pass -- real input always wins, and this is consumed
            // exactly once either way.
            if(dir[i] == not_supported){
                uint8_t simulated = mavlinkComms.consumeSimulatedEncoderDirection(i);
                if(simulated == CCW || simulated == CW)
                    dir[i] = (DIRECTION_TYPE)simulated;
            }
        }
        uint8_t btnMap = buildButtonMap(bMap);
        // A simulated press (IHM_SIMULATE_BUTTON, PC -> board) reads as
        // pressed for this one pass. btnMap is active-low, so clearing the
        // bit presses it; a real press is already 0 and stays 0. Applied
        // before every btnMap consumer below (edge detection, telemetry).
        btnMap &= (uint8_t)~mavlinkComms.consumeSimulatedButtons();

        // rot0: switch the active screen (PWM / SERIAL / Output) -- mirrors
        // the deleted TabSelector's rot0-drives-tabs behavior. Authored
        // directly here (not through janus_focus_move/activate) because
        // this board has a *dedicated* control for it, unlike Janus's
        // stock single-control encoder scaffold, whose nav_tabs task 4
        // reaches the tab strip by walking off the end of the focus order
        // -- not needed here, but harmless: rot1 below can still reach the
        // strip that way too, since app.yaml declares `nav: tabs`.
        if(dir[0] == CW || dir[0] == CCW){
            uint16_t count = janus_app.screen_count;
            uint16_t next = (uint16_t)((janus_app.active_screen + (dir[0] == CW ? 1 : (count - 1))) % count);
            // janus_render_screen only ever fills widgets' own rects, never
            // clears first -- same stale-pixel bug the deleted GUI::update()
            // had (CHANGELOG.md 2026-08-18) before its own explicit clear.
            // The runtime can't own this: it has no notion of "canvas
            // background color", that's a per-project/hardware choice.
           // tft.fillScreen(0x0000);
            janus_switch_screen(&janus_app, next); // also re-renders the new screen + repaints the tab strip
            focusFirstOnScreen();
        }

        // rot1 + its button: focus move / activate within the current
        // screen -- mirrors the deleted GUI::update()'s tab-local
        // navigation (BTN_MASK_ROT1). rot1 only scans the screen's own
        // widgets: rot0 owns the tabs, so Janus's walk-off-the-end onto the
        // tab strip (nav_tabs epic task 4) is unwanted here. A copy of the
        // app with no nav strip gets Janus's documented plain per-screen
        // wrap; focus movement only reads the app (active_screen is never
        // changed by a move), so the copy is safe.
        const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);
        if(dir[1] == CW || dir[1] == CCW){
            janus_app_t screenOnly = janus_app;
            screenOnly.nav_tabs = NULL;
            screenOnly.nav_tab_count = 0;
            janus_focus_move(&screenOnly, dir[1] == CW ? 1 : -1);
        }
        bool rot1Pressed = (btnMap & BTN_MASK_ROT1) == 0x00;
        bool rot1WasPressed = (prevBtnMap & BTN_MASK_ROT1) == 0x00;
        if(rot1Pressed && !rot1WasPressed){
            janus_input_result_t hit = janus_focus_activate(&janus_app);
            switch (hit.kind) {
                case JANUS_INPUT_ACTION:
                    janus_handle_action((janus_action_t)hit.action);
                    janus_render_screen(screen); // reflect the state the action just changed
                    break;
                case JANUS_INPUT_NAVIGATE:
                    tft.fillScreen(0x0000); // see the rot0 branch above for why
                    janus_switch_screen(&janus_app, (uint16_t)hit.navigate_target);
                    focusFirstOnScreen();
                    break;
                case JANUS_INPUT_TOGGLE_BOX:
                    janus_toggle_box(hit.widget);
                    break;
                default:
                    break;
            }
        }
        prevBtnMap = btnMap;

        // rot2 changes whatever rot1 has selected, live (navigation
        // initiative's editing rule): a frequency label steps its channel's
        // frequency (CW = higher, clamped at 62500 Hz / 15 Hz), a duty bar
        // steps that output's duty 1 % (CW = higher, clamped 0-100), a
        // switch (PWM Enabled/Inverting, relay) is set -- CW = on /
        // inverting, CCW = off / non-inverting, never flipped. Any other
        // focus: nothing. janus_focus_activate only resolves the
        // focused widget to its action here, it runs nothing: its one side
        // effect, committing a previewed tab, needs focus on the nav strip,
        // which rot1 never gives (see above).
        if(dir[2] == CW || dir[2] == CCW){
            int8_t step = dir[2] == CW ? 1 : -1;
            janus_input_result_t hit = janus_focus_activate(&janus_app);
            if (hit.kind == JANUS_INPUT_ACTION) {
                janus_action_t action = (janus_action_t)hit.action;
                int8_t ch = pwmFrequencyActionChannel(action);
                uint8_t dutyCh, dutyOut;
                if (!janusSetSwitch(action, step > 0)) {
                    if (ch >= 0) {
                        PWMFrequency f = (PWMFrequency)pwmLast[ch].f_selector;
                        pwmSetFrequency((uint8_t)ch, pwmStepFrequency(f, step));
                    } else if (pwmDutyActionTarget(action, &dutyCh, &dutyOut)) {
                        uint8_t duty = pwmLast[dutyCh].out[dutyOut].duty_percent;
                        pwmSetDuty(dutyCh, dutyOut, pwmStepDuty(duty, step));
                    }
                }
                // Dirty-only repaint. Since Janus shared_field_dirty
                // (c56f14c) it repaints every widget bound to a changed
                // field, so a switch and the LED sharing its field both
                // follow; no full redraw needed any more.
                janus_render_screen_if_dirty(screen);
            }
        }

        // PC-driven PWM (PWM_CHANNEL_CONFIG). Same apply path as the
        // on-screen switches -- last writer wins. Invalid frames are dropped
        // silently (no ack in the protocol).
        bool pwmUiChanged = false;
        for (uint8_t ch = 0; ch < 4; ch++) {
            mavlink_pwm_channel_config_t m;
            if (!mavlinkComms.takePwmChannelConfig(ch, &m)) continue;
            PwmWireConfig w = {
                m.channel, m.f_selector, m.frequency,
                { { m.out1_enabled, m.out1_inverting, m.out1_duty_percent },
                  { m.out2_enabled, m.out2_inverting, m.out2_duty_percent },
                  { m.out3_enabled, m.out3_inverting, m.out3_duty_percent } }
            };
            if (!pwmWireConfigValid(w)) continue;
            applyPwmChannel(w);
            pwmUiChanged = true;
        }
        // Repaint only when the PWM tab is the one showing, and only the
        // dirty widgets -- not a full-screen redraw (see the ~100 ms block's
        // note about that regression). Off-screen the values simply wait in
        // pwm_instance for the next full render of the tab.
        if (pwmUiChanged && janus_app_get_screen(&janus_app, janus_app.active_screen) == &pwm_screen) {
            janus_render_screen_if_dirty(&pwm_screen);
        }

        // PC-driven relays (IHM_RELAY_COMMAND). Same path as the Output tab,
        // last writer wins; only relays whose state actually changes are
        // driven, so a repeated frame does nothing.
        uint8_t relayCmdMask, relayCmdState;
        if (mavlinkComms.takeRelayCommand(&relayCmdMask, &relayCmdState)) {
            uint8_t current = 0;
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++) {
                if (relayState[i]) current |= (uint8_t)(1u << i);
            }
            uint8_t target = relayCommandApply(current, relayCmdMask, relayCmdState);
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++) {
                if ((current ^ target) & (1u << i)) relaySet(i, (target >> i) & 1u);
            }
            if (current != target && janus_app_get_screen(&janus_app, janus_app.active_screen) == &relay_screen) {
                janus_render_screen_if_dirty(&relay_screen);
            }
        }

        if(newDataAvailable){
            voltage0 = receivedRawData[0];
            voltage1 = receivedRawData[1];
            newDataAvailable = false;
        }

        //dac1.setVoltage(voltage0, false);
        //dac0.setVoltage(voltage1, false);

        if(timeCounter>=100){
            uint16_t stats = timeStatistics;
            uint8_t count = timeCounter;
            timeStatistics=0;
            timeCounter=0;

            uint16_t analogIn[4];
            for(uint8_t i=0;i<4;i++){
                analogIn[i] = analogInputs.read(i);
            }
            uint16_t battVoltage = analogInputs.read(ANALOG_INPUT_BATT_VOLTAGE_INDEX);
            uint8_t rotation = (bMap>>4) & 0x01;
            uint8_t charging = (bMap>>5) & 0x01;

            mavlinkComms.sendBoardState(btnMap & 0x7F,
                                         (uint8_t)dir[0], (uint8_t)dir[1], (uint8_t)dir[2],
                                         rotation, charging, battVoltage,
                                         analogIn, stats, count);

            uint8_t relayMask = 0;
            for (uint8_t i = 0; i < NUMBER_OF_RELAYS; i++)
            {
                if (relayState[i])
                    relayMask |= (uint8_t)(1u << i);
            }
            mavlinkComms.sendRelayState(relayMask);

            // One PWM channel per tick, round-robin: all 4 refresh every
            // ~400 ms and each tick's frames fit the TX ring together.
            // Duty is clamped as the adapters clamp it, so the PC sees what
            // was applied, not a raw out-of-range value it may have sent.
            static uint8_t pwmStateCh = 0;
            const PwmWireConfig& p = pwmLast[pwmStateCh];
            mavlink_ihm_pwm_state_t s;
            s.channel = p.channel;
            s.f_selector = p.f_selector;
            s.frequency = p.frequency;
            s.out1_enabled = p.out[0].enabled; s.out1_inverting = p.out[0].inverting; s.out1_duty_percent = min(p.out[0].duty_percent, (uint8_t)100);
            s.out2_enabled = p.out[1].enabled; s.out2_inverting = p.out[1].inverting; s.out2_duty_percent = min(p.out[1].duty_percent, (uint8_t)100);
            s.out3_enabled = p.out[2].enabled; s.out3_inverting = p.out[2].inverting; s.out3_duty_percent = min(p.out[2].duty_percent, (uint8_t)100);
            mavlinkComms.sendPwmState(s);
            pwmStateCh = (pwmStateCh + 1) & 0x03;

            // Janus UI state (screen / focus / open boxes) for the PC's
            // screen mirror: on change, else every 5 ticks (~500 ms) as a
            // heartbeat. Reads the real janus_app -- focus is a runtime
            // global indexed within the active screen, so the nav-less copy
            // RE1 moves focus on reports the same index.
            static janus_remote_state_t uiSent;
            static uint8_t uiTicksSinceSend = 4;   // first tick sends
            janus_remote_state_t ui;
            janus_remote_state_get(&janus_app, &ui);
            if (++uiTicksSinceSend >= 5 || ui.screen != uiSent.screen || ui.focus != uiSent.focus ||
                ui.nav_focus != uiSent.nav_focus || ui.boxes_expanded != uiSent.boxes_expanded) {
                mavlinkComms.sendUiState(ui.screen, ui.focus, ui.nav_focus, ui.boxes_expanded);
                uiSent = ui;
                uiTicksSinceSend = 0;
            }

            refreshBusStatusInstance();

            // This tick used to also force-redraw the active screen's first
            // top-level widget every ~100ms, back when that widget was
            // always a per-screen status_bar row needing a periodic refresh.
            // Since Janus's status_bar epic (2026-09-15), the status text is
            // app-level chrome (app.yaml's `status:`, static, painted once
            // by janus_render_status_bar) -- it's no longer a screen widget
            // at all, so there's nothing left for this tick to periodically
            // refresh. Removed rather than repointed at whatever now
            // happens to be widgets[0] on each screen, which would silently
            // redraw unrelated content for no reason.
            //
            // Known regression, still not fixed here: BusStatus's own
            // CAN/RS485 fields only refresh on tab-switch or a box being
            // toggled, not passively on this timer. Needs its own mechanism
            // (e.g. an action fired from the MAVLink receive path) if live
            // passive refresh there still matters.
        }

    }
    return 0;
}
