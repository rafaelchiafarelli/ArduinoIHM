This is a simple IHM and a function generator

It is capable of generating pwm signals with fixed and precise frequency with a multitude of resolutions in duty-cycle and also a variable frequency with a variable resolution in duty-cycle.

all the 70 IO´s of the Arduino MEGA ware used in this project.

Timer2 is for time keeping and fast-handler operations (communications, debug and others)


Timers 1,3,4 and 5 are devoted to the primary function of pwm generation.
we have OC5 and OC3 as simple pwm generators
OC1 and OC4 as full featured pwm generators (with 3 outputs each)
Timers 1,3,4 and 5 are fully committed to PWM generation, with none free
for anything else -- DC/stepper motor control (`MotorDC`) instead rides
the existing ~1ms system tick for its (coarse) software-PWM speed
control. Servo control is not part of the IHM solution -- a dedicated
servo controller is planned as separate future work.


