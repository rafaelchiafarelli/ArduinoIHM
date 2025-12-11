#pragma once

#include <string>

typedef enum {
    STATE_PWM_CONFIG,
    STATE_SERIAL_CONFIG,
    STATE_OUTPUT_CONFIG
    
}FunctionalStates;



class StateMachine {
    private:
        FunctionalStates  currentState;


    public:
        StateMachine():currentState(STATE_PWM_CONFIG){

        }

        void setState(StateMachineStates newState){
            currentState = newState;
        }

        StateMachineStates getState(){
            return currentState;
        }
};