#ifndef FLOWER_PATTERNS_H
#define FLOWER_PATTERNS_H

#include <globals.h>

void rainbowPetals();
void rainbowFlower();
void insideOut();
void sineMove();
void plasma();
void colorFill();
void e131Fill();
void artnetFill();
void onArtnetFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);

#endif  // FLOWER_PATTERNS_H