#ifndef RELEY_H_
#define RELEY_H_
#include <BinaryOutputs.h>

#define NUMBER_OF_RELAYS 8

typedef struct{
    uint8_t index=0;
    bool enabled=false;
    bool state=false;
}relayCtrl;

class Relay
{   
private:
    relayCtrl relays[NUMBER_OF_RELAYS];
    const BinaryOutputs relays_pins;
public:
    Relay(const BinaryOutputs bnOuts):relays_pins(bnOuts){
        for(int i=0;i<NUMBER_OF_RELAYS;i++){
            relays[i].index=i;
            relays[i].enabled = false;
        }
    };
    void setRelay(uint8_t index, bool state){
        if(index<NUMBER_OF_RELAYS && relays[index].enabled){
            relays[index].state = state;
            relays_pins.SetOutput(relays[index].index, (const bool)state);
        }
    };
    void enableRelay(uint8_t index){
        if(index<NUMBER_OF_RELAYS){
            relays[index].enabled = true;
        }
    };
    void disableRelay(uint8_t index){
        if(index<NUMBER_OF_RELAYS){
            relays[index].enabled = false;
            relays[index].state = false;
            relays_pins.SetOutput(relays[index].index, false);
        }
    };
    void ultra_slow_handler(){
        //this function can be used to monitor the relays status or do some maintenance tasks
        for(int i=0;i<NUMBER_OF_RELAYS;i++){
            if(relays[i].enabled){
                //refresh the relay state to avoid any unexpected state change
                relays_pins.SetOutput(relays[i].index, relays[i].state);
            }
        }
    };
};

#endif /* RELEY_H_ */