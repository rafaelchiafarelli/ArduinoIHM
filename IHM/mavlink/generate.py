#!/usr/bin/env python3
"""Regenerate mavlink/generated/ (C, for the firmware) and
mavlink/generated_py/ (Python, for bench tooling in mavlink/scripts/) from
mavlink/ihm_dialect.xml.

Requires: pip install pymavlink

Run from anywhere; paths below are relative to this file, not the cwd.
"""
import pathlib
from pymavlink.generator import mavgen

HERE = pathlib.Path(__file__).resolve().parent

# The C generator's output is a directory tree; the Python generator's output
# is a single module file, so it gets a base path (mavgen appends ".py").
(HERE / "generated_py").mkdir(exist_ok=True)
for language, out in (("C", HERE / "generated"),
                      ("Python3", HERE / "generated_py" / "ihm_dialect")):
    opts = mavgen.Opts(
        output=str(out),
        wire_protocol="2.0",
        language=language,
        strict_units=False,
    )
    mavgen.mavgen(opts, [str(HERE / "ihm_dialect.xml")])
