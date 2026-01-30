#include "roll.h"
#include "score.h"
#include "agent.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// --- Global variables ---

DiceState ALL_DICE_STATES[NUM_DICE_STATES];
double *EV_TABLE;
uint8_t *EV_VALID;

// --- Dice state initialization ---

void init_dice_states(void) {
    int id = 0;
    for (int a = 0; a <= 5; a++)
    for (int b = 0; b <= 5 - a; b++)
    for (int c = 0; c <= 5 - a - b; c++)
    for (int d = 0; d <= 5 - a - b - c; d++)
    for (int e = 0; e <= 5 - a - b - c - d; e++) {
        int f = 5 - a - b - c - d - e;
        ALL_DICE_STATES[id++] = (DiceState){{a,b,c,d,e,f}};
    }
}

// --- Helper functions ---

static inline MaskGroup get_mask_group(int combo_type) {
    switch (combo_type) {
        case 1: return (MaskGroup){ REROLL_MASKS_PAIR,       NUM_REROLL_MASKS_PAIR };
        case 2: return (MaskGroup){ REROLL_MASKS_TWO_PAIR,  NUM_REROLL_MASKS_TWO_PAIR };
        case 3: return (MaskGroup){ REROLL_MASKS_TRIPS,     NUM_REROLL_MASKS_TRIPS };
        case 4: return (MaskGroup){ REROLL_MASKS_FULL_HOUSE,NUM_REROLL_MASKS_FULL_HOUSE };
        case 5: return (MaskGroup){ REROLL_MASKS_QUADS,     NUM_REROLL_MASKS_QUADS };
        case 6: return (MaskGroup){ REROLL_MASKS_YAHTZEE,   NUM_REROLL_MASKS_YAHTZEE };
        default:return (MaskGroup){ REROLL_MASKS,           NUM_REROLL_MASKS };
    }
}

static inline int ipow(int base, int exp) {
    int r = 1;
    while (exp--) r *= base;
    return r;
}

static inline size_t memo_index(CardMask card, int dice_id, int rolls_left) {
    return ((size_t)card * NUM_DICE_STATES + dice_id) * NUM_ROLLS + rolls_left;
}

// --- Dice & card conversions ---

DiceState dice_to_state(int dice[5]) {
    DiceState s = {{0}};
    for (int i = 0; i < 5; i++) s.c[dice[i]-1]++;
    return s;
}

int dice_state_id(DiceState s) {
    for (int i = 0; i < NUM_DICE_STATES; i++) {
        if (memcmp(&s, &ALL_DICE_STATES[i], sizeof(DiceState)) == 0)
            return i;
    }
    return -1;
}

CardMask card_to_mask(const int card[13]) {
    CardMask mask = 0;
    for (int i = 0; i < 13; i++)
        if (card[i] != -1) mask |= (1u << i);
    return mask;
}

int card_used(CardMask card, int i) {
    return (card >> i) & 1;
}

CardMask card_mark(CardMask card, int i) {
    return card | (1u << i);
}

// --- Reorder dice to group same numbers together ---

void reorder_dice_combo(int dice[5], int out[5]) {
    int freq[7] = {0};
    for (int i = 0; i < 5; i++) freq[dice[i]]++;

    int idx = 0;
    for (int count = 1; count <= 5; count++)
        for (int v = 1; v <= 6; v++)
            if (freq[v] == count)
                for (int k = 0; k < count; k++) out[idx++] = v;
}

// --- Combo type detection ---

int get_combo_type(int d[5]) {
    if (d[0] == d[4]) return 6;        // Yahtzee
    if (d[1] == d[4]) return 5;        // Quads
    if (d[2] == d[4] && d[0] == d[1]) return 4; // Full House
    if (d[2] == d[4]) return 3;        // Trips
    if (d[1] == d[2] && d[3] == d[4]) return 2; // Two Pair
    if (d[3] == d[4]) return 1;        // Pair
    return 0;
}

