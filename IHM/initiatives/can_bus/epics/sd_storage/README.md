# Epic: sd_storage

The SD card as storage for protocol data (J1939 database, logs, ECU
images for UDS flashing).

## Tasks

```
1-mount-and-files   CS pin, mount, open/read/append, card-absent handling, RAM cost   (deps: spi_sharing/1; blocked: Q2, Q7)
2-buffered-log      a small double buffer so a slow SD write never blocks the loop     (deps: 1, main_loop_timing/1)
```

## Acceptance gate

Bench: card inserted and absent both boot cleanly; a 1 MB file reads
back byte-identical; logging at a recorded frame rate loses nothing for
10 minutes.
