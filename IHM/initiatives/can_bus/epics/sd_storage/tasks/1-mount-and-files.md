# Task 1: mount-and-files

**Status:** planned -- blocked on can_bus open questions 2 (CS pin) and 7 (RAM)
**Depends on:** `spi_sharing/1`

## Contract

1. Use `lib/SD` (or a lighter FAT library if the RAM cost is too high;
   compare and record) with the confirmed CS pin through `spi_sharing`.
2. Mount at boot with a bounded timeout; card absent -> a status flag,
   never a hang.
3. Read, append and seek on files; record RAM and flash cost here.
