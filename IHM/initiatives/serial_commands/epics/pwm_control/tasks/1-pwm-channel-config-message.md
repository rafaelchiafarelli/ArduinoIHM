# Task 1: pwm-channel-config-message

**Status:** done
**Branch:** `1-pwm-channel-config-message` (from `tasks`)
**Depends on:** nothing

## Contract

### In

Nothing. This is a pure protocol-definition change.

### Delivers

1. **`mavlink/ihm_dialect.xml`** gains:

   - **enum `PWM_FREQUENCY`** -- one entry per `PWMFrequency` value in
     `lib/MultiOutput/src/PWMTiming.h`, in the same order, same implied
     values (0-based):

     | value | name | mirrors |
     |---|---|---|
     | 0 | `PWM_FREQUENCY_62500_HZ` | `frequency_62_500HZ` |
     | 1 | `PWM_FREQUENCY_31250_HZ` | `frequency_31_250HZ` |
     | 2 | `PWM_FREQUENCY_15625_HZ` | `frequency_15_625HZ` |
     | 3 | `PWM_FREQUENCY_7812_HZ` | `frequency_7812_5HZ` |
     | 4 | `PWM_FREQUENCY_3906_HZ` | `frequency_3906_25HZ` |
     | 5 | `PWM_FREQUENCY_1953_HZ` | `frequency_1953_125HZ` |
     | 6 | `PWM_FREQUENCY_976_HZ` | `frequency_976_5625HZ` |
     | 7 | `PWM_FREQUENCY_488_HZ` | `frequency_488_28125HZ` |
     | 8 | `PWM_FREQUENCY_244_HZ` | `frequency_244_140625HZ` |
     | 9 | `PWM_FREQUENCY_122_HZ` | `frequency_122_0703125HZ` |
     | 10 | `PWM_FREQUENCY_61_HZ` | `frequency_61_03515625HZ` |
     | 11 | `PWM_FREQUENCY_30_HZ` | `frequency_30_517578125HZ` |
     | 12 | `PWM_FREQUENCY_15_HZ` | `frequency_15_2587890625HZ` |
     | 13 | `PWM_FREQUENCY_VARIABLE` | `frequency_variable` |

     Add a `<description>` on the enum saying it mirrors `PWMFrequency`
     exactly and the two must stay in sync -- same wording style as
     `IHM_ENCODER_DIRECTION`'s note about `DIRECTION_TYPE`. Do **not**
     add an entry for `NUMBER_OF_PWM_FREQUENCIES` (it's a count, not a
     value).

   - **message `PWM_CHANNEL_CONFIG`, id 303**, PC -> board. Fields in
     this order:

     ```
     uint8_t  channel              # 0-3
     uint8_t  f_selector           # enum PWM_FREQUENCY
     uint16_t frequency            # raw ICRn TOP; only used when f_selector == PWM_FREQUENCY_VARIABLE
     uint8_t  out1_enabled         # output A
     uint8_t  out1_inverting
     uint8_t  out1_duty_percent    # 0-100
     uint8_t  out2_enabled         # output B -- ignored for channels 0 and 1 (simplex)
     uint8_t  out2_inverting
     uint8_t  out2_duty_percent
     uint8_t  out3_enabled         # output C -- ignored for channels 0 and 1 (simplex)
     uint8_t  out3_inverting
     uint8_t  out3_duty_percent
     ```

     Message `<description>`: PC configures one PWM channel; one message
     covers both simplex (0/1, output A only) and complex (2/3, A/B/C)
     channels; board applies immediately, no ack. Keep the "not
     bit-packed across byte boundaries on purpose" convention note
     consistent with the other messages.

2. **`mavlink/generated/`** regenerated: `python mavlink/generate.py`.
   The new `mavlink/generated/ihm_dialect/mavlink_msg_pwm_channel_config.h`
   and the touched shared headers (`ihm_dialect.h`, `mavlink.h`,
   `testsuite.h`, `version.h`) are committed -- `generated/` is committed
   output by policy (see `mavlink/README.md`), a stale one is a real bug.

3. **`mavlink/README.md`** -- add the `PWM_CHANNEL_CONFIG` row to the
   Messages table (direction PC -> board, 13 bytes). Leave the "largest
   message" / 64-byte cap sentence alone -- `RS485_SIGNAL_CONFIG` at 38
   is still the largest.

## Dependencies

None. `PWMFrequency` in `PWMTiming.h` is the source of truth the enum
mirrors; that file is not modified.

## Pre-work

Already done: `pymavlink` is installed on this machine
(`C:\Users\rafae\AppData\Roaming\Python\Python314\site-packages\pymavlink`).
`generate.py` only needs `pymavlink.generator.mavgen`.

## Definition of done

- `python mavlink/generate.py` runs clean; `git status` shows only
  `ihm_dialect.xml`, files under `mavlink/generated/`, and
  `mavlink/README.md`.
- `platformio run` (env `megaatmega2560`) builds -- the regenerated
  headers must not break the AVR build that already `#include`s this
  dialect.
- `test_native/run_tests.ps1` passes (unchanged, but confirm).
- Task file marked done in the same commit.

## Notes

- Message ids so far: 300 `IHM_BOARD_STATE`, 301 `CAN_SIGNAL_CONFIG`,
  302 `RS485_SIGNAL_CONFIG`. 303 is next.
- No `<include>common.xml</include>` -- this message references nothing
  from the standard dialect, same as the existing three.
- Conventional-commit: `feat(mavlink): add PWM_CHANNEL_CONFIG message`.
