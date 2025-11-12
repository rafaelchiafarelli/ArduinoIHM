#include "ServoMotor.h"

void ServoMotor::load(uint16_t *ar)
{
    for (int i = 0; i < NUMBER_OF_ENGINES; i++)
    {
        if (ar[i] <= pos_max )
            engines[i].position = ar[i];
        else
            engines[i].position = MAX_POSITION-MIN_POSITION;
    }
}
void ServoMotor::load(uint16_t ar,uint8_t index)
{
    if (index < NUMBER_OF_ENGINES)
    {
        if (ar <= MIN_POSITION && ar>=0 )
            engines[index].position = ar;
        else
            engines[index].position = MAX_POSITION-MIN_POSITION;
    }
}
void ServoMotor::timer_handler()
{
    switch (state)
    {
    case SERVO_UP:
        OCR4A = MIN_POSITION - engines[counter].position;
        state = SERVO_DOWN;
        if(engines[counter].enabled) {
            engines_pins.SetOutput(engines[counter].index, false);
        }
        break;
    case SERVO_DOWN:
        counter++;
        if (counter >= NUMBER_OF_ENGINES)
        {
            counter = 0;
        }
        OCR4A = MAX_POSITION - engines[counter].position;
        if(engines[counter].enabled) {
            engines_pins.SetOutput(engines[counter].index, true);
        }
        state = SERVO_UP;
        break;
    case SERVO_WAIT:

        break;
    }
}