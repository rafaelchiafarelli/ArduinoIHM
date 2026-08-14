#ifndef RELEY_H_
#define RELEY_H_
#include <MultiplexedBus.h>

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
    const MultiplexedBus bus;

    // The bus is byte-wide and shared, not individually addressable per
    // bit -- every change re-latches all 8 relays' current state at once.
    void refreshBus() const{
        uint8_t value = 0;
        for(int i=0;i<NUMBER_OF_RELAYS;i++){
            if(relays[i].enabled && relays[i].state){
                value |= (uint8_t)(1 << i);
            }
        }
        bus.write(MUX_RELAY_STROBE, value);
    }
public:
    Relay(const MultiplexedBus muxBus):bus(muxBus){
        for(int i=0;i<NUMBER_OF_RELAYS;i++){
            relays[i].index=i;
            relays[i].enabled = false;
        }
    };
    void setRelay(uint8_t index, bool state){
        if(index<NUMBER_OF_RELAYS && relays[index].enabled){
            relays[index].state = state;
            refreshBus();
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
            refreshBus();
        }
    };
    void ultra_slow_handler(){
        //refresh the whole latched byte to avoid any unexpected state change
        refreshBus();
    };
};

#endif /* RELEY_H_ */