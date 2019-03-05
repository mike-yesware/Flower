#include <led_utils.h>

void rainbowPetals() {
  centerLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, NUM_LEDS_PER_PETAL - 1).fill_rainbow(hue, 32);

  remapFirstToAll();
}