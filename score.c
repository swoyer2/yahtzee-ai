#include "score.h"

#include <stdbool.h>

int sum_dice(int dice_combo[5]) {
  int count = 0;
  for (int i = 0; i < 5; i++) count += dice_combo[i];

  return count;
}

void get_dice_counts(int dice_combo[5], int dice_counts[6]) {
  for (int i = 0; i < 5; i++) dice_counts[dice_combo[i] - 1] += 1;
}

bool is_straight(int dice_combo[5], int straight_size) {
  int in_a_row = 0;
  int dice_counts[6] = {0, 0, 0, 0, 0, 0};
  get_dice_counts(dice_combo, dice_counts);

  for (int i = 0; i < 6; i++) {
    if (dice_counts[i] != 0)
      in_a_row += 1;
    else
      in_a_row = 0;
    if (in_a_row == straight_size) return true;
  }
  return false;
}

bool is_full_house(int dice_combo[5]) {
  int dice_counts[6] = {0, 0, 0, 0, 0, 0};
  get_dice_counts(dice_combo, dice_counts);

  bool has_three_of_a_kind = false;
  bool has_pair = false;

  for (int i = 0; i < 6; i++) {
    if (dice_counts[i] == 3)
      has_three_of_a_kind = true;
    else if (dice_counts[i] == 2)
      has_pair = true;
  }
  if (has_three_of_a_kind && has_pair) return true;
  return false;
}

int score_combo(int dice_combo[5], enum scoring_category category) {
  int dice_counts[6] = {0, 0, 0, 0, 0, 0};
  get_dice_counts(dice_combo, dice_counts);

  switch (category) {
    case ACES:
      return 1 * dice_counts[0];
    case TWOS:
      return 2 * dice_counts[1];
    case THREES:
      return 3 * dice_counts[2];
    case FOURS:
      return 4 * dice_counts[3];
    case FIVES:
      return 5 * dice_counts[4];
    case SIXES:
      return 6 * dice_counts[5];
    case THREE_OF_A_KIND:
      for (int i = 0; i < 6; i++) {
        if (dice_counts[i] >= 3) return sum_dice(dice_combo);
      }
      return 0;
    case FOUR_OF_A_KIND:
      for (int i = 0; i < 6; i++) {
        if (dice_counts[i] >= 4) return sum_dice(dice_combo);
      }
      return 0;
    case YAHTZEE:
      for (int i = 0; i < 6; i++) {
        if (dice_counts[i] >= 5) return 50;
      }
      return 0;
    case FULL_HOUSE:
      if (is_full_house(dice_combo)) return 25;
      return 0;
    case SMALL_STRAIGHT:
      if (is_straight(dice_combo, 4)) return 30;
      return 0;
    case LARGE_STRAIGHT:
      if (is_straight(dice_combo, 5)) return 40;
      return 0;
    case CHANCE:
      int sum = 0;
      for (int i = 0; i < 5; i++) sum += dice_combo[i];
      return sum;
    default:
      return -1;
  }
}

void update_score_card(int card[13], int dice_combo[5], enum scoring_category category) {
  int score = score_combo(dice_combo, category);
  card[category] = score; 
}

int tally_score(int card[13]) {
  int final_score = 0;

  for (int i = 0; i < 13; i++) {
    if (card[i] != -1) {
      final_score += card[i];
    }
    if (i == 5 && final_score == 63) {
      final_score += 35;
    }
  }

  return final_score;
}
