# Task 5: pwm-sliders

**Status:** done 2026-09-27. Builds x64 + x86 Debug and is installed in
x64\Debug. Bench, with the real UI driven by script on COM3 (channel 0,
VARIABLE, A enabled at 30 %): slider positions 0/250/500/750/1000 gave TOP
65535/4870/361/26/1 (244.14 Hz / 3.285 kHz / 44.199 kHz / 592.593 kHz /
8 MHz), each "Board applied" with a matching readback. Sweeping to 8 MHz and
back kept the slider at 30 % (the readout showed 0 % -> 50.0 % at TOP 1).
Fixed 976 Hz moved the disabled slider to match. CH0 was left disabled
afterwards. Rafael's on-screen label check is still to do.
**Depends on:** nothing. Companion only: no firmware and no dialect change.

Reported by Rafael 2026-09-27: VARIABLE frequency "shows a strange
frequency". The firmware is correct: f = 16 MHz / (TOP + 1), so TOP 1000
gives 15 984 Hz and TOP 160 gives 99 378 Hz. The companion asks for a raw
timer count ("Raw TOP (ICRn)") where a person thinks in Hz.

## Decisions (Rafael, 2026-09-27)

- The VARIABLE range is **244 Hz - 8 MHz**, the full hardware range: TOP
  65535 down to 1.
- The frequency slider is **logarithmic**.
- The sliders only set values; **nothing is sent until Send**, as today.
- The frequency slider is **enabled only when VARIABLE is selected** (as
  Raw TOP is today). With a fixed frequency selected it is disabled and
  shows that frequency. The fixed 122/61/30/15 Hz are below the slider's
  244 Hz, so the slider sits at its left end while the text shows the exact
  value.
- **Duty becomes a slider per output**, from the lowest to the highest duty
  the board can actually produce at the selected frequency. It changes
  whenever the frequency does.

## Contract

### Delivers (`IHMPCController.cpp`, `HOW_TO_USE.md`; `.bak` backups)

1. **Frequency slider** (trackbar, positions 0-1000, log scale) replaces
   the Raw TOP edit. For position p: f = 244.14 x (8e6 / 244.14)^(p/1000),
   TOP = round(16e6 / f) - 1, clamped to 1..65535. The readout next to it
   shows the frequency the board will really produce, 16e6 / (TOP + 1),
   and the TOP. Send puts that TOP in `PWM_CHANNEL_CONFIG.frequency`,
   exactly as the edit did.
2. **Duty sliders** (one per output A/B/C, 0-100 %) replace the Duty %
   edits. The board maps percent to raw as raw = pct x TOP / 100 (rounded
   down, `scaleDutyCycleToRaw`), with TOP = 255/511/1023 for the fixed
   selectors and the chosen TOP for VARIABLE. A slider move snaps to the
   smallest percent that gives its raw value, so each position is a
   distinct output. The readout shows the percent sent and the resulting
   duty: (raw + 1) / (TOP + 1) in Fast PWM non-inverting (datasheet: raw 0
   still gives a one-count spike), and 100 % at raw = TOP.
3. **Frequency changes refresh the duty readouts** (combo selection or
   frequency slider), so they always show what the board would produce. The
   duty sliders themselves stay put: the first build re-snapped them, which
   lost the user's duty for good on a sweep to 8 MHz (30 % -> 0 %). Snapping
   happens only when the user moves a duty slider, and at Send.
   Also as built: the Outputs lines and the Send/readback result show
   VARIABLE in Hz, not TOP, and the combo entry is renamed
   "VARIABLE (slider)". Trackbars get a window-coloured background (they
   painted black without a brush in `WM_CTLCOLORSTATIC`).
4. The rows and the TOP check in `OnPwmSendClicked` are adapted (the
   slider can't produce an invalid TOP). The readback comparison is
   unchanged, because it still compares TOP.
5. **`HOW_TO_USE.md`**: the PWM panel section describes the sliders, the
   244 Hz - 8 MHz range, and why duty is coarse at high frequencies (for
   example, at 8 MHz only 50 % or 100 %).

## Definition of done

Builds x64 + x86 Debug. Bench (COM3): VARIABLE at a few slider positions
reads back the TOP the readout shows, and the on-screen label agrees with
the readout's Hz. The task file is marked done in the same commit.
