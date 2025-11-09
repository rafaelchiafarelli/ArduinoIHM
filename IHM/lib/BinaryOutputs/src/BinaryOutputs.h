#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>

#define MAX_OUTPUT_SIZE 19

typedef struct
{
    volatile uint8_t * port;
    volatile uint8_t * ddr;
    const uint8_t mask_set;
    const uint8_t mask_reset;
    
} out_port;




class BinaryOutputs
{
public:
    out_port ports[MAX_OUTPUT_SIZE];


    void SetOutput(int i, bool v)
    {
        if(v) //set the output
            *ports[i].port |= ports[i].mask_set;
        else //reset the output
            *ports[i].port &= ports[i].mask_reset;
    }
    BinaryOutputs() : ports({
                            {&PORTC, &DDRC,(uint8_t)0b00000100,(uint8_t)0b11111011},                    
                            {&PORTC, &DDRC,(uint8_t)0b00000010,(uint8_t)0b11111101},
                            {&PORTC, &DDRC,(uint8_t)0b00000001,(uint8_t)0b11111110},
                            {&PORTD, &DDRD,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PORTG, &DDRG,(uint8_t)0b00000100,(uint8_t)0b11111011},
                            {&PORTG, &DDRG,(uint8_t)0b00000010,(uint8_t)0b11111101},
                            {&PORTG, &DDRG,(uint8_t)0b00000001,(uint8_t)0b11111110},
                            {&PORTL, &DDRL,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PORTB, &DDRB,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PORTB, &DDRB,(uint8_t)0b01000000,(uint8_t)0b10111111},
                            {&PORTB, &DDRB,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PORTB, &DDRB,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PORTH, &DDRH,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PORTH, &DDRH,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PORTG, &DDRG,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PORTF, &DDRF,(uint8_t)0b00010000,(uint8_t)0b11101111}
                        })
    {
        for(int i=0; i<MAX_OUTPUT_SIZE; i++)
        {
            *ports[i].ddr |= ports[i].mask_set;
            *ports[i].port &= ports[i].mask_reset;
        }
    };
};