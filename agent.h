#ifndef AGENT_H
#define AGENT_H

#include <stdint.h>

#define NUM_REROLL_MASKS 32

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
  
struct Move {
  int cat;
  int score;
};

uint8_t get_best_mask(int dice_combo[5], int card[13], int rolls_left); 
static inline int ipow(int base, int exp); 
double ev_for_mask(int dice_combo[5], uint8_t mask, int card[13], int rolls_left); 
struct Move get_best_move(int dice_combo[5], int card[13]);
double get_best_ev(int dice_combo[5], int card[13], int rolls_left);

#endif
