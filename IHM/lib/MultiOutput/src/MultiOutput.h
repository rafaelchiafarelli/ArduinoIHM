#ifndef _MULTI_OUTPUT_H_
#define _MULTI_OUTPUT_H_
#include "MotorDC.h"
#include "PWM.h"
#include "Relay.h"
#include "MultiplexedBus.h"

// Servo control is not part of the IHM solution -- removed 2026-08-16.
// The multiplexed bus still has a third latch physically wired (dig_0,
// see MultiplexedBus.h's MUX_SERVO_STROBE) for a servo device, but no
// firmware in this repo drives it. A dedicated servo controller is
// planned as future, separate work -- not a rewrite of this driver.
class MultiOutput
{
    const BinaryOutputs bnOuts;
    const MultiplexedBus bus;
    Relay relays;
    MotorDC motors;
    PWM pwm;

private:
    bool value_f = false;
    bool value_s = false;
public:
    MultiOutput():bnOuts(),
                    bus(bnOuts),
                    relays(bus),
                    motors(bus,MOTOR_STEPPER),
                    pwm(){
                        bnOuts.setup();
                        bus.enableOutputs();
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