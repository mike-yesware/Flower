#include <network_setup.h>

WiFiUDP udpClient;
Syslog syslog(udpClient, "192.168.7.200", 514, "Little", "flower", LOG_USER | LOG_INFO, SYSLOG_PROTO_IETF);

HomieNode flowerNode("display", "flower");

void onHomieEvent(const HomieEvent& event) {
  switch (event.type) {
    case HomieEventType::STANDALONE_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Homie: Standalone mode started"));
      break;
    case HomieEventType::CONFIGURATION_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Configuration mode started"));
      break;
    case HomieEventType::NORMAL_MODE:
      syslog.log(LOG_KERN | LOG_INFO, F("Normal mode started"));
      break;
    case HomieEventType::OTA_STARTED:
      syslog.log(LOG_KERN | LOG_INFO, F("OTA started"));
      break;
    case HomieEventType::OTA_PROGRESS:
      syslog.logf(LOG_KERN | LOG_INFO, "OTA progress, %d/%d", event.sizeDone, event.sizeTotal);
      break;
    case HomieEventType::OTA_FAILED:
      syslog.log(LOG_KERN | LOG_ERR, F("OTA failed"));
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      syslog.log(LOG_KERN | LOG_INFO, F("OTA successful"));
      break;
    case HomieEventType::ABOUT_TO_RESET:
      syslog.log(LOG_KERN | LOG_INFO, F("About to reset"));
      break;
    case HomieEventType::WIFI_CONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "Wi-Fi connected, IP: %s, gateway: %s, mask: %s", event.ip.toString().c_str(), event.gateway.toString().c_str(), event.mask.toString().c_str());
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "Wi-Fi disconnected, reason: %d", (int8_t)event.wifiReason);
      break;
    case HomieEventType::MQTT_READY:
      syslog.log(LOG_KERN | LOG_INFO, F("MQTT connected"));
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      syslog.logf(LOG_KERN | LOG_INFO, "MQTT disconnected, reason: %d", (int8_t)event.mqttReason);
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      syslog.logf(LOG_KERN | LOG_INFO, "MQTT packet acknowledged, packetId: %d", event.packetId);
      break;
    case HomieEventType::READY_TO_SLEEP:
      syslog.log(LOG_KERN | LOG_INFO, F("Ready to sleep"));
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
  Homie.setup();
}