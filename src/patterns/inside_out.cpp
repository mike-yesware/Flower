#include <led_utils.h>

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