# Epic: spi_sharing

CAN-1, CAN-2 and the SD card share the SPI bus (D50-D53). Today nothing
uses SPI. Once the CAN RX runs in an INT ISR, every other SPI user must
not be interrupted mid-transaction.

## Tasks

```
1-spi-transactions   one SPI wrapper: transactions, per-device CS, ISR-safe access   (blocked: Q3)
```

## Acceptance gate

Build + native suite; bench: CS lines idle high, and an SD access and a
CAN read never overlap (scope or logic analyser on CS lines).
