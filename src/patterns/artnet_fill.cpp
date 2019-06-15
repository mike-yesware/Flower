#include <led_utils.h>

void artnetFill() {
  artnet.read();
}

void onArtnetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  // read universe and put into the right part of the display buffer
  // syslog.logf(
  //   LOG_DEBUG,
  //   "universe=%d length=%d sequence=%d",
  //   universe,
  //   length,
  //   sequence
  // );

  for (uint8_t i = 0; i < length / 3; i++) {
    if (i < NUM_LEDS) {
      // syslog.logf(
      //   LOG_DEBUG,
      //   "R=%d G=%d B=%d",
      //   data[i * 3],
      //   data[i * 3 + 1],
      //   data[i * 3 + 2]
      // );

      leds[i].setRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
}