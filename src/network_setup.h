#ifndef FLOWER_NETWORK_SETUP_H
#define FLOWER_NETWORK_SETUP_H

#include <Homie.h>
#include <Syslog.h>
#include <ESPAsyncE131.h>
#include <ArtnetWifi.h>

#include <globals.h>

void stopWiFiAndSleep();
void setupNetwork();
void setupNode();
void setupE131();
void setupArtnet();

#endif  // FLOWER_NETWORK_SETUP_H