#include <led_utils.h>

// Copy data written to tempPetalLeds out to each petal LEDs exactly as they are
void copyLeds() {
  petalLeds(0, NUM_LEDS_PETALS - 1) = tempPetalLeds(0, NUM_LEDS_PETALS - 1);
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