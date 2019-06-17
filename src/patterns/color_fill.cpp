#include <led_utils.h>

void colorFill() {
  tempPetalLeds(0, numLeds() - 1).fill_solid(CRGB(255, 255, 255));

  copyLeds();
}