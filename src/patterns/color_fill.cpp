#include <led_utils.h>

void colorFill() {
  centerLeds[0] = CRGB(255, 255, 255);
  tempPetalLeds(0, NUM_LEDS_PETALS - 1).fill_solid(CRGB(255, 255, 255));

  copyLeds();
}