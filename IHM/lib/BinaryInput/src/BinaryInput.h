#pragma once
#include <avr/pgmspace.h>
#include <avr/io.h>

#ifndef _PIND
#define _PIND 0x09
#define _PORTD 0x0B
#endif
#ifndef _DDRD
#define _DDRD 0x0A
#endif
#ifndef _PING
#define _PING 0x12
#define _PORTG 0x14
#endif
#ifndef _DDRG
#define _DDRG 0x13
#endif
#ifndef _PINL
#define _PINL 0x109
#define _PORTL 0x10B
#endif
#ifndef _DDRL
#define _DDRL 0x10a
#endif
#ifndef _PINB
#define _PINB 0x03
#define _PORTB 0x05

#endif
#ifndef _DDRB
#define _DDRB 0x04
#endif

#define MAX_PIN_AMOUNT 16
#define MAX_PIN_PORT 5
typedef struct
{
    volatile uint8_t * pins;
    volatile uint8_t * pull_up;
    volatile uint8_t * ddr;
    uint8_t ddr_mask;
    
    
} input_port;

typedef struct
{
    uint8_t value;
}input_value;

typedef struct 
{
    uint8_t idx;
    uint8_t mask;
    
}pin_to_index;

class BinaryInputs
{
public:
    input_value in[MAX_PIN_PORT];
    const input_port ports[MAX_PIN_PORT];
    const pin_to_index pins[MAX_PIN_AMOUNT];
    
    void fast_handler()
    {
        in[0].value = *ports[0].pins;
        in[1].value = *ports[1].pins;
        in[2].value = *ports[2].pins;
        in[3].value = *ports[3].pins;
        in[4].value = *ports[4].pins;
    };
    bool get_pin(uint8_t index){
        if(index<MAX_PIN_AMOUNT)
            return ((in[pins[index].idx].value & pins[index].mask) == 0);
        else
            return false;
    }

    BinaryInputs() : ports({
                            {&PINA,&PORTA, &DDRA, 0b00010101},    /* PORTA */
                            {&PINC,&PORTC, &DDRC, 0b10110000},    /* PORTC */
                            {&PINE,&PORTE, &DDRE, 0b00010000},    /* PORTE */
                            {&PINF,&PORTF, &DDRF, 0b11100000},    /* PORTF */
                            {&PINL,&PORTL, &DDRL, 0b01110111},    /* PORTL */
                            }),
                    pins({  
                            {4,0b00010000}, /*PA4*/
                            {4,0b00010000}, /*PE4*/
                            {5,0b00100000}, /*PF5*/
                            {6,0b01000000}, /*PF6*/
                            {0,0b00000001}, /*PA0 */
                            {2,0b00000100}, /*PA2*/
                            {7,0b10000000}, /*PF7*/
                            {6,0b01000000}, /*PL6*/
                            {5,0b00100000}, /*PL5*/
                            {4,0b00010000}, /*PL4*/
                            {2,0b00000100}, /*PL2*/
                            {1,0b00000010}, /*PL1*/
                            {0,0b00000001}, /*PL0*/
                            {4,0b00010000}, /*PC4*/
                            {5,0b00100000}, /*PC5*/
                            {7,0b10000000} /*PC7*/
                            })
    {
        DDRA = ports[0].ddr_mask;
        DDRC = ports[1].ddr_mask;
        DDRE = ports[2].ddr_mask;
        DDRF = ports[3].ddr_mask;
        DDRL = ports[4].ddr_mask;
        MCUCR |= ~(1<<PUD);
        *ports[0].pull_up = ~ports[0].ddr_mask;
        *ports[1].pull_up = ~ports[1].ddr_mask;
        *ports[2].pull_up = ~ports[2].ddr_mask;
        *ports[3].pull_up = ~ports[3].ddr_mask;
        *ports[4].pull_up = ~ports[3].ddr_mask;
    };
};
