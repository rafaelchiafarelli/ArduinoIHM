#!/usr/bin/env python3
"""Send one IHM_DAC_COMMAND (id 315) to the IHM board over its Serial2 port.

Requires: pip install pymavlink pyserial
(the dialect binding lives in mavlink/generated_py/, made by mavlink/generate.py)

Examples
  # DAC0 to mid-scale
  dac_cmd.py --port COMx --channel 0 --value 2048

  # DAC1 to full scale
  dac_cmd.py --port COMx --channel 1 --value 4095

There is no ack message: after sending, the script waits for the board's
periodic IHM_DAC_STATE (316) and prints it, including whether the DAC ACKed
on I2C. The first 316 after ~300 ms is used, since one sent before the board
applied the command can still be in flight. Close any other program (e.g. the
companion app) holding the port first.
"""
import argparse
import pathlib
import sys
import time

HERE = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "generated_py"))
import ihm_dialect  # noqa: E402  (path set up just above)


def build_parser():
    p = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--port", required=True, help="the Serial2 USB-TTL adapter's port, e.g. COMx or /dev/ttyUSB0")
    p.add_argument("--baud", type=int, default=250000)
    p.add_argument("--channel", type=int, choices=(0, 1), required=True, help="DAC0 (0x62) or DAC1 (0x63)")
    p.add_argument("--value", type=int, required=True, help="12-bit DAC code, 0-4095")
    return p


def main(argv=None):
    args = build_parser().parse_args(argv)
    if not 0 <= args.value <= 4095:
        print(f"value must be 0..4095, got {args.value}")
        return 2

    import serial  # imported late so --help works without pyserial installed
    port = serial.Serial(args.port, args.baud, timeout=0.1)
    mav = ihm_dialect.MAVLink(port, srcSystem=255, srcComponent=0)
    mav.ihm_dac_command_send(args.channel, args.value)
    print(f"sent IHM_DAC_COMMAND channel={args.channel} value={args.value}")

    start = time.time()
    while time.time() < start + 1.5:
        data = port.read(64)
        for byte in data:
            try:
                msg = mav.parse_char(bytes([byte]))
            except Exception:
                continue  # noise / partial frame; keep scanning
            if msg is not None and msg.get_type() == "IHM_DAC_STATE" and time.time() >= start + 0.3:
                for i in range(2):
                    present = "present" if msg.present >> i & 1 else "NOT answering"
                    print(f"readback DAC{i}: code={msg.value[i]} ({present})")
                ok = msg.value[args.channel] == args.value and msg.present >> args.channel & 1
                print("applied" if ok else "not applied -- see the DAC's line above")
                return 0 if ok else 1
    print("no IHM_DAC_STATE seen -- board may not be on this port/baud, or may not be running")
    return 1


if __name__ == "__main__":
    sys.exit(main())
