#include "roll.h"
#include "score.h"
#include "agent.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int score_best_cat(int dice_combo[5], int card[13]) {
  int best_score = 0;
  int temp_card[13];

  for (int i = 0; i < 13; i++) {
    memcpy(temp_card, card, sizeof(temp_card));
    update_score_card(temp_card, dice_combo, i); 
    int score = tally_score(temp_card);
    if (score > best_score) {
      best_score = score;
    }
  }
  return best_score;
}

static inline int ipow(int base, int exp) {
  int r = 1;
  while (exp--) r *= base;
  return r;
}

double ev_for_mask(int dice_combo[5], uint8_t mask, int card[13]) {
  int k = BITCOUNT[mask];
  int n = ipow(6, k);

  int outcome[5];
  double total = 0;

  for (int i = 0; i < n; i++) {
    memcpy(outcome, dice_combo, 5 * sizeof(int));

    int temp = i;
    for (int d = 0; d < 5; d++) {
      if (mask & (1 << d)) {
        outcome[d] = (temp % 6) + 1;
        temp /= 6;
      }
    }

    total += score_best_cat(outcome, card);
  }

  return total / n;
}

uint8_t get_best_mask(int dice_combo[5], int card[13]) {
  uint8_t best_mask = REROLL_MASKS[0]; 
  double best_ev = ev_for_mask(dice_combo, best_mask, card);

  for (int i = 1; i < NUM_REROLL_MASKS; i++) {
    double ev = ev_for_mask(dice_combo, REROLL_MASKS[i], card);
    if (ev > best_ev) {
      best_mask = REROLL_MASKS[i];
      best_ev = ev;
    }
  }
  return best_mask;
}
