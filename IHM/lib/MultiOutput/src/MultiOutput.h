#ifndef _MULTI_OUTPUT_H_
#define _MULTI_OUTPUT_H_
#include "MotorDC.h"
#include "ServoMotor.h"
#include "PWM.h"
#include "Relay.h"
#include "MultiplexedBus.h"

class MultiOutput
{
    const BinaryOutputs bnOuts;
    const MultiplexedBus bus;
    Relay relays;
    MotorDC motors;
    PWM pwm;
    ServoMotor engines;

private:
    bool value_f = false;
    bool value_s = false;
public:
    MultiOutput():bnOuts(),
                    bus(bnOuts),
                    relays(bus),
                    motors(bnOuts,MOTOR_STEPPER),
                    pwm(),
                    engines(bnOuts){
                        bnOuts.setup();
                        bus.enableOutputs();
                    };

    void timer_handler(){
        engines.timer_handler();
    };

    void setup(){
        bnOuts.setup();
        for(uint8_t i=0; i<NUMBER_OF_RELAYS; i++){
            relays.enableRelay(i);
        }
    };

    void fast_handler(){
        motors.fast_handler();
    };

    void slow_handler(){
        relays.ultra_slow_handler();

    };

    Relay* getRelays(){ return &relays; };
};

#endif /* _MULTI_OUTPUT_H_ */