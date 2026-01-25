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

double ev_for_mask(int ordered_dice_combo[5], uint8_t mask, int card[13], int rolls_left) {
  int k = BITCOUNT[mask];
  int n = ipow(6, k);

  int outcome[5];
  double total = 0;

  for (int i = 0; i < n; i++) {
    memcpy(outcome, ordered_dice_combo, 5 * sizeof(int));

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

// return
// 0 -> single 1 -> pair 2 -> two pair 3 -> trips 4 -> full house 5 -> quads 6 -> yahtzee
int get_combo_type(int ordered_dice_combo[5]) {
  if (ordered_dice_combo[0] == ordered_dice_combo[4]) {
    return 6;
  }
  if (ordered_dice_combo[1] == ordered_dice_combo[4]) {
    return 5;
  }
  if (ordered_dice_combo[2] == ordered_dice_combo[4] && ordered_dice_combo[0] == ordered_dice_combo[1]) {
    return 4;
  }
  if (ordered_dice_combo[2] == ordered_dice_combo[4]) {
    return 3;
  }
  if (ordered_dice_combo[1] == ordered_dice_combo[2] && ordered_dice_combo[3] == ordered_dice_combo[4]) {
    return 2;
  }
  if (ordered_dice_combo[3] == ordered_dice_combo[4]) {
    return 1;
  }
  return 0;
}

uint8_t get_best_mask(int dice_combo[5], int card[13], int rolls_left) {
  int ordered_dice_combo[5];
  reorder_dice_combo(dice_combo, ordered_dice_combo);

  int combo_type = get_combo_type(ordered_dice_combo);
  const uint8_t *mask_group = REROLL_MASKS;
  int num_masks = NUM_REROLL_MASKS;
 
  switch (combo_type) {
      case 0: 
          mask_group = REROLL_MASKS;
          num_masks = NUM_REROLL_MASKS;
          break;

      case 1: // pair
          mask_group = REROLL_MASKS_PAIR;
          num_masks = NUM_REROLL_MASKS_PAIR;
          break;

      case 2: // two pair
          mask_group = REROLL_MASKS_TWO_PAIR;
          num_masks = NUM_REROLL_MASKS_TWO_PAIR;
          break;

      case 3: // trips
          mask_group = REROLL_MASKS_TRIPS;
          num_masks = NUM_REROLL_MASKS_TRIPS;
          break;

      case 4: // full house
          mask_group = REROLL_MASKS_FULL_HOUSE;
          num_masks = NUM_REROLL_MASKS_FULL_HOUSE;
          break;

      case 5: // quads
          mask_group = REROLL_MASKS_QUADS;
          num_masks = NUM_REROLL_MASKS_QUADS;
          break;

      case 6: // yahtzee
          mask_group = REROLL_MASKS_YAHTZEE;
          num_masks = NUM_REROLL_MASKS_YAHTZEE;
          break;
  }

  uint8_t best_mask = mask_group[0]; 
  double best_ev = ev_for_mask(ordered_dice_combo, best_mask, card, rolls_left);

  for (int i = 1; i < num_masks; i++) {
    double ev = ev_for_mask(ordered_dice_combo, mask_group[i], card, rolls_left);
    if (ev > best_ev) {
      best_mask = mask_group[i];
      best_ev = ev;
    }
  }
  return best_mask;
}

double get_best_ev(int ordered_dice_combo[5], int card[13], int rolls_left) {
  if (rolls_left == 0) {
    return get_best_move(ordered_dice_combo, card).score;
  }

  int combo_type = get_combo_type(ordered_dice_combo);
  const uint8_t *mask_group = REROLL_MASKS;
  int num_masks = NUM_REROLL_MASKS;
 
  switch (combo_type) {
    case 0: 
        mask_group = REROLL_MASKS;
        num_masks = NUM_REROLL_MASKS;
        break;

    case 1: // pair
        mask_group = REROLL_MASKS_PAIR;
        num_masks = NUM_REROLL_MASKS_PAIR;
        break;

    case 2: // two pair
        mask_group = REROLL_MASKS_TWO_PAIR;
        num_masks = NUM_REROLL_MASKS_TWO_PAIR;
        break;

    case 3: // trips
        mask_group = REROLL_MASKS_TRIPS;
        num_masks = NUM_REROLL_MASKS_TRIPS;
        break;

    case 4: // full house
        mask_group = REROLL_MASKS_FULL_HOUSE;
        num_masks = NUM_REROLL_MASKS_FULL_HOUSE;
        break;

    case 5: // quads
        mask_group = REROLL_MASKS_QUADS;
        num_masks = NUM_REROLL_MASKS_QUADS;
        break;

    case 6: // yahtzee
        mask_group = REROLL_MASKS_YAHTZEE;
        num_masks = NUM_REROLL_MASKS_YAHTZEE;
        break;
  }
 
  double best = -1e9;
  for (int i = 0; i < num_masks; i++) {
    double ev = ev_for_mask(ordered_dice_combo, mask_group[i], card, rolls_left);
    if (ev > best) { 
      best = ev;
    }
  }
  return best;
}

// reorders so it will push largest group of nums to the right ex: [5, 5, 1, 1, 1]
void reorder_dice_combo(int dice_combo[5], int ordered_dice_combo[5]) {
  typedef struct {
    int value;
    int count;
  } Group;

  int freq[7] = {0};
  for (int i = 0; i < 5; i++) {
    freq[dice_combo[i]] += 1;
  }

  Group groups[5];
  int group_count = 0;

  for (int v = 1; v <= 6; v++) {
    if (freq[v] > 0) {
      groups[group_count].value = v;
      groups[group_count].count = freq[v];
      group_count += 1;
    }
  }

  for (int i = 0; i < group_count - 1; i++) {
    for (int j = i + 1; j < group_count; j++) {
      if (groups[i].count > groups[j].count) {
        Group tmp = groups[i];
        groups[i] = groups[j];
        groups[j] = tmp;
      }
    }
  }

  int idx = 0;
  for (int g = 0; g < group_count; g++) {
    for (int k = 0; k < groups[g].count; k++) {
      ordered_dice_combo[idx++] = groups[g].value;
    }
  }
}


