#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

void print_dice(int dice[5]) {
    for (int i = 0; i < 5; i++) printf("%d ", dice[i]);
    printf("\n");
}

void print_mask_binary(uint8_t mask) {
    for (int i = 0; i < 5; i++) printf("%u", (mask >> i) & 1);
    printf("\n");
}

bool is_game_over(int card[13]) {
    for (int i = 0; i < 13; i++) if (card[i] == -1) return false;
    return true;
}

int main() {
    int dice[5];
    int card[13] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

    // Initialize dice states and memo table
    init_dice_states();
    init_memo();

    while (!is_game_over(card)) {
        // --- First roll ---
        int mask[5] = {1,1,1,1,1};
        roll(dice, mask);

        int sorted_dice[5];
        reorder_dice_combo(dice, sorted_dice);
        print_dice(sorted_dice);

        CardMask card_mask = card_to_mask(card);
        uint8_t best_mask = get_best_mask(sorted_dice, card, 2);
        print_mask_binary(best_mask);

        bit_roll(sorted_dice, best_mask);

        // --- Second roll ---
        int sorted_dice_2[5];
        reorder_dice_combo(sorted_dice, sorted_dice_2);
        print_dice(sorted_dice_2);

        best_mask = get_best_mask(sorted_dice_2, card, 1);
        print_mask_binary(best_mask);

        bit_roll(sorted_dice_2, best_mask);
        print_dice(sorted_dice_2);

        // --- Choose best category and mark card ---
        struct Move m = get_best_move(sorted_dice_2, card);
        update_score_card(card, sorted_dice_2, m.cat);
        card_mask = card_mark(card_mask, m.cat);  // update mask

        print_card(card);
    }

    printf("Game Over!\n");
    return 0;
}

