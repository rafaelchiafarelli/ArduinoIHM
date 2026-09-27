#!/usr/bin/env python3
"""Send one IHM_RELAY_COMMAND (id 309) to the IHM board over its Serial2 port.

Requires: pip install pymavlink pyserial
(the dialect binding lives in mavlink/generated_py/, made by mavlink/generate.py)

Examples
  # relay 0 on
  relay_cmd.py --port COMx --set 0=1

  # relay 0 off and relay 3 on in one frame; the others are left alone
  relay_cmd.py --port COMx --set 0=0 3=1

  # every relay off
  relay_cmd.py --port COMx --set 0=0 1=0 2=0 3=0 4=0 5=0 6=0 7=0

There is no ack message: after sending, the script waits for the board's
periodic IHM_RELAY_STATE (304) and prints it. The first 304 after ~300 ms is
used, since one sent before the board applied the command can still be in
flight. Close any other program (e.g. the companion app) holding the port
first.
"""
import argparse
import pathlib
import sys
import time

HERE = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "generated_py"))
import ihm_dialect  # noqa: E402  (path set up just above)


def parse_assignment(text):
    """Accept RELAY=STATE, relay 0-7, state 0/1/on/off."""
    try:
        relay, state = text.split("=", 1)
        relay = int(relay)
        state = {"0": 0, "off": 0, "1": 1, "on": 1}[state.lower()]
    except (ValueError, KeyError):
        raise argparse.ArgumentTypeError(f"expected RELAY=STATE (e.g. 3=1 or 3=off), got {text!r}")
    if not 0 <= relay <= 7:
        raise argparse.ArgumentTypeError(f"relay must be 0..7, got {relay}")
    return relay, state


def build_parser():
    p = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--port", required=True, help="the Serial2 USB-TTL adapter's port, e.g. COMx or /dev/ttyUSB0")
    p.add_argument("--baud", type=int, default=250000)
    p.add_argument("--set", nargs="+", type=parse_assignment, required=True, metavar="RELAY=STATE",
                   help="one or more relay assignments, e.g. 0=1 3=off")
    return p


def main(argv=None):
    args = build_parser().parse_args(argv)
    mask = state = 0
    for relay, on in args.set:  # a later assignment to the same relay wins
        mask |= 1 << relay
        state = (state & ~(1 << relay)) | (on << relay)

    import serial  # imported late so --help works without pyserial installed
    port = serial.Serial(args.port, args.baud, timeout=0.1)
    mav = ihm_dialect.MAVLink(port, srcSystem=255, srcComponent=0)
    mav.ihm_relay_command_send(mask, state)
    print(f"sent IHM_RELAY_COMMAND mask=0b{mask:08b} state=0b{state:08b}")

    start = time.time()
    while time.time() < start + 1.5:
        data = port.read(64)
        for byte in data:
            try:
                msg = mav.parse_char(bytes([byte]))
            except Exception:
                continue  # noise / partial frame; keep scanning
            if msg is not None and msg.get_type() == "IHM_RELAY_STATE" and time.time() >= start + 0.3:
                relays = msg.relays
                print(f"readback relays=0b{relays:08b} ("
                      + " ".join(f"{i}={'on' if relays >> i & 1 else 'off'}" for i in range(8)) + ")")
                applied = (relays & mask) == (state & mask)
                print("applied" if applied else "board reports a different state for the masked relays")
                return 0 if applied else 1
    print("no IHM_RELAY_STATE seen -- board may not be on this port/baud, or may not be running")
    return 1


if __name__ == "__main__":
    sys.exit(main())
