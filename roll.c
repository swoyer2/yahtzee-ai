#include "roll.h"

#include <stdlib.h>
#include <stdint.h>

int dice_roll() { return arc4random_uniform(6) + 1; }

void roll(int dice_combo[5], int positions[5]) {
  for (int i = 0; i < 5; i++) {
    if (positions[i] == 1) {
      dice_combo[i] = dice_roll();
    }
  }
}

void bit_roll(int dice_combo[5], uint8_t mask) {
    for (int i = 0; i < 5; ++i) {
        if (mask & (1 << i)) {
            printf("Rolling die %d\n", i);
            dice_combo[i] = (rand() % 6) + 1;
        }
    }
}

