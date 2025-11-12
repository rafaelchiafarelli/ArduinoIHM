#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <BinaryOutputs.h>

#define NUMBER_OF_ENGINES 10
#define MAX_POSITION 31248
#define MIN_POSITION 15624
#define INITIAL_POSITION 15624
typedef enum
{
    SERVO_UP,
    SERVO_DOWN,
    SERVO_WAIT
} servo_type;

typedef struct{
    uint8_t index=0;
    bool enabled=false;
    uint16_t position = INITIAL_POSITION;
}engine;

class ServoMotor
{
private:
    const BinaryOutputs engines_pins;    
    const uint16_t pos_max = MAX_POSITION;
    const uint16_t pos_0 = INITIAL_POSITION;
    unsigned int counter = 0;
    engine engines[NUMBER_OF_ENGINES];
    servo_type state = SERVO_UP;

public:

    ServoMotor(const BinaryOutputs bnOut):engines_pins(bnOut)
    {   
        //initialization of the engines 
        for(int i=0;i<NUMBER_OF_ENGINES;i++){
            engines[i].index=i+8; //the first servo is connected to pin 8
            engines[i].position = pos_0;
            engines[i].enabled = false;
        }
    }
    
    void timer_handler();
    void load(uint16_t *ar);
    void load(uint16_t ar,uint8_t index);
    void enableEngine(uint8_t index){
        /**
         * CONFIGURE TIMER 2 TO GENERAT THE 1ms PULSE FOR SERVO MOTORS
         */
        if(index<NUMBER_OF_ENGINES){
            engines[index].enabled = true;
        }
    }
};