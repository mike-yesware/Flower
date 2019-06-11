#include <led_utils.h>

#define CHANNEL_START 1 /* Channel to start listening at */

void e131Fill() {
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer
    
    for (int i = 0; i < NUM_LEDS; i++) {
      int j = i * 3 + (CHANNEL_START - 1);
      tempPetalLeds[i].setRGB(packet.property_values[j], packet.property_values[j+1], packet.property_values[j+2]);

      // syslog.logf(
      //   LOG_DEBUG,
      //   "universe=%d i=%d j=%d R=%d G=%d B=%d",
      //   htons(packet.universe),
      //   i,
      //   j,
      //   packet.property_values[j],
      //   packet.property_values[j+1],
      //   packet.property_values[j+2]
      // );
    }
    
    copyLeds();
  }
}

  // tempPetalLeds(0, NUM_LEDS_PETALS - 1).fill_solid(CRGB(255, 255, 255));

  // copyLeds();