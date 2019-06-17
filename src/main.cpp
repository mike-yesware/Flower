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
CRGBArray<100> leds;
// Temporary petal array for writing data before manipulating its layout
CRGBArray<100> tempPetalLeds;

// Global bytes for changing values that persist across loops
uint8_t hue = 0;
uint8_t gradientPosition = 0;
uint8_t ledLoc = 0;

// Global to control the desplay of patterns
bool runLeds = false;

// Global counter for tracking FPS values
uint16_t fps = 0;
uint16_t artnetFps = 0;

// Palette registration and holding variables
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;

// All patterns arrary and current pattern postion
SimplePatternList patterns = { rainbowPetals, artnetFill, plasma, sineMove, rainbowFlower, insideOut, colorFill };
uint8_t currentPatternNumber = 0;

void setupLEDs() {
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, 100);
  FastLED.setBrightness(16);
}

void setup() {
  setupNetwork();
  setupNode();
  setupLEDs();
  setupArtnet();
  runLeds = Homie.isConfigured();
}

void timedLog(){
  syslog.logf(
    LOG_INFO,
    "pattern=%d fps=%d artnet_fps=%d center_leds=%ld petals=%ld leds_per_petal=%ld free_heap=%d max_free_block=%d heap_frag=%d",
    currentPatternNumber,
    fps / 5,
    artnetFps / 5,
    numLedsCenterSetting.get(),
    numPetalsSetting.get(),
    numLedsPerPetalSetting.get(),
    ESP.getFreeHeap(),
    ESP.getMaxFreeBlockSize(),
    ESP.getHeapFragmentation()
  );
  
  fps = 0;
  artnetFps = 0;
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  currentPatternNumber = (currentPatternNumber + 1) % ARRAY_SIZE(patterns);
}

void loop() {
  fps++;

  Homie.loop();

  if(runLeds) {
    patterns[currentPatternNumber]();
    FastLED.show();
  } 

  EVERY_N_SECONDS ( 5 ) { timedLog(); }
  EVERY_N_MILLISECONDS( 20 ) { hue++; }
  // EVERY_N_SECONDS( 90 ) { nextPattern(); }
}
