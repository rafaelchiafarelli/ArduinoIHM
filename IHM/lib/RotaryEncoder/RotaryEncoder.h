#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_
#include <Arduino.h>
#include <avr/io.h>
#include <BinaryInput.h>
#define MAX_NUMBER_OF_ENCODERS 3
typedef struct{
    uint8_t pinA;
    uint8_t pinB;
    int16_t  value;
    uint8_t   lastState;
}rotary_encoder_t;
class RotaryEncoder
{
    private:
        const BinaryInputs inputs;
        rotary_encoder_t encoders[MAX_NUMBER_OF_ENCODERS];
    public:
        RotaryEncoder(const BinaryInputs binInputs):inputs(binInputs){
            encoders[0] = {4,5,0,0};
            encoders[1] = {7,8,0,0};
            encoders[2] = {10,11,0,0};
            encoders[3] = {13,14,0,0};
        };
        void ten_ms_handler(){
            for(uint8_t i=0;i<MAX_NUMBER_OF_ENCODERS;i++){
                uint8_t pinAState = inputs.get_pin(encoders[i].pinA)?1:0;
                uint8_t pinBState = inputs.get_pin(encoders[i].pinB)?1:0;
                uint8_t currentState = (pinAState<<1) | pinBState;
                int8_t stateChange = (encoders[i].lastState <<2) | currentState;
                switch(stateChange){
                    case 0b0001:
                    case 0b0111:
                    case 0b1110:
                    case 0b1000:
                        encoders[i].value++; //moved ClockWise
                        break;
                    case 0b0010:
                    case 0b1011:
                    case 0b1101:
                    case 0b0100:    
                        encoders[i].value--; //moved CounderClockWise
                        break;
                    default:
                        break;
                }
                encoders[i].lastState = currentState;
            }
        };
        int16_t getValue(uint8_t index){
            if(index<MAX_NUMBER_OF_ENCODERS)
                return encoders[index].value;
            else
                return 0;
        };
        void resetValue(uint8_t index){
            if(index<MAX_NUMBER_OF_ENCODERS)   
                encoders[index].value=0;
        };
};  

#endif /* ROTARY_ENCODER_H_ */
