#!/usr/bin/env python3
"""Regenerate mavlink/generated/ from mavlink/ihm_dialect.xml.

Requires: pip install pymavlink

Run from anywhere; paths below are relative to this file, not the cwd.
"""
import pathlib
from pymavlink.generator import mavgen

HERE = pathlib.Path(__file__).resolve().parent

opts = mavgen.Opts(
    output=str(HERE / "generated"),
    wire_protocol="2.0",
    language="C",
    strict_units=False,
)
mavgen.mavgen(opts, [str(HERE / "ihm_dialect.xml")])
