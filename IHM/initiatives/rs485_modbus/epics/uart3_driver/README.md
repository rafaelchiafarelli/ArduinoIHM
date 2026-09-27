# Epic: uart3_driver

Interrupt-driven USART3 for RS-485 half-duplex.

## Tasks

```
1-usart3-rings-and-direction   RX/TX rings, DE/RE or auto direction, TXC release, byte timestamps   (blocked: Q1, Q7)
```

## Acceptance gate

Build + native suite (ring tests); bench: loopback through the RS-485
module at 9600 and 115200 baud, with no lost bytes and the DE line
released within one character time after the last stop bit (scope).
