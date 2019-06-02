#ifndef FLOWER_NETWORK_SETUP_H
#define FLOWER_NETWORK_SETUP_H

#include <Homie.h>
#include <Syslog.h>

#include <globals.h>

void stopWiFiAndSleep();
void setupNetwork();
void setupNode();

#endif  // FLOWER_NETWORK_SETUP_H