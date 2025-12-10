#ifndef _STANDARD_DEFINITIONS_H_
#define _STANDARD_DEFINITIONS_H_

#include <Display.h> // For the display


// Define display and touch pins (these are typically fixed for shields)
#define YP A3 // MUST be an analog pin, use "An" notation!
#define XM A2 // MUST be an analog pin, use "An" notation!
#define YM 9 // Can be a digital pin
#define XP 8 // Can be a digital pin

#define TS_MINX 100 // Adjust these values based on calibration
#define TS_MINY 120
#define TS_MAXX 900
#define TS_MAXY 940
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define GRAY    0x7FFF
#define BLUE    0x001F

#define RED     0xF800
#define GREEN   0x07E0
#define DARK_GREEN   0x03F0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define DARK_ORANGE 0x7DE0
#endif // _STANDARD_DEFINITIONS_H_
