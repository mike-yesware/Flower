#include <network_setup.h>
#include <patterns.h>

WiFiUDP udpClient;
Syslog syslog(udpClient, "192.168.7.200", 514, "Swirl", "flower", LOG_USER | LOG_INFO, SYSLOG_PROTO_IETF);

HomieNode flowerNode("display", "flower");

// ESPAsyncE131 e131(1);
ArtnetWifi artnet;

void onHomieEvent(const HomieEvent& event) {
  switch (event.type) {
    case HomieEventType::STANDALONE_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: Standalone mode started"));
      break;
    case HomieEventType::CONFIGURATION_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: Configuration mode started"));
      break;
    case HomieEventType::NORMAL_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: Normal mode started"));
      break;
    case HomieEventType::OTA_STARTED:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: OTA started"));
      break;
    case HomieEventType::OTA_PROGRESS:
      syslog.logf(LOG_KERN | LOG_INFO, "Homie: OTA progress, %d/%d", event.sizeDone, event.sizeTotal);
      break;
    case HomieEventType::OTA_FAILED:
      syslog.log(LOG_KERN | LOG_ERR, F("Homie: OTA failed"));
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: OTA successful"));
      break;
    case HomieEventType::ABOUT_TO_RESET:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: About to reset"));
      break;
    case HomieEventType::WIFI_CONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "Homie: Wi-Fi connected, IP: %s, gateway: %s, mask: %s", event.ip.toString().c_str(), event.gateway.toString().c_str(), event.mask.toString().c_str());
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "Homie: Wi-Fi disconnected, reason: %d", (int8_t)event.wifiReason);
      break;
    case HomieEventType::MQTT_READY:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: MQTT connected"));
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "Homie: MQTT disconnected, reason: %d", (int8_t)event.mqttReason);
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      syslog.logf(LOG_KERN | LOG_INFO, "Homie: MQTT packet acknowledged, packetId: %d", event.packetId);
      break;
    case HomieEventType::READY_TO_SLEEP:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: Ready to sleep"));
      break;
  }
}

void stopWiFiAndSleep() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setupNetwork() {
  Homie_setFirmware("flower", "0.0.1");
  Homie_setBrand("garden");
  Homie.disableLogging();
  Homie.onEvent(onHomieEvent);
  Homie.setup();
}

void setupE131() {
  if (e131.begin(E131_UNICAST)) {
    syslog.log(LOG_KERN | LOG_INFO, F("E131: Connected"));
  } else {
    syslog.log(LOG_KERN | LOG_INFO, F("E131: Connection failed"));
  }
}

void setupArtnet() {
  artnet.setArtDmxCallback(onArtnetFrame);
}

uint8_t stringToInt8(const String& string) {
  for (byte i = 0; i < string.length(); i++) {
    if (isDigit(string.charAt(i)) == false) return 0;
  }
  return string.toInt();
}

bool onChangeBrightness(const HomieRange& range, const String& value) {
  syslog.log(LOG_DEBUG, F("Starting onChangeBrightness"));
  uint8_t numericValue = stringToInt8(value);
  syslog.logf(LOG_DEBUG, "Setting brightness=%u", numericValue);

  FastLED.setBrightness(numericValue);

  flowerNode.setProperty("brightness").send(String(numericValue));
}

bool onChangeHue(const HomieRange& range, const String& value) {
  syslog.log(LOG_DEBUG, F("Starting onChangeHue"));
  uint8_t numericValue = stringToInt8(value);
  syslog.logf(LOG_DEBUG, "Setting hue=%u", numericValue);

  hue = numericValue;

  flowerNode.setProperty("hue").send(String(numericValue));
}

bool onChangeMode(const HomieRange& range, const String& value) {
  syslog.log(LOG_DEBUG, F("Starting onChangeMode"));
  syslog.log(LOG_DEBUG, "Setting mode=" + value);

  // flowerNode.setProperty("mode").send(String(numericValue));
}

bool onChangePattern(const HomieRange& range, const String& value) {
  syslog.log(LOG_DEBUG, F("Starting onChangePattern"));
  uint8_t numericValue = stringToInt8(value);
  syslog.logf(LOG_DEBUG, "Setting pattern=%u", numericValue);

  currentPatternNumber = numericValue;

  flowerNode.setProperty("pattern").send(String(numericValue));
}

void setupNode() {
  flowerNode.advertise("brightness").settable(onChangeBrightness);
  flowerNode.advertise("hue").settable(onChangeHue);
  flowerNode.advertise("mode").settable(onChangeMode);
  flowerNode.advertise("pattern").settable(onChangePattern);
}
