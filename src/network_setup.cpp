#include <network_setup.h>

WiFiUDP udpClient;
Syslog syslog(udpClient, "192.168.7.200", 514, "Little", "flower", LOG_KERN | LOG_INFO, SYSLOG_PROTO_BSD);

HomieNode flowerNode("display", "sketch");

void stopWiFiAndSleep() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setupNetwork() {
  Homie_setFirmware("flower", "0.0.1");
  Homie_setBrand("garden");
  Homie.setup();
}