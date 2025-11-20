#ifndef PORTS_H
#define PORTS_H

typedef struct
{
    volatile uint8_t * pins;
    volatile uint8_t * port;
    volatile uint8_t * ddr;
    const uint8_t mask_set;
    const uint8_t mask_reset;
    
} port_type;


#endif /* PORTS_H */