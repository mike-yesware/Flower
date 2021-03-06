#ifndef FLOWER_LED_UTILS_H
#define FLOWER_LED_UTILS_H

#include <globals.h>
#include <palettes.h>

uint8_t numLedsPetals();
uint8_t numLeds();
void copyLeds();
void remapFirstToAll();
void fillPetal(uint8_t petal, CRGB color);
void remapSolid();
void remap12();
void remapDirection();
void remapClockwise();

#endif  // FLOWER_LED_UTILS_H