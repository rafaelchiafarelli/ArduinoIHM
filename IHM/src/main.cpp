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
    Serial.print("LCD_ID=0x");
    Serial.println(id, HEX);
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
    Serial.println("SERIAL_OK");
    //dac0.begin(0x62);
    Serial.println("DAC0_OK");
    //dac1.begin(0x63);
    Serial.println("DAC1_OK");
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
    Serial.println("BOOT");
    multiOuput.setup();
    Serial.println("MULTIOUT_OK");

    display_driver_init();
    Serial.println("DISP_INIT_OK");
    const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);
    Serial.println("SCREEN_PICKED");
    janus_render_screen(screen);
    Serial.println("RENDER_OK");
    janus_focus_move(screen, 0);   // establish initial focus
    Serial.println("FOCUS_OK");
}

ISR(TIMER2_COMPA_vect){ /*~1.008ms system tick*/
    //should we stop the timer interrupt?
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
        }
        uint8_t btnMap = buildButtonMap(bMap);

        // rot0: switch the active screen (PWM / SERIAL / Output) -- mirrors
        // the deleted TabSelector's rot0-drives-tabs behavior. Janus's own
        // `nav: tabs` block in app.yaml is metadata only (nav_titles for a
        // tab bar), not a wired input path, so this is authored directly
        // here rather than through janus_focus_move/activate.
        if(dir[0] == CW || dir[0] == CCW){
            uint16_t count = janus_app.screen_count;
            uint16_t next = (uint16_t)((janus_app.active_screen + (dir[0] == CW ? 1 : (count - 1))) % count);
            // janus_render_screen only ever fills widgets' own rects, never
            // clears first -- same stale-pixel bug the deleted GUI::update()
            // had (CHANGELOG.md 2026-08-18) before its own explicit clear.
            // The runtime can't own this: it has no notion of "canvas
            // background color", that's a per-project/hardware choice.
           // tft.fillScreen(0x0000);
            janus_switch_screen(&janus_app, next); // also re-renders the new screen
            janus_focus_move(janus_app_get_screen(&janus_app, janus_app.active_screen), 0);
        }

        // rot1 + its button: focus move / activate within the current
        // screen -- mirrors the deleted GUI::update()'s tab-local
        // navigation (BTN_MASK_ROT1).
        const janus_screen_desc_t *screen = janus_app_get_screen(&janus_app, janus_app.active_screen);
        if(dir[1] == CW || dir[1] == CCW){
            janus_focus_move(screen, dir[1] == CW ? 1 : -1);
        }
        bool rot1Pressed = (btnMap & BTN_MASK_ROT1) == 0x00;
        bool rot1WasPressed = (prevBtnMap & BTN_MASK_ROT1) == 0x00;
        if(rot1Pressed && !rot1WasPressed){
            janus_input_result_t hit = janus_focus_activate(screen);
            switch (hit.kind) {
                case JANUS_INPUT_ACTION:
                    janus_handle_action((janus_action_t)hit.action);
                    janus_render_screen(screen); // reflect the state the action just changed
                    break;
                case JANUS_INPUT_NAVIGATE:
                    tft.fillScreen(0x0000); // see the rot0 branch above for why
                    janus_switch_screen(&janus_app, (uint16_t)hit.navigate_target);
                    janus_focus_move(janus_app_get_screen(&janus_app, janus_app.active_screen), 0);
                    break;
                case JANUS_INPUT_TOGGLE_BOX:
                    janus_toggle_box(hit.widget);
                    break;
                default:
                    break;
            }
        }
        prevBtnMap = btnMap;

        mavlinkComms.poll();

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

            refreshBusStatusInstance();

            // This tick used to also do a full janus_render_screen here,
            // to pick up BusStatus's passive (non-action) MAVLink updates.
            // Since the 2026-09-05 status-bar/tab-bar redesign, that
            // repainted the *entire* active screen every ~100ms regardless
            // of what actually changed -- including screens with nothing
            // passively updating at all -- which is what "the telemetry
            // refresh cadence is for the header, not for every part of the
            // screen" was calling out. Redraw only the status bar now
            // (janus_render_widget, new 2026-09-05 -- see janus_runtime.h)
            // -- status_bar is always the first top-level widget on every
            // screen, by convention (every *.screen.yaml authors it that
            // way), not something Janus enforces, so this breaks silently
            // if that convention is ever violated.
            //
            // Known regression from this change, not fixed here:
            // BusStatus's own CAN/RS485 fields no longer refresh on this
            // timer -- only on tab-switch or a box being toggled. Needs
            // its own mechanism (e.g. an action fired from the MAVLink
            // receive path) if live passive refresh there still matters;
            // deliberately left as a follow-up rather than smuggled back
            // in as a second full-screen call here.
            const janus_screen_desc_t *active_screen = janus_app_get_screen(&janus_app, janus_app.active_screen);
            janus_screen_desc_t ls = janus_screen_load(active_screen);
            if (ls.widget_count > 0) {
                janus_render_widget(&ls.widgets[0], ls.bound_struct);
            }
        }

    }
    return 0;
}
