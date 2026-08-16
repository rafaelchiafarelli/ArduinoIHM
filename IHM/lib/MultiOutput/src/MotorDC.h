#ifndef _MOTOR_DC_H_
#define _MOTOR_DC_H_
#include <MultiplexedBus.h>

// Bit positions within MotorDC's one latched byte (MUX_MOTOR_STROBE).
// PLACEHOLDER -- pending confirmation against IOs IHM.xlsx / the KiCad
// schematic (see IHM/NEXT-SESSION.md's open bit-layout question).
#define MOTOR_BIT_ENA 0
#define MOTOR_BIT_DIRA 1
#define MOTOR_BIT_ENB 2
#define MOTOR_BIT_DIRB 3

// Software-PWM carrier: MOTOR_DUTY_STEPS ticks/period on the ~1.008ms
// system tick -> ~99.2Hz, at the floor of the confirmed-acceptable
// 100Hz-1kHz range for this driver. No dedicated fast timer is free for
// this -- Timer1/3/4/5 are fully committed to PWM's 4-channel generator
// (see MultiOutput/README.md).
#define MOTOR_DUTY_STEPS 10

typedef enum{
    DUAL_MOTOR_DC,
    MOTOR_STEPPER
}OperationMode;

typedef enum{
    CLOCKWISE,
    COUNTER_CLOCKWISE,
    STOPED

}Direction;

class MotorDC
{
    private:

        const MultiplexedBus bus;
        OperationMode mode = DUAL_MOTOR_DC;

        // MOTOR_STEPPER state
        uint8_t stepCount = 0;
        uint8_t stepDirection = STOPED;

        // DUAL_MOTOR_DC state -- duty is 0..MOTOR_DUTY_STEPS (10% steps),
        // tickCount wraps 0..MOTOR_DUTY_STEPS-1 once per fast_handler()
        // call; the motor's enable bit is on while tickCount < duty.
        bool dirA = false;
        uint8_t dutyA = 0;
        uint8_t tickCountA = 0;

        bool dirB = false;
        uint8_t dutyB = 0;
        uint8_t tickCountB = 0;

        // The bus is byte-wide and shared, not individually addressable
        // per bit -- every change re-latches both motors' full state at
        // once, same pattern as Relay::refreshBus().
        void refreshBus() const{
            uint8_t value = 0;
            if(tickCountA < dutyA) value |= (uint8_t)(1 << MOTOR_BIT_ENA);
            if(dirA) value |= (uint8_t)(1 << MOTOR_BIT_DIRA);
            if(tickCountB < dutyB) value |= (uint8_t)(1 << MOTOR_BIT_ENB);
            if(dirB) value |= (uint8_t)(1 << MOTOR_BIT_DIRB);
            bus.write(MUX_MOTOR_STROBE, value);
        }

    public:
        MotorDC(const MultiplexedBus muxBus, OperationMode m):bus(muxBus),
        mode(m){
            stepCount = 0;
            stepDirection = STOPED;
        };

        // Called every ~1.008ms tick (TIMER2_COMPA_vect) -- drives either
        // the stepper commutation sequence or the DC software-PWM duty
        // cycle, depending on mode. Both write through the shared bus, so
        // only one of them is ever active per instance.
        void fast_handler(){
            if(mode == MOTOR_STEPPER){
                if(stepDirection == STOPED){
                    stepCount = 0;
                    bus.write(MUX_MOTOR_STROBE, 0);
                    return;
                }
                uint8_t value = 0;
                switch (stepCount)
                {
                case 0:
                    value = (uint8_t)(1 << MOTOR_BIT_ENA) | (uint8_t)(1 << MOTOR_BIT_DIRB);
                    break;
                case 1:
                    value = (uint8_t)(1 << MOTOR_BIT_DIRA) | (uint8_t)(1 << MOTOR_BIT_DIRB);
                    break;
                case 2:
                    value = (uint8_t)(1 << MOTOR_BIT_DIRA) | (uint8_t)(1 << MOTOR_BIT_ENB);
                    break;
                case 3:
                    value = (uint8_t)(1 << MOTOR_BIT_ENA) | (uint8_t)(1 << MOTOR_BIT_ENB);
                    break;
                default:
                    break;
                }
                bus.write(MUX_MOTOR_STROBE, value);

                if(stepDirection == CLOCKWISE){
                    stepCount++;
                    if(stepCount>=4){
                        stepCount=0;
                    }
                }else{
                    if(stepCount==0){
                        stepCount=3;
                    }else{
                        stepCount--;
                    }
                }
                return;
            }

            tickCountA++;
            if(tickCountA >= MOTOR_DUTY_STEPS){
                tickCountA = 0;
            }
            tickCountB++;
            if(tickCountB >= MOTOR_DUTY_STEPS){
                tickCountB = 0;
            }
            refreshBus();
        };

        void stopMotorA(){
            dutyA = 0;
        };
        void setMotorA(int8_t speed, Direction direction){
            dirA = (direction == CLOCKWISE);
            uint8_t magnitude = (uint8_t)(speed < 0 ? -speed : speed);
            if(magnitude > 100){
                magnitude = 100;
            }
            dutyA = (uint8_t)((magnitude * MOTOR_DUTY_STEPS + 50) / 100);
        };
        void stopMotorB(){
            dutyB = 0;
        };
        void setMotorB(int8_t speed, Direction direction){
            dirB = (direction == CLOCKWISE);
            uint8_t magnitude = (uint8_t)(speed < 0 ? -speed : speed);
            if(magnitude > 100){
                magnitude = 100;
            }
            dutyB = (uint8_t)((magnitude * MOTOR_DUTY_STEPS + 50) / 100);
        };

};

#endif /* _MOTOR_DC_H_ */