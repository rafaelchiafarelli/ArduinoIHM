#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_
#include <Arduino.h>
#include <avr/io.h>
#include <BinaryInput.h>
#define MAX_NUMBER_EMCODERS 3

typedef enum
{
	not_supported = 0,
	CCW,
	CW
}DIRECTION_TYPE;

typedef struct{
	union{
		struct{
		unsigned s1:1;
		unsigned s2:1;
		unsigned ls1:1;
		unsigned ls2:1;
		};
		unsigned byte;
	};
	int Pin0;
	int Pin1;
	DIRECTION_TYPE direction;
}ENCODER_TYPE;

class RotaryEncoder
{
    private:
        BinaryInputs *inputs;
        const DIRECTION_TYPE cDirection[16] = {not_supported,CW,CCW,not_supported,CCW,not_supported,not_supported,CW,CW,not_supported,not_supported,CCW,not_supported,CCW,CW,not_supported};
        ENCODER_TYPE encoders[MAX_NUMBER_EMCODERS];
        
    public:
        RotaryEncoder(BinaryInputs *binInputs):inputs(binInputs){
            
            encoders[0].byte = 0;
            encoders[0].Pin0 = 6;
            encoders[0].Pin1 = 7;
            encoders[0].direction = not_supported;
            
            encoders[1].byte = 0;
            encoders[1].Pin0 = 9;
            encoders[1].Pin1 = 11;
            encoders[1].direction = not_supported;
            
            encoders[2].byte = 0;
            encoders[2].Pin0 = 12;
            encoders[2].Pin1 = 13;
            encoders[2].direction = not_supported;
            
        };
        void ms_handler(uint16_t bMap){
            for(uint8_t i=0;i<MAX_NUMBER_EMCODERS;i++){
                encoders[i].s1 = 0x0001 & (bMap>>encoders[i].Pin0);
                encoders[i].s2 = 0x0001 & (bMap>>encoders[i].Pin1);
                if((encoders[i].ls1 != encoders[i].s1)||(encoders[i].ls2 != encoders[i].s2))
		            {//there was a movement for this encoder
                        if(((encoders[i].ls1 == 1) && (encoders[i].ls2 == 1))||
                            ((encoders[i].ls1 == 0) && (encoders[i].ls2 == 0)))
                            {
                                encoders[i].direction = cDirection[encoders[i].byte];
                            }
                    }
                encoders[i].ls1 = encoders[i].s1;
                encoders[i].ls2 = encoders[i].s2;
                }
        };
        DIRECTION_TYPE getDirection(uint8_t enc){
            DIRECTION_TYPE ret = encoders[enc].direction;
            encoders[enc].direction = not_supported;
            return ret;
        }

        
};  

#endif /* ROTARY_ENCODER_H_ */
