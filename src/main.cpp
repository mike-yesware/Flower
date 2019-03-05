// External libraries
#include <Arduino.h>
#include <FastLED.h>
#include <Homie.h>

// Modules
#include <globals.h>
#include <network_setup.h>
#include <led_utils.h>
#include <patterns.h>
#include <palettes.h>

// Real led array used for display to the strips
CRGBArray<NUM_LEDS> leds;
// Subsets for seperating the center from the petals
CRGBSet centerLeds(leds(0, NUM_LEDS_CENTER - 1));
CRGBSet petalLeds(leds(NUM_LEDS_CENTER, NUM_LEDS_PETALS - 1));
// Temporary petal array for writing data before manipulating it's layout
CRGBArray<NUM_LEDS_PETALS> tempPetalLeds;

// Global bytes for changing values that persist across loops
uint8_t hue = 0;
uint8_t gradientPosition = 0;
uint8_t ledLoc = 0;

// Global counter for tracking FPS
uint16_t fps = 0;

// Palette registration and holding variables
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;

// All patterns arrary and current pattern postion
SimplePatternList patterns = { rainbowPetals, plasma, sineMove, rainbowFlower, insideOut };
uint8_t currentPatternNumber = 0;

void setupLEDs() {
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void setup() {
  setupLEDs();
  setupNetwork();
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  currentPatternNumber = (currentPatternNumber + 1) % ARRAY_SIZE(patterns);
}

void loop() {
  fps++;

  Homie.loop();

  patterns[currentPatternNumber]();
  FastLED.show();

  EVERY_N_BSECONDS ( 5 ) { syslog.logf(LOG_DEBUG, "fps: %d", fps / 5); fps = 0; }
  EVERY_N_MILLISECONDS( 20 ) { hue++; }
  EVERY_N_SECONDS( 90 ) { nextPattern(); }
}
