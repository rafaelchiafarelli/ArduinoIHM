#ifndef JANUS_GEN_BINDINGS_H
#define JANUS_GEN_BINDINGS_H

typedef struct {
    int ch0_enabled;
    int ch0_inverting;
    int ch0_frequency;
    int ch0_variable_top;
    int ch0_duty_percent;
    int ch1_enabled;
    int ch1_inverting;
    int ch1_frequency;
    int ch1_variable_top;
    int ch1_duty_percent;
    int ch2_frequency;
    int ch2_variable_top;
    int ch2_a_enabled;
    int ch2_a_duty_percent;
    int ch2_b_enabled;
    int ch2_b_duty_percent;
    int ch2_c_enabled;
    int ch2_c_duty_percent;
    int ch3_frequency;
    int ch3_variable_top;
    int ch3_a_enabled;
    int ch3_a_duty_percent;
    int ch3_b_enabled;
    int ch3_b_duty_percent;
    int ch3_c_enabled;
    int ch3_c_duty_percent;
} pwm_t;

extern pwm_t pwm_instance;

typedef struct {
    int can0_enabled;
    int can0_id;
    int can0_dlc;
    int can0_extended;
    int can1_enabled;
    int can1_id;
    int can1_dlc;
    int can1_extended;
    int rs485_enabled;
    int rs485_length;
    int rs485_period_ms;
} bus_status_t;

extern bus_status_t bus_status_instance;

typedef struct {
    int relay_0;
    int relay_1;
    int relay_2;
    int relay_3;
    int relay_4;
    int relay_5;
    int relay_6;
    int relay_7;
} relay_t;

extern relay_t relay_instance;

#endif  /* JANUS_GEN_BINDINGS_H */
