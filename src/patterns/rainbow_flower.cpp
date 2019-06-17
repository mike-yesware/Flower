#include <led_utils.h>

void rainbowFlower() {
  tempPetalLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, numLedsPetals() - 1).fill_rainbow(hue, 8);

  copyLeds();
}