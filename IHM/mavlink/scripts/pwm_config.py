#!/usr/bin/env python3
"""Send one PWM_CHANNEL_CONFIG (id 303) to the IHM board over its debug serial port.

Requires: pip install pymavlink pyserial
(the dialect binding lives in mavlink/generated_py/, made by mavlink/generate.py)

Examples
  # simplex channel 0 (OC3A): 62.5 kHz, 25 % duty
  pwm_config.py --port COM7 --channel 0 --freq 62500_HZ --a on --a-duty 25

  # complex channel 2: OC1A 20 % + OC1B inverted 60 %, shared 3.9 kHz timebase
  pwm_config.py --port COM7 --channel 2 --freq 3906_HZ --a on --a-duty 20 \
      --b on --b-invert --b-duty 60

  # variable frequency: raw ICRn TOP (timer period) is required
  pwm_config.py --port COM7 --channel 1 --freq VARIABLE --top 2000 --a on --a-duty 50

There is no ack message: after sending, the script waits ~1 s for the
board's periodic IHM_BOARD_STATE telemetry and reports "link OK" if it
arrives -- the only proof the board is alive on that port. Close any other
program (e.g. the companion app) holding the port first.
"""
import argparse
import pathlib
import sys
import time

HERE = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "generated_py"))
import ihm_dialect  # noqa: E402  (path set up just above)

FREQ_PREFIX = "PWM_FREQUENCY_"
FREQS = {
    name[len(FREQ_PREFIX):]: value
    for value, entry in ihm_dialect.enums["PWM_FREQUENCY"].items()
    for name in [entry.name]
}


def parse_freq(text):
    """Accept an enum suffix (62500_HZ, VARIABLE), the full name, or a raw index."""
    key = text.upper()
    if key.startswith(FREQ_PREFIX):
        key = key[len(FREQ_PREFIX):]
    if key in FREQS:
        return FREQS[key]
    if key.isdigit() and int(key) in FREQS.values():
        return int(key)
    raise argparse.ArgumentTypeError(
        f"unknown frequency {text!r}; choose one of: " + ", ".join(FREQS))


def build_parser():
    p = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--port", required=True, help="serial port, e.g. COM7 or /dev/ttyACM0")
    p.add_argument("--baud", type=int, default=250000)
    p.add_argument("--channel", type=int, required=True, choices=range(4),
                   help="0,1 = simplex (output A only); 2,3 = complex (A/B/C)")
    p.add_argument("--freq", type=parse_freq, required=True,
                   help="frequency selector: " + ", ".join(FREQS) + " (or its index 0-13)")
    p.add_argument("--top", type=int, default=0,
                   help="raw ICRn TOP (1-65535); required iff --freq VARIABLE, otherwise ignored")
    for out in "abc":
        p.add_argument(f"--{out}", choices=("on", "off"), default="off",
                       help=f"output {out.upper()} enable (default off)")
        p.add_argument(f"--{out}-invert", action="store_true",
                       help=f"output {out.upper()} inverting edge")
        p.add_argument(f"--{out}-duty", type=int, default=0, metavar="PCT",
                       help=f"output {out.upper()} duty 0-100 %% (default 0)")
    return p


def validate(args, parser):
    variable = args.freq == FREQS["VARIABLE"]
    if variable and not 1 <= args.top <= 0xFFFF:
        parser.error("--freq VARIABLE requires --top in 1..65535")
    if not variable and args.top:
        parser.error("--top is only valid with --freq VARIABLE")
    for out in "abc":
        if not 0 <= getattr(args, f"{out}_duty") <= 100:
            parser.error(f"--{out}-duty must be 0..100")
    if args.channel < 2:
        for out in "bc":
            if getattr(args, out) == "on" or getattr(args, f"{out}_invert") or getattr(args, f"{out}_duty"):
                parser.error(f"--{out}* options are only valid for channels 2 and 3 "
                             "(0 and 1 have a single output, A)")


def main(argv=None):
    parser = build_parser()
    args = parser.parse_args(argv)
    validate(args, parser)

    import serial  # imported late so --help works without pyserial installed
    port = serial.Serial(args.port, args.baud, timeout=0.1)
    mav = ihm_dialect.MAVLink(port, srcSystem=255, srcComponent=0)
    mav.pwm_channel_config_send(
        args.channel, args.freq, args.top,
        int(args.a == "on"), int(args.a_invert), args.a_duty,
        int(args.b == "on"), int(args.b_invert), args.b_duty,
        int(args.c == "on"), int(args.c_invert), args.c_duty)
    print(f"sent PWM_CHANNEL_CONFIG channel={args.channel} freq={args.freq}")

    deadline = time.time() + 1.0
    while time.time() < deadline:
        data = port.read(64)
        for byte in data:
            try:
                msg = mav.parse_char(bytes([byte]))
            except Exception:
                continue  # noise / partial frame; keep scanning
            if msg is not None and msg.get_type() == "IHM_BOARD_STATE":
                print("link OK (board telemetry seen)")
                return 0
    print("no telemetry seen -- board may not be on this port/baud, or may not be running")
    return 1


if __name__ == "__main__":
    sys.exit(main())
