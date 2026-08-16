#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#include "SerialCommunication.h"
#include "BinaryOutputs.h"
#include "BinaryInput.h"
#include <GUI.h>
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
// Declared before `gui` (was after, further down) so its name exists when
// GUI's constructor takes a MavlinkComms* -- construction order itself
// doesn't matter here, GUI only stores the pointer.
MavlinkComms mavlinkComms(&Serial);
GUI gui = GUI(&tft, &pwm, multiOuput.getRelays(), &mavlinkComms);
uint16_t receivedRawData[10];
BinaryInputs userInputs;
// Written in TIMER2_COMPA_vect, read from main() -- see comment above.
volatile uint16_t bMap = 0;
RotaryEncoder rotaryEncoders(&userInputs);
AnalogInputs analogInputs;

void setup()
{
    Serial.begin(250000);
    dac0.begin(0x62);    
    dac1.begin(0x63);    
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
    gui.setup();
    multiOuput.setup();

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

int main()
{
    init();
    setup();
    
    while (1)
    {

   // gui.showRegularLoop(WarningType::TIMER0_OVERUN);

        mavlinkComms.poll();

        if(newDataAvailable){
            //gui.receiveData();
            voltage0 = receivedRawData[0];
            voltage1 = receivedRawData[1];
            newDataAvailable = false;
        }

        dac1.setVoltage(voltage0, false);
        dac0.setVoltage(voltage1, false);

        DIRECTION_TYPE dir[MAX_NUMBER_EMCODERS];
        for(int i =0;i<MAX_NUMBER_EMCODERS;i++){
            dir[i] = rotaryEncoders.getDirection(i);
        }

        uint8_t btnMap = buildButtonMap(bMap);

        gui.update(dir[0],dir[1],dir[2], btnMap);

        if(timeCounter>=100){
            uint16_t stats = timeStatistics;
            uint8_t count = timeCounter;
            gui.updateTimeStatistics(stats/count);
            timeStatistics=0;
            timeCounter=0;
        // gui.screenMachine(IDLE_SCREEN);

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
        }

    }
    return 0;
}