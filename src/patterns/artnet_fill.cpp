#include <led_utils.h>

void artnetFill() {
  artnet.read();
}

void onArtnetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP) {
  artnetFps++;

  for (uint8_t i = 0; i < length / 3; i++) {
    if (i < NUM_LEDS) {
      leds[i].setRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
}