#include <led_utils.h>

void rainbowPetals() {
  tempPetalLeds[0] = CHSV(255 - hue, 255, 255);
  tempPetalLeds(0, numLedsPerPetalSetting.get() - 1).fill_rainbow(hue, 32);

  remapFirstToAll();
}