#ifndef _HANDLER_MANAGER_H_
#define _HANDLER_MANAGER_H_

#define ONE_MS_T0_TICKS 4 //
#define TEN_MS_T0_TICKS 40 //



//must use timer 2
class HandlerManager {
    private:
    uint8_t counterT0 = 0;
    uint8_t counterT1 = 0;
    /*functions called every 256us */
     void (*fast_handler[])() = ULTRA_FAST_HANDLER_FUNCTIONS;
     /*functions called every 2,56ms*/
     void (*slow_handler[])() = {func1, func2, func3};
public:
    HandlerManager() {}
    void setup() {
        // Initialization code if needed
        //initialize timers
        //T0 -> 256us
        //T1 -> 10ms
        
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

    }

#endif /* _HANDLER_MANAGER_H_ */
