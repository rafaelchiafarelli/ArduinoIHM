#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <Ports.h>
#define MAX_OUTPUT_SIZE 20




class BinaryOutputs
{
    private:
        const port_type ports[MAX_OUTPUT_SIZE];
    public:

        void SetOutput(uint8_t i, const bool v) const
        {
            if(i<MAX_OUTPUT_SIZE) {
                if(v) //set the output
                
                    *ports[i].port |= ports[i].mask_set;
                else //reset the output
                    *ports[i].port &= ports[i].mask_reset;
            }
        }

        BinaryOutputs() : ports({
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b00000001,(uint8_t)0b11111110},                    
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b00000010,(uint8_t)0b11111101},
                            {&PINC, &PORTC, &DDRC,(uint8_t)0b00000100,(uint8_t)0b11111011},
                            {&PIND, &PORTD, &DDRD,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PING, &PORTG, &DDRG,(uint8_t)0b00000100,(uint8_t)0b11111011},
                            {&PING, &PORTG, &DDRG,(uint8_t)0b00000010,(uint8_t)0b11111101},
                            {&PING, &PORTG, &DDRG,(uint8_t)0b00000001,(uint8_t)0b11111110},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PINB, &PORTB, &DDRB,(uint8_t)0b10000000,(uint8_t)0b01111111},
                            {&PINB, &PORTB, &DDRB,(uint8_t)0b01000000,(uint8_t)0b10111111},
                            {&PINH, &PORTH, &DDRH,(uint8_t)0b01000000,(uint8_t)0b10111111},
                            {&PINB, &PORTB, &DDRB,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PINH, &PORTH, &DDRH,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINH, &PORTH, &DDRH,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PING, &PORTG, &DDRG,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINF, &PORTF, &DDRF,(uint8_t)0b00010000,(uint8_t)0b11101111},
                            {&PINE, &PORTE, &DDRE,(uint8_t)0b00001000,(uint8_t)0b11110111},
                            {&PINH, &PORTH, &DDRH,(uint8_t)0b00001000,(uint8_t)0b11110111},
                            {&PINB, &PORTB, &DDRB,(uint8_t)0b00100000,(uint8_t)0b11011111},
                            {&PINL, &PORTL, &DDRL,(uint8_t)0b00001000,(uint8_t)0b11110111}
                        })
        {
            setup();
        };
        void setup() const{
            for(int i=0; i<MAX_OUTPUT_SIZE; i++)
            {
                *ports[i].ddr |= ports[i].mask_set;
                *ports[i].port &= ports[i].mask_reset;
            }
        }
};