// --- EV calculations ---

double ev_for_mask(int dice[5], uint8_t mask, int card[13], int rolls_left) {
    int k = BITCOUNT[mask];
    int n = ipow(6, k);

    int outcome[5];
    double total = 0;

    for (int i = 0; i < n; i++) {
        memcpy(outcome, dice, 5 * sizeof(int));
        int temp = i;
        for (int d = 4; d >= 0; d--) {
            if (mask & (1 << d)) {
                outcome[d] = (temp % 6) + 1;
                temp /= 6;
            }
        }
        total += get_best_ev(outcome, card, rolls_left - 1);
    }
    return total / n;
}

uint8_t get_best_mask(int dice[5], int card[13], int rolls_left) {
    MaskGroup group = get_mask_group(get_combo_type(dice));

    uint8_t best_mask = group.masks[0];
    double best_ev = ev_for_mask(dice, best_mask, card, rolls_left);

    for (int i = 1; i < group.count; i++) {
        double ev = ev_for_mask(dice, group.masks[i], card, rolls_left);
        if (ev > best_ev) {
            best_ev = ev;
            best_mask = group.masks[i];
        }
    }
    return best_mask;
}

double get_best_ev(int dice[5], int card[13], int rolls_left) {
    DiceState ds = dice_to_state(dice);
    int dice_id = dice_state_id(ds);

    if (dice_id < 0) return 0; // safeguard

    CardMask card_mask = card_to_mask(card);
    size_t idx = memo_index(card_mask, dice_id, rolls_left);

    if (EV_VALID[idx]) return EV_TABLE[idx];

    double result;

    if (rolls_left == 0) {
        result = get_best_move(dice, card).score;
    } else {
        MaskGroup group = get_mask_group(get_combo_type(dice));
        double best = -1e9;
        for (int i = 0; i < group.count; i++) {
            double ev = ev_for_mask(dice, group.masks[i], card, rolls_left);
            if (ev > best) best = ev;
        }
        result = best;
    }

    EV_VALID[idx] = 1;
    EV_TABLE[idx] = result;
    return result;
}

// --- Best move selection ---

struct Move get_best_move(int dice[5], int card[13]) {
    int best_score = -1;
    int best_cat = -1;
    int temp_card[13];

    for (int i = 0; i < 13; i++) {
        if (!card || card[i] == -1) {
            if (card) memcpy(temp_card, card, sizeof(temp_card));
            else for (int j = 0; j < 13; j++) temp_card[j] = -1;

            update_score_card(temp_card, dice, i);
            int score = tally_score(temp_card);
            score += bonus_heuristic(temp_card);

            if (score > best_score) {
                best_score = score;
                best_cat = i;
            }
        }
    }

    return (struct Move){ best_cat, best_score };
}

int bonus_heuristic(int card[13]) {
  int points_needed = 63;
  int possible_points_left = 0;

  for (int i = 0; i < 6; i++) {
    if (card[i] != -1) {
      points_needed -= card[i];
    }
    else {
      possible_points_left += (i+1) * 5;
    }
  }

  if (points_needed <= 0) {
    return 0; // already got bonus
  }
  else if (points_needed > possible_points_left) {
    return -35; // Not possible to get bonus
  }
  else {
    int feasibility = possible_points_left - points_needed;
    return (int) ( -35.0 + 35.0 * ((double)feasibility / 42.0)); 
  }
}

// --- Memoization table initialization ---

void init_memo(void) {
    size_t N = (size_t)NUM_CARD_STATES * NUM_DICE_STATES * NUM_ROLLS;
    EV_TABLE = malloc(N * sizeof(double));
    EV_VALID = calloc(N, sizeof(uint8_t));
    if (!EV_TABLE || !EV_VALID) {
        fprintf(stderr, "Failed to allocate memo tables\n");
        exit(1);
    }
}

