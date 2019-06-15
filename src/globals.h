#ifndef FLOWER_GLOBALS_H
#define FLOWER_GLOBALS_H

#include <Homie.h>
#include <FastLED.h>
#include <Syslog.h>
#include <ESPAsyncE131.h>
#include <ArtnetWifi.h>

extern WiFiUDP udpClient;
extern Syslog syslog;
extern HomieNode flowerNode;
extern ESPAsyncE131 e131;
extern ArtnetWifi artnet;

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)      // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)    // Analog Unsigned subtraction macro. if result <0, then => 0

// Array sizing helpers
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Magic numbers
#define DATA_PIN 13
#define CLOCK_PIN 14
#define NUM_PETALS 6
#define NUM_LEDS_PER_PETAL 5
#define NUM_LEDS_CENTER 1
#define NUM_LEDS_PETALS (NUM_PETALS * NUM_LEDS_PER_PETAL)
#define NUM_LEDS NUM_LEDS_PETALS + NUM_LEDS_CENTER

// Real led array used for display to the strips
extern CRGBArray<NUM_LEDS> leds;
// Subsets for seperating the center from the petals
extern CRGBSet centerLeds;
extern CRGBSet petalLeds;
// Temporary petal array for writing data before manipulating it's layout
extern CRGBArray<NUM_LEDS_PETALS> tempPetalLeds;

// Global bytes for changing values that persist across loops
extern uint8_t hue;
extern uint8_t gradientPosition;
extern uint8_t ledLoc;

// Global counter for tracking FPS
extern uint16_t fps;

// Palette registration and holding variables
extern const TProgmemPalette16 purpleAndBlackPalette PROGMEM;
extern CRGBPalette16 currentPalette;
extern CRGBPalette16 targetPalette;

// All patterns arrary and current pattern postion
typedef void (*SimplePatternList[])();
extern SimplePatternList patterns;
extern uint8_t currentPatternNumber;
extern const TProgmemPalette16 purpleAndBlackPalette PROGMEM;

#endif  // FLOWER_GLOBALS_H