#ifndef ROLL_H
#define ROLL_H

#include <stdint.h>
#include <stdio.h>

int dice_roll();
void roll(int dice_combo[5], int positions[5]);
void bit_roll(int dice_combo[5], uint8_t mask);

#endif
