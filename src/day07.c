#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

void bad_input()
{
    printf("The input file isn't valid!\n");
    exit(-1);
}

#define MAX_LINE_NUMBER 1000
#define MAX_LINE_SIZE 15

struct hand {
    int64_t strength;
    int bid;
};

int compare_hand(const void* lhs, const void* rhs)
{
    if (((struct hand*)lhs)->strength == ((struct hand*)rhs)->strength) return 0;
    return 2*(((struct hand*)lhs)->strength > ((struct hand*)rhs)->strength) -1;
}

int main(int argc, char**argv)
{
    // file parsing
    if(argc<2) bad_input(); // exit execution
    FILE* input_file = fopen(argv[1], "r");
    if(!input_file) bad_input(); // exit execution
    char input[MAX_LINE_NUMBER][MAX_LINE_SIZE];
    int line_nb = 0;
    while(line_nb<1000 && fgets(input[line_nb], 200, input_file)) line_nb++;
    fclose(input_file);
    
    // For the sake of speed, what follows suppose the input is valid
    clock_t start_point = clock();

    uint64_t output1 = 0, output2 = 0;

    struct hand hands[1000];
    char card_to_val[85];
    card_to_val['A'] = 12;
    card_to_val['K'] = 11;
    card_to_val['Q'] = 10;
    card_to_val['J'] = 9;
    card_to_val['T'] = 8;
    for (char c = '9'; c >= '2'; c--)
        card_to_val[c] = c - '2';
    int magic_value = 371293;
    char card_counter[13];

    struct hand hands2[1000];
    char card_to_val2[85];
    card_to_val2['A'] = 12;
    card_to_val2['K'] = 11;
    card_to_val2['Q'] = 10;
    card_to_val2['T'] = 9;
    for (char c = '9'; c >= '2'; c--)
        card_to_val2[c] = c - '2' + 1;
    card_to_val2['J'] = 0;

    for (int i = 0; i < line_nb; i++)
    {
        hands[i].bid = hands2[i].bid = atoi(input[i]+6);
        hands[i].strength = hands2[i].strength = 0;
        memset(card_counter, 0, 13);
        int nb_of_J = 0;
        for (int j = 0; j < 5; j++) {
            hands[i].strength *= 13;
            hands[i].strength += card_to_val[input[i][j]];
            card_counter[card_to_val[input[i][j]]]++;

            hands2[i].strength *= 13;
            hands2[i].strength += card_to_val2[input[i][j]];
            if (input[i][j] == 'J') nb_of_J++;
        }
        int nb_of_twos = 0, max_nb = 0;
        for (int k = 0; k < 13; k++) {
            if (max_nb < card_counter[k]) max_nb = card_counter[k];
            if (card_counter[k] == 2) nb_of_twos++;
        }
        if (max_nb == 5) hands[i].strength += magic_value * 6;
        if (max_nb == 4) hands[i].strength += magic_value * 5;
        if (max_nb == 3) hands[i].strength += magic_value * (3 + nb_of_twos);
        if (max_nb == 2) hands[i].strength += magic_value * nb_of_twos;

        nb_of_twos = max_nb = 0;
        for (int k = 0; k < 13; k++) {
            if (k == 9) continue;
            if (max_nb < card_counter[k]) max_nb = card_counter[k];
            if (card_counter[k] == 2) nb_of_twos++;
        }
        if (max_nb == 2 && nb_of_J) nb_of_twos--;
        if (max_nb + nb_of_J == 5) hands2[i].strength += magic_value * 6;
        if (max_nb + nb_of_J == 4) hands2[i].strength += magic_value * 5;
        if (max_nb + nb_of_J == 3) hands2[i].strength += magic_value * (3 + nb_of_twos);
        if (max_nb + nb_of_J == 2) hands2[i].strength += magic_value * (nb_of_twos + nb_of_J);
    }

    qsort(hands, line_nb, sizeof(struct hand), compare_hand);
    qsort(hands2, line_nb, sizeof(struct hand), compare_hand);
    for (int i = 0; i < line_nb; i++) output1 += hands[i].bid * (i+1);
    for (int i = 0; i < line_nb; i++) output2 += hands2[i].bid * (i+1);

    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}
