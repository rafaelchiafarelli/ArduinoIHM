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
MavlinkComms mavlinkComms(&Serial);

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

void setup()
{
    Serial.begin(250000);
    
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
    
    const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);

    janus_render_screen(screen);
    janus_render_status_bar(&janus_app);   // app-level status band -- no-op if app.yaml had no `status:`
    janus_render_nav_bar(&janus_app);   // app-level PWM/SERIAL/Output tab strip -- no-op if app.yaml had no `nav:`

    janus_focus_move(&janus_app, 0);   // establish initial focus

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
            pwm_instance.ch0_state_label = pwmStateLabel[0];
            pwm_dirty.ch0_enabled = pwm_dirty.ch0_duty_percent = pwm_dirty.ch0_inverting = pwm_dirty.ch0_state_label = true;
            break;
        case 1:
            pwm_instance.ch1_enabled = simplex->enabled;
            pwm_instance.ch1_duty_percent = simplex->dutyCyclePercent;
            pwm_instance.ch1_inverting = simplex->inverting;
            pwm_instance.ch1_state_label = pwmStateLabel[1];
            pwm_dirty.ch1_enabled = pwm_dirty.ch1_duty_percent = pwm_dirty.ch1_inverting = pwm_dirty.ch1_state_label = true;
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

ISR(TIMER2_COMPA_vect){ /*~1.008ms system tick*/
    
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
    // Serial RX: bounded drain of the USART0 ring into the MAVLink parser.
    // Last on purpose (AVR ISRs don't nest, so the USART0 RX ISR waits for
    // everything above) and before the timeStatistics sample so the loop-
    // time debug field reflects its cost. Never blocks, never drives
    // hardware -- see MavlinkComms::tick().
    mavlinkComms.tick();
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
    const mavlink_can_signal_config_t *can0 = mavlinkComms.getCanSignalConfig(0);
    if(can0){
        bus_status_instance.can0_enabled = can0->enable ? 1 : 0;
        bus_status_instance.can0_id = (int)can0->can_id;
        bus_status_instance.can0_dlc = can0->dlc;
        bus_status_instance.can0_extended = can0->extended_id ? 1 : 0;
    }

    const mavlink_can_signal_config_t *can1 = mavlinkComms.getCanSignalConfig(1);
    if(can1){
        bus_status_instance.can1_enabled = can1->enable ? 1 : 0;
        bus_status_instance.can1_id = (int)can1->can_id;
        bus_status_instance.can1_dlc = can1->dlc;
        bus_status_instance.can1_extended = can1->extended_id ? 1 : 0;
    }

    const mavlink_rs485_signal_config_t *rs485 = mavlinkComms.getRs485SignalConfig();
    if(rs485){
        bus_status_instance.rs485_enabled = rs485->enable ? 1 : 0;
        bus_status_instance.rs485_length = rs485->length;
        bus_status_instance.rs485_period_ms = rs485->period_ms;
    }
}

int main()
{
    init();
    setup();

    // Edge-detected each pass below -- BinaryInputs/ButtonMap's convention
    // is active-low (a bit reads 0 while the button is held), so this is
    // "was rot1's button already down last pass" for a single-fire click.
    uint8_t prevBtnMap = 0xFF;

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
                uint8_t simulated = 0;
                if(mavlinkComms.takeSimulatedEncoderDirection(i, &simulated) && (simulated == CCW || simulated == CW))
                    dir[i] = (DIRECTION_TYPE)simulated;
            }
        }
        uint8_t btnMap = buildButtonMap(bMap);

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
            janus_focus_move(&janus_app, 0);
        }

        // rot1 + its button: focus move / activate within the current
        // screen -- mirrors the deleted GUI::update()'s tab-local
        // navigation (BTN_MASK_ROT1). janus_focus_move/activate now take
        // the whole janus_app_t (nav_tabs epic task 4), not just the
        // active screen -- walking rot1 past the last/first focusable
        // widget lands on the tab strip itself, previewed there, and a
        // click commits the switch, same as rot0 above.
        const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);
        if(dir[1] == CW || dir[1] == CCW){
            janus_focus_move(&janus_app, dir[1] == CW ? 1 : -1);
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
                    janus_focus_move(&janus_app, 0);
                    break;
                case JANUS_INPUT_TOGGLE_BOX:
                    janus_toggle_box(hit.widget);
                    break;
                default:
                    break;
            }
        }
        prevBtnMap = btnMap;
        // MAVLink RX is drained from the Timer2 tick (mavlinkComms.tick());
        // this loop only consumes what it stored, via the take*() accessors.

        // PC-driven PWM (PWM_CHANNEL_CONFIG). Same registers as the
        // on-screen PWM tab -- last writer wins. Invalid frames are dropped
        // silently (no ack in the protocol). Duty goes through
        // compute*CallArgs because setupPWMChannelN takes RAW OCR counts,
        // not percent.
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
            pwmUiChanged = true;
        }
        // Repaint only when the PWM tab is the one showing, and only the
        // dirty widgets -- not a full-screen redraw (see the ~100 ms block's
        // note about that regression). Off-screen the values simply wait in
        // pwm_instance for the next full render of the tab.
        if (pwmUiChanged && janus_app_get_screen(&janus_app, janus_app.active_screen) == &pwm_screen) {
            janus_render_screen_if_dirty(&pwm_screen);
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
