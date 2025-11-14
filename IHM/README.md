This is a simple IHM and a function generator

It is capable of generating pwm signals with fixed and precise frequency with a multitude of resolutions in duty-cycle and also a variable frequency with a variable resolution in duty-cycle.

all the 70 IO´s of the Arduino MEGA ware used in this project.

Timer2 is for time keeping and fast-handler operations (communications, debug and others)


Timers 1,3,4 and 5 are devoted to the primary function of pwm generation.
we have OC5 and OC3 as simple pwm generators
OC1 and OC4 as full featured pwm generators (with 3 outputs each)
Timers 1,3,4 or 5 could be used for Servo control and DC motor (both as DC and stepper) as a time keeper and output control.


