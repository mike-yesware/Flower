#include <led_utils.h>

void sineMove() {
  EVERY_N_MILLIS_I(thisTimer, 100) {                                   // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval = beatsin8(10,25,100);                             // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                                     // Use that as our update timer value.
    ledLoc = (ledLoc + 1) % (NUM_LEDS_PETALS);                              // A simple routine to just move the active LED UP the strip.
    tempPetalLeds[ledLoc + 1] = ColorFromPalette(PartyColors_p, ledLoc * 14);  // Pick a slightly rotating colour from the Palette
  }

  fadeToBlackBy(tempPetalLeds, NUM_LEDS_PETALS, 1);                                   // Leave a nice comet trail behind.
  copyLeds();
}