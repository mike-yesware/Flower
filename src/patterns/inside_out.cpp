#include <led_utils.h>

void insideOut() {
  EVERY_N_MILLISECONDS(75) { gradientPosition--; }
  
  for(uint8_t led = 0; led < numLedsPerPetalSetting.get() + 1; led++) {
    if(led == 0) {
      tempPetalLeds[0] = ColorFromPalette(purpleAndBlackPalette, gradientPosition);
    } else {
      tempPetalLeds[led - 1] = ColorFromPalette(purpleAndBlackPalette, gradientPosition + (5 * led) );
    }
  }

  remapFirstToAll();
}