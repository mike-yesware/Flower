#include <led_utils.h>

// Calculate the number of petal LEDs based on current settings
uint8_t numLedsPetals() {
  return numPetalsSetting.get() * numLedsPerPetalSetting.get();
}

// Calculate the number of total LEDs based on current settings
uint8_t numLeds() {
  return numLedsCenterSetting.get() + numLedsPetals();
}

// Copy data written to tempPetalLeds out to each petal LEDs exactly as they are
void copyLeds() {
  leds(numLedsCenterSetting.get(), numLedsCenterSetting.get() + numLedsPetals() - 1) = tempPetalLeds(0, numLedsPetals() - 1);
}

// Copy the first petal's temp data to all petals display data
void remapFirstToAll() {
  for(uint8_t petal = 0; petal < numPetalsSetting.get(); petal++) {
    leds(numLedsCenterSetting.get() + (petal * numLedsPerPetalSetting.get()),
        (petal * numLedsPerPetalSetting.get()) + numLedsPerPetalSetting.get() + numLedsCenterSetting.get() - 1) = tempPetalLeds(0, numLedsPerPetalSetting.get() - 1);
  }
}

// Given a zero indexed petal number, fill with the supplied color
void fillPetal(uint8_t petal, CRGB color) {
  leds(numLedsCenterSetting.get() + (petal * numLedsPerPetalSetting.get()), (petal * numLedsPerPetalSetting.get()) + numLedsPerPetalSetting.get() + numLedsCenterSetting.get() - 1) = color;
}

// Copy data written to tempPetalLeds out to each petal in full.
// This makes an pixel identical bar of color for each tempPetalLed written to
// 0 == first petal == 0, 1, 2 or (0 * 3) to (0 * 3) + 3
// 1 == second petal == 4, 5, 6 or (1 * 3) + 1 to (1 * 3) + 3
// ...
// 5 == sixth petal == 16, 17, 18 or (5 * 3) + 1 to (5 * 3) + 3
void remapSolid() {
  for(uint8_t petal = 0; petal < numPetalsSetting.get(); petal++) {
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
  for(uint8_t virtualPetal = 0; virtualPetal < numPetalsSetting.get() * 2; virtualPetal++) {
    switch(virtualPetal % 2)
    {
      case false:  // even
        leds[((virtualPetal / 2) * 3) + numLedsCenterSetting.get() + 1] = tempPetalLeds[virtualPetal];
        
        break;
      case true:  // odd
        leds[((virtualPetal / 2) * 3) + numLedsCenterSetting.get()] = tempPetalLeds[virtualPetal];
        leds[((virtualPetal / 2) * 3) + numLedsCenterSetting.get() + 2] = tempPetalLeds[virtualPetal];

        break;
    }
  }
}

// Flip data on each petal such that the outside LED is now the inside LED and vice versa.
void remapDirection() {
  for(uint8_t petal = 0; petal < numPetalsSetting.get(); petal++) {
    leds((petal * numLedsPerPetalSetting.get()) + numLedsCenterSetting.get(), (petal * numLedsPerPetalSetting.get()) + numLedsPerPetalSetting.get() + numLedsCenterSetting.get() - 1) = tempPetalLeds(
        (petal * numLedsPerPetalSetting.get()) + numLedsPerPetalSetting.get() - 1, (petal * numLedsPerPetalSetting.get()));
  }
}

// Copy data written to tempPetalLeds out to each petal LED in a clockwise, outside in order.
// Starting at the outside means we begin with the last led and work backwards.
void remapClockwise() {
  uint8_t interation = 0;

  for(int8_t led = numLedsPerPetalSetting.get() - 1; led  < 0; led--) {
    for(uint8_t petal = 0; petal < numPetalsSetting.get(); petal++) {
      leds[led + (petal * numLedsPerPetalSetting.get()) + numLedsCenterSetting.get()] = tempPetalLeds[interation];

      interation++;
    }
  }
}