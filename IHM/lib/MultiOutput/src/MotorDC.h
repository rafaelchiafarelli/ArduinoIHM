#ifndef _MOTOR_DC_H_
#define _MOTOR_DC_H_
#include <BinaryOutputs.h>

#define PWMA_INDEX 8
#define DIRA_INDEX 10
#define ENA_INDEX 11

#define PWMB_INDEX 9
#define DIRB_INDEX 12
#define ENB_INDEX 13

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

        const BinaryOutputs L928_pins;
        OperationMode mode = DUAL_MOTOR_DC;
        uint8_t stepCount;
        uint8_t stepDirection;

        uint8_t dirA = 0;
        uint8_t pwmA = 0;
        uint8_t enA = 0;
        
        uint8_t dirB = 0;
        uint8_t pwmB = 0;
        uint8_t enB = 0;

    public:
        MotorDC(const BinaryOutputs bnOuts, OperationMode m):L928_pins(bnOuts),
        mode(m){

            pwmA = PWMA_INDEX;
            pwmB = PWMB_INDEX;
            
            dirA = DIRA_INDEX;
            enA = ENA_INDEX;

            dirB = DIRB_INDEX;
            enB = ENB_INDEX;
            stepCount = 0;
            stepDirection = STOPED;

        };
        void setup(OperationMode m){
        //Motor DC
            //set the timer for PWM generation
            //7bit PWM;
            //no interruption
            mode = m;
            if(mode == DUAL_MOTOR_DC){
                //setup timer 1 for PWM generation            
                TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1); //fast PWM 8bit
                TCCR1B = (1 << WGM12) | (1 << CS11); //prescaler 8
            }   
        //Motor Stepper
            //the stepper motor will use the fast handler to create the steps
            //4KHz frequency 
        
        };
        void fast_handler(){
            //this function is called every 256us
            //it is used to create the microstepping for stepper motors
            if(mode != MOTOR_STEPPER){
                return;
            }
            if(stepDirection == STOPED){
                stepCount = 0;
                L928_pins.SetOutput(enA, false); //
                L928_pins.SetOutput(dirA, false); //
                L928_pins.SetOutput(enB, false); //
                L928_pins.SetOutput(dirB, false); //
                return;
            }
            switch (stepCount)
            {
            case 0:
                /* code */
                L928_pins.SetOutput(enA, true); //
                L928_pins.SetOutput(dirA, false); //
                L928_pins.SetOutput(enB, false); //
                L928_pins.SetOutput(dirB, true); //
                break;
            case 1:
                /* code */
                L928_pins.SetOutput(enA, false); //
                L928_pins.SetOutput(dirA, true); //
                L928_pins.SetOutput(enB, false); //
                L928_pins.SetOutput(dirB, true); //
                break;
            case 2:
                /* code */
                L928_pins.SetOutput(enA, false); //
                L928_pins.SetOutput(dirA, true); //
                L928_pins.SetOutput(enB, true); //
                L928_pins.SetOutput(dirB, false); //
                break;
            case 3:
                /* code */
                L928_pins.SetOutput(enA, true); //
                L928_pins.SetOutput(dirA, false); //
                L928_pins.SetOutput(enB, true); //
                L928_pins.SetOutput(dirB, false); //
                break;                                                                          
            default:
                break;
            }

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
        };

        void stopMotorA(){
            //analogWrite(pwmA, 0); //uncomment when using with Arduino
            L928_pins.SetOutput(enA, false); //disable motor A
            L928_pins.SetOutput(dirA, false); //disable motor A
        };
        void setMotorA(int8_t speed, Direction direction){
            if(direction == CLOCKWISE){
                L928_pins.SetOutput(enA, false);
                L928_pins.SetOutput(dirA, true);
            }else{
                L928_pins.SetOutput(enA, true);
                L928_pins.SetOutput(dirA, false);
            }
            //set speed
            //map speed from -100 to 100 to 0-255
            //uint8_t pwm_value = (uint8_t)(abs(speed) * 255 / 100);
            //analogWrite(pwmA, pwm_value); //uncomment when using with Arduino
        };
        void stopMotorB(){
            //analogWrite(pwmB, 0); //uncomment when using with Arduino
            L928_pins.SetOutput(enB, false); //disable motor B
            L928_pins.SetOutput(dirB, false); //disable motor B
        };
        void setMotorB(int8_t speed, Direction direction){
            if(direction == CLOCKWISE){
                L928_pins.SetOutput(enB, false);
                L928_pins.SetOutput(dirB, true);
            }else{
                L928_pins.SetOutput(enB, true);
                L928_pins.SetOutput(dirB, false);
            }
            //set speed
            //map speed from -100 to 100 to 0-255
            //uint8_t pwm_value = (uint8_t)(abs(speed) * 255 / 100);
            //analogWrite(pwmB, pwm_value); //uncomment when using with Arduino
        };

};

#endif /* _MOTOR_DC_H_ */