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
    bool value_f = false;
    bool value_s = false;
public:
    MultiOutput():bnOuts(),
                    relays(bnOuts),
                    motors(bnOuts,MOTOR_STEPPER),
                    pwm(),
                    engines(bnOuts){
                        bnOuts.setup();
                    };
    void timer_handler(){
        engines.timer_handler();
    };
    void setup(){
        bnOuts.setup();
    }
    void fast_handler(){
        //motors.fast_handler();
        bnOuts.SetOutput(0,value_f);
        value_f = !value_f;

    };
        void slow_handler(){
        //motors.fast_handler();
        bnOuts.SetOutput(1,value_s);
        value_s = !value_s;
    };
};  

#endif /* _MULTI_OUTPUT_H_ */