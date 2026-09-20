# Task 4: pwm-send-script

**Status:** done
**Branch:** `4-pwm-send-script` (from `tasks`)
**Depends on:** task 1 (dialect defines the message). Independent of tasks 2, 3, 5.

## Contract

### In

Nothing from this repo's C code. Needs the `PWM_CHANNEL_CONFIG` definition
from task 1's `mavlink/ihm_dialect.xml`.

### Delivers

1. **A Python binding of the dialect for tooling.** `mavlink/generate.py`
   currently emits C only. Add a second generation pass in the same
   script (`language="Python"`, `output=str(HERE / "generated_py")`) so
   `mavlink/generated_py/ihm_dialect.py` exists and is committed. This is
   the same one-call `mavgen.mavgen(...)` pattern already there, not new
   logic -- but call it out in the commit: `generate.py` now produces two
   trees. Update `mavlink/README.md` to say so.

2. **`mavlink/scripts/pwm_config.py`** -- a pymavlink CLI that sends one
   `PWM_CHANNEL_CONFIG`. Suggested interface:

   ```
   python mavlink/scripts/pwm_config.py --port COM7 [--baud 250000]
       --channel N
       --freq VARIABLE|62500_HZ|31250_HZ|...   (PWM_FREQUENCY entry, suffix or index)
       [--top N]                               (raw ICRn TOP; required iff --freq VARIABLE)
       --a on|off [--a-invert] --a-duty PCT
       [--b on|off --b-invert --b-duty PCT]    (channels 2/3 only; error if given for 0/1)
       [--c on|off --c-invert --c-duty PCT]
   ```

   - loads the dialect from `mavlink/generated_py/` (add that dir to
     `sys.path`, `import ihm_dialect`), opens the port with
     `pymavlink.mavutil.mavlink_connection(port, baud=...)`, and calls
     the generated `mav.pwm_channel_config_send(...)`.
   - after sending, wait up to ~1 s for the next `IHM_BOARD_STATE` and
     print "link OK" / "no telemetry seen" -- there is no ack message, so
     this is the only confirmation the board is alive on the port.
   - `--help` documents every `--freq` value.

3. A one-line pointer in `mavlink/README.md`'s tooling notes to
   `scripts/pwm_config.py`.

## Dependencies

Task 1 only. If task 1 is not yet merged into `tasks`, stop -- the
message shape must be final before the sender is written against it.

## Pre-work

`pymavlink` is installed on this machine (used by `generate.py` already).

## Definition of done

- `python mavlink/generate.py` produces both `generated/` and
  `generated_py/`, both committed.
- `python mavlink/scripts/pwm_config.py --help` works and lists the
  frequency options.
- `platformio run` and `test_native/run_tests.ps1` still pass (this task
  adds no C, but confirm nothing else moved).
- Task file marked done in the same commit.
- Actually sending to a board and seeing the waveform is Rafael's bench
  step (shared with task 3's DoD).

## Notes

- Python bench tooling only -- nothing here is compiled into the
  firmware or affects RAM.
- If adding the Python pass to `generate.py` turns out to need more than
  a few lines (e.g. mavgen Python output needs post-processing), stop and
  flag it -- split the Python-binding generation into its own task rather
  than growing this one.
- Conventional-commits: `build(mavlink): also generate a Python dialect binding`
  then `feat(mavlink): pwm_config.py bench sender for PWM_CHANNEL_CONFIG`.
