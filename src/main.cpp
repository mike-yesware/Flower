#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)      // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)    // Analog Unsigned subtraction macro. if result <0, then => 0

// Array sizing helper
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

// Palette registration and holding variables
extern const TProgmemPalette16 purpleAndBlackPalette PROGMEM;
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;

// Prototypes for all the display patterns
void rainbowPetals();
void rainbowFlower();
void insideOut();
void sineMove();
void plasma();

// All patterns arrary and current pattern postion
typedef void (*SimplePatternList[])();
SimplePatternList patterns = { rainbowPetals, plasma, sineMove, rainbowFlower, insideOut };
uint8_t currentPatternNumber = 0;

void stopWiFiAndSleep() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setup() {
  stopWiFiAndSleep();

  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  currentPatternNumber = (currentPatternNumber + 1) % ARRAY_SIZE(patterns);
}

void loop() {
  patterns[currentPatternNumber]();
  FastLED.show();

  EVERY_N_MILLISECONDS( 20 ) { hue++; }
  EVERY_N_SECONDS( 90 ) { nextPattern(); }
}

// Copy data written to tempPetalLeds out to each petal LEDs exactly as they are
void copyLeds() {
  petalLeds(0, NUM_LEDS_PETALS) = tempPetalLeds(0, NUM_LEDS_PETALS);
}

// Copy the first petal's temp data to all petals display data
void remapFirstToAll() {
  for(uint8_t petal = 0; petal < NUM_PETALS; petal++) {
    petalLeds(petal * NUM_LEDS_PER_PETAL,
        (petal * NUM_LEDS_PER_PETAL) + NUM_LEDS_PER_PETAL - 1) = tempPetalLeds(0, NUM_LEDS_PER_PETAL - 1);
  }
}

// Given a zero indexed petal number, fill with the supplied color
void fillPetal(uint8_t petal, CRGB color) {
  petalLeds(petal * NUM_LEDS_PER_PETAL, (petal * NUM_LEDS_PER_PETAL) + NUM_LEDS_PER_PETAL - 1) = color;
}

// Copy data written to tempPetalLeds out to each petal in full.
// This makes an pixel identical bar of color for each tempPetalLed written to
// 0 == first petal == 0, 1, 2 or (0 * 3) to (0 * 3) + 3
// 1 == second petal == 4, 5, 6 or (1 * 3) + 1 to (1 * 3) + 3
// ...
// 5 == sixth petal == 16, 17, 18 or (5 * 3) + 1 to (5 * 3) + 3
void remapSolid() {
  for(uint8_t petal = 0; petal < NUM_PETALS; petal++) {
    fillPetal(petal, tempPetalLeds[petal]);
  }
}

// Copy data written to tempPetalLeds out to each petal LEDs in
// an alternating inside/outside pattern. This allows the inside pixel to fill before
// the two outside pixels.
// TODO: option for outside then inside ordering?
// 0 == first petal inside == 1 or ((0 / 2) * 3) + 1
// 1 == first petal outside == 0 and 2 or ((1 / 2) * 3) and ((1 / 2) * 3) + 2
// ...
// 10 == sixth petal inside == 16 or ((10 / 2) * 3) + 1
// 11 == sixth petal outside == 15 and 17 or ((11 / 2) * 3) and ((11 / 2) * 3) + 2
void remap12() {
  for(uint8_t virtualPetal = 0; virtualPetal < NUM_PETALS * 2; virtualPetal++) {
    switch(virtualPetal % 2)
    {
      case false:  // even
        petalLeds[((virtualPetal / 2) * 3) + 1] = tempPetalLeds[virtualPetal];
        
        break;
      case true:  // odd
        petalLeds[((virtualPetal / 2) * 3)] = tempPetalLeds[virtualPetal];
        petalLeds[((virtualPetal / 2) * 3) + 2] = tempPetalLeds[virtualPetal];

        break;
    }
  }
}

// Flip data on each petal such that the outside LED is now the inside LED and vice versa.
void remapDirection() {
  for(uint8_t petal = 0; petal < NUM_PETALS; petal++) {
    petalLeds((petal * NUM_LEDS_PER_PETAL), (petal * NUM_LEDS_PER_PETAL) + NUM_LEDS_PER_PETAL - 1) = tempPetalLeds(
        (petal * NUM_LEDS_PER_PETAL) + NUM_LEDS_PER_PETAL - 1, (petal * NUM_LEDS_PER_PETAL));
  }
}

// Copy data written to tempPetalLeds out to each petal LED in a clockwise, outside in order.
// Starting at the outside means we begin with the last led and work backwards.
void remapClockwise() {
  uint8_t interation = 0;

  for(int8_t led = NUM_LEDS_PER_PETAL - 1; led  < 0; led--) {
    for(uint8_t petal = 0; petal < NUM_PETALS; petal++) {
      petalLeds[led + (petal * NUM_LEDS_PER_PETAL)] = tempPetalLeds[interation];

      interation++;
    }
  }
}

void rainbowPetals() {
  centerLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, NUM_LEDS_PER_PETAL - 1).fill_rainbow(hue, 32);

  remapFirstToAll();
}

void rainbowFlower() {
  centerLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, NUM_LEDS_PETALS - 1).fill_rainbow(hue, 8);

  copyLeds();
}

void insideOut() {
  EVERY_N_MILLISECONDS(75) { gradientPosition--; }
  
  for(uint8_t led = 0; led < NUM_LEDS_PER_PETAL + 1; led++) {
    if(led == 0) {
      centerLeds[0] = ColorFromPalette(purpleAndBlackPalette, gradientPosition);
    } else {
      tempPetalLeds[led - 1] = ColorFromPalette(purpleAndBlackPalette, gradientPosition + (5 * led) );
    }
  }

  remapFirstToAll();
}

void sineMove() {
  EVERY_N_MILLIS_I(thisTimer, 100) {                                   // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,25,100);                             // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                                     // Use that as our update timer value.
    ledLoc = (ledLoc + 1) % (NUM_LEDS_PETALS);                              // A simple routine to just move the active LED UP the strip.
    tempPetalLeds[ledLoc + 1] = ColorFromPalette(PartyColors_p, ledLoc * 14);  // Pick a slightly rotating colour from the Palette
  }

  fadeToBlackBy(tempPetalLeds, NUM_LEDS_PETALS, 1);                                   // Leave a nice comet trail behind.
  copyLeds();
}

void plasma() {
  EVERY_N_MILLISECONDS(50) {
    uint8_t thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
    uint8_t thatPhase = beatsin8(7,-64,64);

    for(uint8_t i = 0; i < NUM_LEDS_PETALS; i++) {
      uint8_t colorIndex = cubicwave8((i*23)+thisPhase)/2 + cos8((i*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
      uint8_t thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsuba gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

      tempPetalLeds[i] = ColorFromPalette(currentPalette, colorIndex, thisBright);  // Let's now add the foreground colour.
    }

    copyLeds();
  }

  EVERY_N_MILLISECONDS(50) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(3) {
    uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
  }
}

const TProgmemPalette16 purpleAndBlackPalette PROGMEM =
{
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Purple
};