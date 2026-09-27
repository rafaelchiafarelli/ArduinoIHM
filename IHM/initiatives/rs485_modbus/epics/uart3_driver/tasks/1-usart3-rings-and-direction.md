# Task 1: usart3-rings-and-direction

**Status:** planned -- blocked on open questions 1 (module / DE pin) and 7 (duplicate vs shared driver)
**Depends on:** nothing

## Contract

1. USART3 driver (`lib/Uart3`, or a shared driver per open question 7):
   RX ISR into a `ByteRing`, TX ring drained by the UDRE ISR,
   configurable baud and parity (8N1/8E1/8O1).
2. Direction: if DE/RE exists, raise it before the first byte and drop
   it in the **TXC** ISR (after the last stop bit, not on an empty TX
   ring); with an auto-direction module, nothing to drive.
3. RX byte timestamps (or an inter-byte gap counter) fine enough for
   Modbus timing: 1.5/3.5 characters (about 0.8/1.8 ms at 19200; the spec
   fixes 750/1750 us above 19200 baud). The ~1 ms Timer2 tick is too
   coarse at high baud, so use a timer capture or `micros()`, and record
   the choice here.
4. Own echo is ignored (on half-duplex the RX may see what TX sent).
