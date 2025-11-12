#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#include "SerialCommunication.h"
#include "BinaryOutputs.h"
#include "AnalogOut.h"
#include "BinaryInput.h"
#include "SD.h"
#include <GUI.h>
#include <MCP4725.h>
#include <Arduino.h>
#include <Display.h>
#include <BinaryInput.h>
#include <BinaryOutputs.h>
#include <AnalogOut.h>
#include <MultiOutput.h>

#define VOLTAGE_REGULATOR_0_ADDRESS 0
#define VOLTAGE_REGULATOR_1_ADDRESS 1

#define ONE_MS_T0_TICKS 4 //
#define TEN_MS_T0_TICKS 40 //
uint8_t counterT0 = 0;
uint8_t counterT1 = 0;
uint16_t timeStatistics = 0;
uint8_t timeCounter = 0;
BinaryInputs userInputs;
SerialCommunication comms;
MultiOutput multiOuput;
MCP4725 dac1,dac0;
uint16_t voltage0 = 0;
uint16_t voltage1 = 0;
GUI gui = GUI();
uint16_t receivedRawData[10];

void setup()
{
    Serial.begin(250000);
    Serial.println("Starting System...");
    // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
    // For MCP4725A0 the address is 0x60 or 0x61
    
    dac0.begin(0x60);    
    dac1.begin(0x61);    

    Serial.println("Starting GUI...");
    gui.setup();
    sei(); // allow interrupts
   
}
/*
ISR(TIMER0_COMPA_vect){
    
    multiOuput.fast_handler();

    if(comms.fast_handler(receivedRawData,10))
    {
      gui.receiveData(receivedRawData);
      voltage0 = receivedRawData[VOLTAGE_REGULATOR_0_ADDRESS];
      voltage1 = receivedRawData[VOLTAGE_REGULATOR_1_ADDRESS];
    } 

    counterT0++;
    if (counterT0 >= TEN_MS_T0_TICKS) {
        counterT0 = 0;
        // Call the handler functions of various modules every 10ms
        // Example:
        // module1.ten_ms_handler();
        // module2.ten_ms_handler();
        userInputs.fast_handler();
    }
    if(timeCounter>=100){
        gui.updateTimeStatistics(timeStatistics/100);
        timeStatistics=0;
        timeCounter=0;
    }    
    timeStatistics += TCNT0;
    timeCounter+=1;

    TCNT0 = 0; //reset the T0 timer;

}

ISR(TIMER0_COMPB_vect){
    // Warning handler code here (called every 512us)
    gui.showWarning(TIMER0_OVERUN);
}

ISR(TIMER0_OVF_vect){
    // Warning handler code here (called every 512us)
    gui.showEmergency(TIMER0_OVERUN);
}
    */
ISR(TIMER2_COMPA_vect){
    multiOuput.timer_handler();
}

int main()
{
  init();
  setup();

  while (1)
  {

    dac1.setVoltage(voltage0, false);
    dac0.setVoltage(voltage1, false);
    
    Serial.println("Running GUI...");
    _delay_ms(200);
    gui.update();
    Serial.println("GUI done");
    _delay_ms(500);
  }
  return 0;
}