#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_
#include <stdint.h>
#define MAX_NUMBER_EMCODERS 3

// Only a BinaryInputs* is ever stored (never dereferenced -- ms_handler and
// getDirection work purely off the bMap value passed in), so a forward
// declaration is enough here and keeps this class free of BinaryInput.h's
// avr/io.h dependency -- which is what makes it possible to unit test the
// quadrature decode logic on the host.
class BinaryInputs;

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
        // Pin0/Pin1 are BinaryInputs pin-table indices (see BinaryInput.h):
        // each rotary encoder module occupies 3 consecutive indices -- two
        // quadrature signal pins plus one push-button -- so Pin0/Pin1 here
        // must be the two *signal* pins, never the button's index.
        //   encoder0: PL6(6)/PL5(7), button PL4(8)
        //   encoder1: PL2(9)/PL1(10), button PL0(11)
        //   encoder2: PC4(12)/PC5(13), button PC7(14)
        RotaryEncoder(BinaryInputs *binInputs):inputs(binInputs){

            encoders[0].byte = 0;
            encoders[0].Pin0 = 6;
            encoders[0].Pin1 = 7;
            encoders[0].direction = not_supported;

            encoders[1].byte = 0;
            encoders[1].Pin0 = 9;
            encoders[1].Pin1 = 10;
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
