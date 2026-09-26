# Task 3: link-fixes-and-buttons

**Status:** done
**Branch:** none in this repo -- the companion
(`C:\Users\rafae\source\repos\IHMPCController`) is not under git, so this
file is the record. `.bak` copies of every edited file sit next to it.
**Depends on:** tasks 1-2, `input_simulation/2`

## Why

Bench 2026-09-25: the companion showed live telemetry, but none of its
commands (encoder turns, PWM) reached the board. The same frames sent from
pymavlink worked, and the bytes were identical (checked by compiling the
companion's headers).

## Delivered

1. **`SerialPort.h`:** DCB now pins `fOutX`/`fInX` off (plus
   `fTXContinueOnXoff`, `fDsrSensitivity`, `fErrorChar`, `fNull`,
   `fAbortOnError`). `GetCommState` had returned the driver's XON/XOFF
   setting, and a 0x13 byte in the binary telemetry froze the transmitter.
   **This was the fix.**
2. **`SerialPort.h`:** the handle is opened with `FILE_FLAG_OVERLAPPED`, with
   one event per direction. On a synchronous handle the listener's pending
   `ReadFile` serializes the UI thread's `WriteFile`.
3. **`IHMPCController.cpp`:** an encoder/button send failure shows in the
   status line (it used to be ignored).
4. **Button simulation:** `SendSimulateButton` / `ButtonBit` /
   `EncoderButtonBit` in `MavlinkLink.h`. B0-B3 buttons next to the input
   LEDs, and a "Push" button per encoder row. Encoder rows are renamed
   RE0-RE2.
5. **Dialect:** `mavlink/ihm_dialect/` re-vendored from
   `IHM/mavlink/generated/ihm_dialect/` (adds 306; 300-305 CRCs unchanged).
   `mavlink_msg_ihm_simulate_button.h` is added to the `.vcxproj` and
   `.filters`.
6. **`HOW_TO_USE.md`:** Serial2 adapter wiring (COM3 on the bench PC),
   B0-B3/Push, id 306, troubleshooting rows.

## Bench

Driven programmatically (BM_CLICK) on COM3: encoder turns switch tabs, and
RE1 Push toggled Relay 0 on the Output tab (seen on the companion's relay
LEDs). Builds in VS 2022 x64 Debug.
