# Task 1: usart3-rings-and-direction

**Status:** planned -- blocked on open questions 1 (module / DE pin) and 7 (duplicate vs shared driver)
**Depends on:** `can_bus`'s `event_timer/1` (the silence timer)

## Contract

1. USART3 driver (`lib/Uart3`, or a shared driver per open question 7),
   strictly event-driven: the RX ISR appends to the frame buffer and
   re-arms the silence timer; the UDRE ISR sends the TX buffer.
   Configurable baud and parity (8N1/8E1/8O1). Nothing polls a ring.
2. Direction: if DE/RE exists, raise it before the first byte and drop
   it in the **TXC** ISR (after the last stop bit, not on an empty TX
   ring); with an auto-direction module, nothing to drive.
3. Frame timing from `event_timer`: each RX byte re-arms a one-shot
   3.5-character timer (about 1.8 ms at 19200; the spec fixes 1750 us
   above 19200 baud), and its expiry is the end of frame. The 1.5-character
   inter-byte rule (a gap > 1.5 char inside a frame = error) is checked
   with the same timer (a second slot), with no timestamps scanned later.
4. Own echo is ignored (on half-duplex the RX may see what TX sent).
