#ifndef BINARYINPUTS_H
#define BINARYINPUTS_H

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <Ports.h>

#define MAX_PIN_AMOUNT 15
#define MAX_PIN_PORT 5

typedef struct
{
    uint8_t value;
    volatile uint8_t * pins;    
}input_value;

typedef struct 
{
    uint8_t idx;
    uint8_t mask;
    
}pin_to_index;

class BinaryInputs
{
public:
    const port_type ports[MAX_PIN_AMOUNT];
    const pin_to_index pins[MAX_PIN_AMOUNT];
    input_value in[MAX_PIN_PORT];

    
    void fast_handler()
    {
        in[0].value = *in[0].pins;
        in[1].value = *in[1].pins;
        in[2].value = *in[2].pins;
        in[3].value = *in[3].pins;
        in[4].value = *in[4].pins;
    };
    bool get_pin(uint8_t index) const{
        if(index<MAX_PIN_AMOUNT)
            return !((in[pins[index].idx].value & pins[index].mask) == 0);
        else
            return false;
    }

    BinaryInputs(): ports({
                            {&PINA, &PORTA, &DDRA,(uint8_t)0b00010000,(uint8_t)0b11101111},                    
                            {&PINE, &PORTE, &DDRE,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PINF, &PORTF, &DDRF,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINF, &PORTF, &DDRF,(uint8_t)0b01000000,(uint8_t)0b10111111},
                            {&PINA, &PORTA, &DDRA,(uint8_t)0b00000001,(uint8_t)0b11111110},
                            {&PINA, &PORTA, &DDRA,(uint8_t)0b00000100,(uint8_t)0b11111011},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b01000000,(uint8_t)0b10111111},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00000100,(uint8_t)0b11111011},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00000010,(uint8_t)0b11111101},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00000001,(uint8_t)0b11111110},
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b10000000,(uint8_t)0b01111111}
                        }),
                    pins({  
                            {0,0b00010000}, /*PA4 push-btn*/
                            {2,0b00010000}, /*PE4 push-btn*/
                            {3,0b00100000}, /*PF5 push-btn*/
                            {3,0b01000000}, /*PF6 push-btn*/
                            {0,0b00000001}, /*PA0 house-keeping*/
                            {0,0b00000100}, /*PA2 house-keeping*/
                            {4,0b01000000}, /*PL6 rotary-encoder*/
                            {4,0b00100000}, /*PL5 rotary-encoder*/
                            {4,0b00010000}, /*PL4 push-btn*/
                            {4,0b00000100}, /*PL2 rotary-encoder*/
                            {4,0b00000010}, /*PL1 rotary-encoder*/
                            {4,0b00000001}, /*PL0 push-btn*/
                            {1,0b00010000}, /*PC4 rotary-encoder*/
                            {1,0b00100000}, /*PC5 rotary-encoder*/
                            {1,0b10000000}  /*PC7 push-btn*/
                            }),
                    in({
                            {0x00, &PINA},    /* PORTA */
                            {0x00, &PINC},    /* PORTC */
                            {0x00, &PINE},    /* PORTE */
                            {0x00, &PINF},    /* PORTF */
                            {0x00, &PINL}    /* PORTL */
                    })
    {
        for(int i=0; i<MAX_PIN_AMOUNT; i++)
        {
            *ports[i].ddr &= ports[i].mask_reset;
            *ports[i].port |= ports[i].mask_set;
        }
        MCUCR |= ~(1<<PUD);

    };
};

#endif /* BINARYINPUTS_H */