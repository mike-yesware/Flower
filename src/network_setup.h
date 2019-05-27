#ifndef FLOWER_NETWORK_SETUP_H
#define FLOWER_NETWORK_SETUP_H

#include <Homie.h>
#include <Syslog.h>

#include <globals.h>

void onHomieEvent();
void stopWiFiAndSleep();
void setupSyslog();
void setupNetwork();

#endif  // FLOWER_NETWORK_SETUP_H