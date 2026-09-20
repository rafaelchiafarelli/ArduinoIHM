This is a simple IHM and a function generator

It is capable of generating pwm signals with fixed and precise frequency with a multitude of resolutions in duty-cycle and also a variable frequency with a variable resolution in duty-cycle.

all the 70 IO´s of the Arduino MEGA ware used in this project.

## Driving it from a PC

The board takes commands over its debug serial port (250000 baud, private
MAVLink dialect in [mavlink/](mavlink/README.md)). Two ways to send them:

- **PC companion app** -- `C:\Users\rafae\source\repos\IHMPCController`
  (Win32). Live inputs/relays, simulated encoder turns, and a PWM command
  panel. Build/use: its `HOW_TO_USE.md`.
- **Scripts** -- `python mavlink/scripts/pwm_config.py --help`.

Today's commands: `PWM_CHANNEL_CONFIG` (4 PWM channels), `IHM_SIMULATE_ENCODER`.
Worked bench examples: [demo/HARDWARE_RUNBOOK.md](demo/HARDWARE_RUNBOOK.md)
("Driving PWM over MAVLink"). Plans and status:
[initiatives/serial_commands/](initiatives/serial_commands/README.md).

Timer2 is for time keeping and fast-handler operations (communications, debug and others)


Timers 1,3,4 and 5 are devoted to the primary function of pwm generation.
we have OC5 and OC3 as simple pwm generators
OC1 and OC4 as full featured pwm generators (with 3 outputs each)
Timers 1,3,4 and 5 are fully committed to PWM generation, with none free
for anything else -- DC/stepper motor control (`MotorDC`) instead rides
the existing ~1ms system tick for its (coarse) software-PWM speed
control. Servo control is not part of the IHM solution -- a dedicated
servo controller is planned as separate future work.


