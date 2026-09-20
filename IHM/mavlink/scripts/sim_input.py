#!/usr/bin/env python3
"""Inject simulated user input into the IHM board over MAVLink.

Requires: pip install pymavlink pyserial

Examples
  sim_input.py --port COM7 button rot1        # click rot1's push button
  sim_input.py --port COM7 button 0 3         # push buttons 0 and 3 together
  sim_input.py --port COM7 encoder 0 cw       # one clockwise step on encoder 0
  sim_input.py --port COM7 encoder 1 ccw --repeat 5

Buttons: 0-3 = push buttons, rot2 / rot1 / rot0 = encoder push buttons
(bits 4/5/6, same layout as IHM_BOARD_STATE.buttons). A button press is a
one-superloop-pass pulse; encoders apply one step per message. Real input
always wins. No ack -- watch the board's screen or IHM_BOARD_STATE.
"""
import argparse
import pathlib
import sys
import time

HERE = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "generated_py"))
import ihm_dialect  # noqa: E402

BUTTONS = {"rot2": 4, "rot1": 5, "rot0": 6}
BUTTONS.update({str(i): i for i in range(4)})
DIRS = {"cw": None, "ccw": None}  # values resolved from the dialect enum in main()


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--port", required=True)
    p.add_argument("--baud", type=int, default=250000)
    p.add_argument("--repeat", type=int, default=1)
    p.add_argument("--gap", type=float, default=0.1, help="seconds between repeats")
    sub = p.add_subparsers(dest="cmd", required=True)
    b = sub.add_parser("button")
    b.add_argument("buttons", nargs="+", choices=sorted(BUTTONS))
    e = sub.add_parser("encoder")
    e.add_argument("encoder", type=int, choices=range(3))
    e.add_argument("direction", choices=sorted(DIRS))
    args = p.parse_args(argv)

    names = {v.name: k for k, v in ihm_dialect.enums["IHM_ENCODER_DIRECTION"].items()}
    dirs = {"cw": names["IHM_ENCODER_CW"], "ccw": names["IHM_ENCODER_CCW"]}

    import serial
    port = serial.Serial(args.port, args.baud, timeout=0.1)
    mav = ihm_dialect.MAVLink(port, srcSystem=255, srcComponent=0)
    for i in range(args.repeat):
        if args.cmd == "button":
            mask = 0
            for name in args.buttons:
                mask |= 1 << BUTTONS[name]
            mav.ihm_simulate_button_send(mask)
        else:
            mav.ihm_simulate_encoder_send(args.encoder, dirs[args.direction])
        if i + 1 < args.repeat:
            time.sleep(args.gap)
    print(f"sent {args.repeat}x {args.cmd}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
