#ifndef _MULTI_OUTPUT_H_
#define _MULTI_OUTPUT_H_
#include "MotorDC.h"
#include "ServoMotor.h"
#include "PWM.h"
#include "Relay.h"

class MultiOutput
{
    const BinaryOutputs bnOuts;
    Relay relays;
    MotorDC motors;
    PWM pwm;
    ServoMotor engines;
    
private:

public:
    MultiOutput():bnOuts(),
                    relays(bnOuts),
                    motors(bnOuts,MOTOR_STEPPER),
                    pwm(),
                    engines(bnOuts){};
    void timer_handler(){
        engines.timer_handler();
    };

    void fast_handler(){
        motors.fast_handler();
    };
};  

#endif /* _MULTI_OUTPUT_H_ */