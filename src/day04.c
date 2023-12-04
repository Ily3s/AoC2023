#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

void bad_input()
{
    printf("The input file isn't valid!\n");
    exit(-1);
}

inline bool eol(char c) {return !c || c == '\n' || c == '\r';}

#define MAX_LINE_NUMBER 200
#define MAX_LINE_SIZE 200

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

    char winning_nums[10];
    uint64_t cards_instances[200];
    for (int i = 0; i < line_nb; i++)
        cards_instances[i] = 1;

    for (int i = 0; i < line_nb; i++)
    {
        int matches = 0;
        int winning_nums_count = 0;
        char* ptr = input[i];

        // Get the winning numbers
        while (*ptr != ':') ptr++; ptr++;
        while (*ptr == ' ') ptr++;
        while (*ptr != '|')
        {
            winning_nums[winning_nums_count] =  strtoll(ptr, &ptr, 10);
            winning_nums_count++; ptr++;
            while (*ptr == ' ') ptr++;
        } 
        ptr++; while (*ptr == ' ') ptr++;

        // get and compare the numbers the player got with the winning numbers
        while (!eol(*ptr))
        {
            char num = strtoll(ptr, &ptr, 10);
            for (int k = 0; k < winning_nums_count; k++)
                matches += winning_nums[k] == num;
            ptr++; while(*ptr == ' ') ptr++;
        }

        // Update the outputs and the number of cards instances
        output1 += 1<<matches>>1;
        output2 += cards_instances[i];
        for (int offset = 1; offset <= matches; offset++)
            cards_instances[i+offset] += cards_instances[i];
    }

    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}
