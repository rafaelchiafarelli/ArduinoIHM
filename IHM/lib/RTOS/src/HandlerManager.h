#ifndef _HANDLER_MANAGER_H_
#define _HANDLER_MANAGER_H_

#define ONE_MS_T0_TICKS 4 //
#define TEN_MS_T0_TICKS 40 //
#define ONE_HUNDRED_MS_T1_TICKS 20 //

#define ONE_SECOND_T1_TICKS 200 //

//must use timer 2
class HandlerManager {
    private:
    uint8_t counterT0 = 0;
    uint8_t counterT1 = 0;
public:
    HandlerManager() {}
    void setup() {
        // Initialization code if needed
        //initialize timers
        //T0 -> 256us
        //T1 -> 5ms
        
    }
    /**
     * timer0 has 3 sources for interrupts:
     * Output Compare Match A --> is connected to the regular fast handler of 256us --> OCA= 64
     * Output Compare Match B --> is connected to the warnning handler of 512us --> OCB= 128
     * Overflow --> is connected to the emergency handler of 1024us (t0 Overflow)
     */
    void t0Tick_handler() {
        //module1.fast_handler();
        //module2.fast_handler();
        counterT0++;
        if (counterT0 >= TEN_MS_T0_TICKS) {
            counterT0 = 0;
            // Call the handler functions of various modules every 10ms
            // Example:
            // module1.ten_ms_handler();
            // module2.ten_ms_handler();
        }
        //reset the T0 timer; 

    }
    void t1Tick_handler() {
        // Call the handler functions of various modules
        // Example:
        // module1.handler();
        // module2.handler();
        counterT1++;
        if (counterT1 >= ONE_HUNDRED_MS_T1_TICKS) {
            counterT1 = 0;
            // Call the handler functions of various modules every 100ms
            // Example:
            // module1.hundred_ms_handler();      
            // module2.hundred_ms_handler();
        }
    }

#endif /* _HANDLER_MANAGER_H_ */
