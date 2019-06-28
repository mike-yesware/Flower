#include <network_setup.h>
#include <patterns.h>

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);

HomieNode flowerNode("display", "flower");
HomieSetting<long> numPetalsSetting("numPetals", "The number of petals on this flower");
HomieSetting<long> numLedsPerPetalSetting("numLedsPerPetal", "The number of LEDs in each petal");
HomieSetting<long> numLedsCenterSetting("numLedsCenter", "The number of LEDs in this flower's center");

Artnet artnet;
byte broadcast[4] = {192, 168, 7, 255};

void stopWiFiAndSleep() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setupNetwork() {
  Homie_setFirmware("flower", "1.0.0");
  Homie_setBrand("garden");
  Homie.disableLogging();

  numPetalsSetting.setDefaultValue(NUM_PETALS).setValidator([] (long candidate) {
    return candidate >= 1;
  });
  numLedsPerPetalSetting.setDefaultValue(NUM_LEDS_PER_PETAL).setValidator([] (long candidate) {
    return candidate >= 1;
  });
  numLedsCenterSetting.setDefaultValue(NUM_LEDS_CENTER).setValidator([] (long candidate) {
    return candidate >= 0;
  });

  Homie.setup();

  syslog.server(Homie.getConfiguration().mqtt.server.host, 514);
  syslog.deviceHostname(Homie.getConfiguration().deviceId);
  syslog.appName("flower");
  syslog.defaultPriority(LOG_USER | LOG_INFO);
}

void setupArtnet() {
  artnet.begin();
  artnet.setBroadcast(broadcast);
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
