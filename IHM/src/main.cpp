#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#include "SerialCommunication.h"
#include "BinaryOutputs.h"
#include "BinaryInput.h"
#include "SD.h"
#include <GUI.h>
#include <MCP4725.h>
#include <Arduino.h>
#include <Display.h>
#include <BinaryInput.h>
#include <BinaryOutputs.h>
#include <MultiOutput.h>
#include <RotaryEncoder.h>
#include "HardwareSerial.h"

#define VOLTAGE_REGULATOR_0_ADDRESS 0
#define VOLTAGE_REGULATOR_1_ADDRESS 1

#define TEN_MS_T0_TICKS 10 //
#define TWENTY_FIVE_MS_T0_TICKS 25 //
uint8_t counterT0 = 0;
uint8_t counterT1 = 0;
bool newDataAvailable = false;
uint16_t timeStatistics = 0;
uint8_t timeCounter = 0;

SerialCommunication comms;
MultiOutput multiOuput;
MCP4725 dac1,dac0;
uint16_t voltage0 = 0;
uint16_t voltage1 = 0;
GUI gui = GUI();
uint16_t receivedRawData[10];

BinaryInputs userInputs;
RotaryEncoder rotaryEncoders(userInputs);

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
    //TCCR2B |= (1 << CS22);   // Prescaler 256, WGM22 bit cleared (Fast PWM Mode)
    TCCR2B |= (1 << WGM22) | (1 << CS22) | (1 << CS20);   // Prescaler 256, WGM22 bit SET (Fast PWM Mode with adjustable frequency)
    OCR2A = 125;                          // Compare value for 256us at 16MHz with prescaler 256
    
    TIMSK2 = 0;

    TCNT2 = 0;                           // Initialize counter value to 0   
    TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B) | (1 << TOIE2); // Enable Compare A, Compare B and Overflow interrupts
    
    sei();
    
    gui.setup();
    multiOuput.setup();
   
}

ISR(TIMER2_COMPA_vect){ /*256us handler*/

    multiOuput.fast_handler();
    userInputs.fast_handler();
    counterT0++;
    if (counterT0 >= TEN_MS_T0_TICKS) { //~1ms elapsed
        counterT0 = 0;
        // Call the handler functions of various modules every 1ms
        // Example:
        // module1.ten_ms_handler();
        // module2.ten_ms_handler();
        //userInputs.slow_handler();
        rotaryEncoders.ten_ms_handler();
    }

    counterT1++;
    if (counterT1 >= TWENTY_FIVE_MS_T0_TICKS) { //~10ms elapsed
        counterT1 = 0;
        multiOuput.slow_handler();
        // Call the handler functions of various modules every 10ms
        // Example:
        // module1.ten_ms_handler();
        // module2.ten_ms_handler();
        newDataAvailable = comms.fast_handler(receivedRawData,10);

    }
    timeStatistics += TCNT2;
    timeCounter+=1;

    //TCNT2 = 0; //reset the T0 timer to the next interrupt point taking into account the drift;

}
//TODO: find out why I need to put this ISR´s here.
ISR(TIMER2_COMPB_vect){
/**
 * must be defined here otherwise the system looses itself.
 */
}

ISR(TIMER2_OVF_vect){
/**
 * must be defined here otherwise the system looses itself.
 */
}

ISR(TIMER1_COMPA_vect){
 //   multiOuput.timer_handler();
 
}

int main()
{
    init();
    setup();
    gui.screenMachine(ScreenType::CONF_ANALOG_OUT);
    while (1)
    {

    //multiOuput.fast_handler();

   // gui.showRegularLoop(WarningType::TIMER0_OVERUN);
        if(timeCounter>=100){
            gui.updateTimeStatistics(timeStatistics/timeCounter);
            timeStatistics=0;
            timeCounter=0;
        // gui.screenMachine(IDLE_SCREEN);
        }    

        if(newDataAvailable){
            //gui.receiveData();
            voltage0 = receivedRawData[0];
            voltage1 = receivedRawData[1];
            newDataAvailable = false;
        }
        
        dac1.setVoltage(voltage0, false);
        dac0.setVoltage(voltage1, false);
        uint16_t bMap = 0x8000;
        for(int i=0;i<MAX_PIN_AMOUNT;i++) {
            bool b = userInputs.get_pin(i);
            bMap |= ((0x01)&((uint16_t)b))<<i;
        }
        Serial.print("map of inputs:");
        Serial.println(bMap,2);

        gui.update();
    }
    return 0;
}