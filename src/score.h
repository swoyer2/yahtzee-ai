#include <stdbool.h>

#ifndef SCORE_H
#define SCORE_H

#define SCORE_CARD_SIZE 15

enum scoring_category {
	ACES,
	TWOS,
	THREES,
	FOURS,
	FIVES,
	SIXES,
	THREE_OF_A_KIND,
	FOUR_OF_A_KIND,
	FULL_HOUSE,
	SMALL_STRAIGHT,
	LARGE_STRAIGHT,
	CHANCE,
	YAHTZEE
};

int sum_dice(int dice_combo[5]);
void get_dice_counts(int dice_combo[5], int dice_counts[6]);
bool is_straight(int dice_combo[5], int straight_size);
bool is_full_house(int dice_combo[5]);
int score_combo(int dice_combo[5], enum scoring_category category);
void update_score_card(
	int card[13], 
	int dice_combo[5], 
	enum scoring_category category);
int tally_score(int card[13]);
bool is_yahtzee_bonus(int card[13], int dice_combo[5]);
#endif
