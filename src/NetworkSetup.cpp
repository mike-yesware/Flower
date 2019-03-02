#include <NetworkSetup.h>

void stopWiFiAndSleep() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void setupWiFi() {
  String apSSID = "Flower" + String(ESP.getChipId());

  WiFiManager wifiManager;
  wifiManager.autoConnect(apSSID.c_str(), "fl0werGarden");
}

void setupMQTT() {

}