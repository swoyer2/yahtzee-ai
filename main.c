#include <stdio.h>
#include <stdint.h>

#include "roll.h"
#include "score.h"
#include "agent.h"

void print_card(int card[13]) {
  printf(
      "\n0 -> Aces: %d\n\
1 -> Twos: %d\n\
2 -> Threes: %d\n\
3 -> Fours: %d\n\
4 -> Fives: %d\n\
5 -> Sixes: %d\n\
\n\
6 -> 3 of a Kind: %d\n\
7 -> 4 of a Kind: %d\n\
8 -> Full House: %d\n\
9 -> Sm Straight: %d\n\
10 -> Lg Straight: %d\n\
11 -> Chance: %d\n\
12 -> Yahtzee: %d\n\
\n\
Total Score: %d\n",
      card[0], card[1], card[2], card[3], card[4], card[5],
      card[6], card[7], card[8],
      card[9], card[10], card[11], card[12], tally_score(card));
}

enum scoring_category get_category_input() {
  int user_input;

  printf("\nWhere would you like to score? ");

  while (scanf("%d", &user_input) != 1) {
    printf("Invalid. Provide a number\n");
  }

  return (enum scoring_category)user_input;
}

void print_mask_binary(uint8_t mask) {
    for (int i = 4; i >= 0; --i) {   
        printf("%u", (mask >> i) & 1);
    }
}

int main() {
  int dice_combo[5];
  enum scoring_category cat;
  int card[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; 

  print_card(card);
  int mask[5] = {1, 1, 1, 1, 1};
  roll(dice_combo, mask);

  for (int i = 0; i < 5; i++) printf("%d", dice_combo[i]);

  uint8_t best_mask = get_best_mask(dice_combo, card);
  printf("\n");
  print_mask_binary(best_mask);
  printf("\n");
  bit_roll(dice_combo, best_mask);

  cat = get_category_input();
  update_score_card(card, dice_combo, cat);

  print_card(card);
}
