#include "roll.h"
#include "score.h"
#include "agent.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct Move get_best_move(int dice_combo[5], int card[13]) {
  int best_score = 0;
  int best_cat = 0;
  int temp_card[13];

  for (int i = 0; i < 13; i++) {
    if (card[i] == -1) {
      memcpy(temp_card, card, sizeof(temp_card));
      update_score_card(temp_card, dice_combo, i); 
      int score = tally_score(temp_card);
      if (score > best_score) {
        best_score = score;
        best_cat = i;
      }
    }
  }
  struct Move best_move = {best_cat , best_score};
  return best_move;
}

static inline int ipow(int base, int exp) {
  int r = 1;
  while (exp--) r *= base;
  return r;
}

double ev_for_mask(int dice_combo[5], uint8_t mask, int card[13], int rolls_left) {
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

    total += get_best_ev(outcome, card, rolls_left - 1);
  }

  return total / n;
}


uint8_t get_best_mask(int dice_combo[5], int card[13], int rolls_left) {
  uint8_t best_mask = REROLL_MASKS[0]; 
  double best_ev = ev_for_mask(dice_combo, best_mask, card, rolls_left);

  for (int i = 1; i < NUM_REROLL_MASKS; i++) {
    double ev = ev_for_mask(dice_combo, REROLL_MASKS[i], card, rolls_left);
    if (ev > best_ev) {
      best_mask = REROLL_MASKS[i];
      best_ev = ev;
    }
  }
  return best_mask;
}

double get_best_ev(int dice_combo[5], int card[13], int rolls_left) {
  if (rolls_left == 0) {
    return get_best_move(dice_combo, card).score;
  }

  double best = -1e9;
  for (int i = 0; i < NUM_REROLL_MASKS; i++) {
    double ev = ev_for_mask(dice_combo, REROLL_MASKS[i], card, rolls_left);
    if (ev > best) { 
      best = ev;
    }
  }
  return best;
}
