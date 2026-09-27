# Task 2: slave-engine

**Status:** planned -- open question 2 answered (event-driven); ISR budget to confirm
**Depends on:** task 1

## Contract

1. A slave engine invoked by the end-of-frame event (the silence-timer
   expiry ISR, interrupts re-enabled): complete request -> register-map
   callbacks (read bits/registers from snapshots, queue writes) ->
   response handed to the UART's TX path. Work bounded per open question
   2 (proposal ~200 us).
2. Broadcast writes are applied without a reply.
3. Counters: CRC errors, exceptions sent, requests served (for
   diagnostics and a future FC 08).
