#ifndef _MULTI_OUTPUT_H_
#define _MULTI_OUTPUT_H_
#include "PWM.h"
#include "Relay.h"
#include "MultiplexedBus.h"

// Servo control is not part of the IHM solution -- removed 2026-08-16.
// The multiplexed bus still has a third latch physically wired (dig_0,
// see MultiplexedBus.h's MUX_SERVO_STROBE) for a servo device, but no
// firmware in this repo drives it. A dedicated servo controller is
// planned as future, separate work -- not a rewrite of this driver.
//
// DC/stepper motor control is removed too (fixes/000013, 2026-09-28: the
// motor was never connected). Its latch (dig_2, MUX_MOTOR_STROBE) stays
// wired; setup() latches 0 into it once so its outputs sit low.
class MultiOutput
{
    const BinaryOutputs bnOuts;
    const MultiplexedBus bus;
    Relay relays;
    PWM pwm;

private:
    bool value_f = false;
    bool value_s = false;
public:
    MultiOutput():bnOuts(),
                    bus(bnOuts),
                    relays(bus),
                    pwm(){
                        bnOuts.setup();
                        bus.enableOutputs();
                    };

    void setup(){
        bnOuts.setup();
        for(uint8_t i=0; i<NUMBER_OF_RELAYS; i++){
            relays.enableRelay(i);
        }
        bus.write(MUX_MOTOR_STROBE, 0);   // unused latch: outputs low
    };

    void slow_handler(){
        relays.ultra_slow_handler();

    };

    Relay* getRelays(){ return &relays; };
};

#endif /* _MULTI_OUTPUT_H_ */