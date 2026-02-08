#ifndef AGENT_H
#define AGENT_H

#include <stdint.h>
#include <stdlib.h>

#define NUM_REROLL_MASKS 32
#define NUM_REROLL_MASKS_PAIR 24
#define NUM_REROLL_MASKS_TWO_PAIR 18
#define NUM_REROLL_MASKS_TRIPS 16
#define NUM_REROLL_MASKS_FULL_HOUSE 12
#define NUM_REROLL_MASKS_QUADS 10
#define NUM_REROLL_MASKS_YAHTZEE 6

#define NUM_DICE_STATES 252
#define NUM_CARD_STATES 8192
#define NUM_ROLLS 3

static const uint8_t REROLL_MASKS_YAHTZEE[NUM_REROLL_MASKS_YAHTZEE] = {
    0b00000,
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11111
};

static const uint8_t REROLL_MASKS_QUADS[NUM_REROLL_MASKS_QUADS] = {
    0b00000,
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b00001,
    0b10001,
    0b11001,
    0b11101,
    0b11111
};

static const uint8_t REROLL_MASKS_FULL_HOUSE[NUM_REROLL_MASKS_FULL_HOUSE] = {
    0b00000,
    0b10000,
    0b11000,
    0b11100,
    0b00010,
    0b10010,
    0b11010,
    0b11110,
    0b00011,
    0b10011,
    0b11011,
    0b11111
};

static const uint8_t REROLL_MASKS_TRIPS[NUM_REROLL_MASKS_TRIPS] = {
    0b00000,
    0b10000,
    0b11000,
    0b11100,
    0b00010,
    0b10010,
    0b11010,
    0b11110,
    0b00001,
    0b10001,
    0b11001,
    0b11101,
    0b00011,
    0b10011,
    0b11011,
    0b11111
};

static const uint8_t REROLL_MASKS_TWO_PAIR[NUM_REROLL_MASKS_TWO_PAIR] = {
    0b00000,
    0b10000,
    0b11000,
    0b00100,
    0b10100,
    0b11100,
    0b00110,
    0b10110,
    0b11110,
    0b00001,
    0b10001,
    0b11001,
    0b00101,
    0b10101,
    0b11101,
    0b00111,
    0b10111,
    0b11111
};
static const uint8_t REROLL_MASKS_PAIR[NUM_REROLL_MASKS_PAIR] = {
    0b00000,
    0b10000,
    0b11000,
    0b00100,
    0b10100,
    0b11100,
    0b00010,
    0b10010,
    0b11010,
    0b00110,
    0b10110,
    0b11110,
    0b00001,
    0b10001,
    0b11001,
    0b00101,
    0b10101,
    0b11101,
    0b00011,
    0b10011,
    0b11011,
    0b00111,
    0b10111,
    0b11111
};
 
static const uint8_t REROLL_MASKS[NUM_REROLL_MASKS] = {
    0b00000,
    0b00001,
    0b00010,
    0b00011,
    0b00100,
    0b00101,
    0b00110,
    0b00111,
    0b01000,
    0b01001,
    0b01010,
    0b01011,
    0b01100,
    0b01101,
    0b01110,
    0b01111,
    0b10000,
    0b10001,
    0b10010,
    0b10011,
    0b10100,
    0b10101,
    0b10110,
    0b10111,
    0b11000,
    0b11001,
    0b11010,
    0b11011,
    0b11100,
    0b11101,
    0b11110,
    0b11111
};
  
// Counts of 1s in the bitmasks
static const uint8_t BITCOUNT[32] = {
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
};

// --- Types ---

typedef struct {
    const uint8_t *masks;
    int count;
} MaskGroup;

typedef uint16_t CardMask;

typedef struct {
    uint8_t c[6];
} DiceState;

struct Move {
    int cat;
    int score;
};

// --- Functions ---

void init_dice_states(void);
void init_memo(void);

double get_best_ev(int ordered_dice_combo[5], int card[13], int rolls_left);
uint8_t get_best_mask(int dice_combo[5], int card[13], int rolls_left);
struct Move get_best_move(int ordered_dice_combo[5], int card[13]);
CardMask card_to_mask(const int card[13]);
int card_used(CardMask card, int i);
CardMask card_mark(CardMask card, int i);
void reorder_dice_combo(int dice_combo[5], int ordered_dice_combo[5]);
double ev_for_mask(int ordered_dice_combo[5], uint8_t mask, int card[13], int rolls_left);
int bonus_heuristic(int card[13]);

int dice_state_id(DiceState s);
DiceState dice_to_state(int dice[5]);
CardMask card_to_mask(const int card[13]);
int card_used(CardMask card, int i);
CardMask card_mark(CardMask card, int i);
int category_order_heuristic(int card[13], int category); 

#endif // AGENT_H
