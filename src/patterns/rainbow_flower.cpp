#include <led_utils.h>

void rainbowFlower() {
  centerLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, NUM_LEDS_PETALS - 1).fill_rainbow(hue, 8);

  copyLeds();

}