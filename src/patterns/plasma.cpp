#include <led_utils.h>

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