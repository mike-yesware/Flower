#include <led_utils.h>

void artnetFill() {
  artnet.read();
}

void onArtnetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {

}