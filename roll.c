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
            dice_combo[i] = dice_roll();
        }
    }
}

// Roll the dice based on the numbers chosen not position of dice. ex: 34511 set_roll [2,0,1,0,0] would roll both 1s and a 3.
void set_roll(int dice_combo[5], int nums_to_roll[5]) {
  for (int i = 0; i < 5; i++) {
    int dice_val = dice_combo[i];
    if (nums_to_roll[dice_val - 1] > 0) {
      dice_combo[i] = dice_roll();
      nums_to_roll[i] -= 1;
    }
  }
}
