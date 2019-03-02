#ifndef FLOWER_NETWORK_SETUP_H
#define FLOWER_NETWORK_SETUP_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <AsyncMqttClient.h>

void stopWiFiAndSleep();
void setupWiFi();
void setupMQTT();

#endif  // FLOWER_NETWORK_SETUP_H