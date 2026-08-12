# StateMachine

Two unrelated things share this folder:

## `PWMStateMachine.h` -- actually used

Pure UI-cursor bookkeeping for [Elements](../Elements/README.md)'
`PWMScreen`: which of the 4 PWM channels is selected, and which field
within that channel's regular (simplex) or shared (complex) field set is
selected. No hardware, no handler -- driven synchronously by
`PWMScreen`'s `selectNext/PreviousChannel`/`selectNext/PreviousField`.
Has native test coverage (`test_native/test_pwm_state_machine.cpp`).

## `StateMachine.hpp` / `StateMachine.cpp` -- dead code, doesn't compile

A separate, more generic-looking `StateMachine` class (`FunctionalStates`
enum: `STATE_PWM_CONFIG`/`STATE_SERIAL_CONFIG`/`STATE_OUTPUT_CONFIG`).
`setState`/`getState` reference an undefined type `StateMachineStates`
(the enum is actually named `FunctionalStates`) -- this would fail to
compile if anything tried to build it. `StateMachine.cpp` is empty.
Nothing in `lib/` or `src/` includes `StateMachine.hpp` (only
`PWMStateMachine.h` is used, confirmed by grep).

Reads like an earlier, abandoned attempt at a generic base class that
`PWMStateMachine` superseded without the old file being removed. Safe to
delete whenever convenient; flagged here so it isn't mistaken for
in-progress work.
