#ifndef FLOWER_NETWORK_SETUP_H
#define FLOWER_NETWORK_SETUP_H

#include <Homie.h>
#include <Syslog.h>
#include <Artnet.h>

#include <globals.h>

void stopWiFiAndSleep();
void setupNetwork();
void setupNode();
void setupArtnet();

#endif  // FLOWER_NETWORK_SETUP_H