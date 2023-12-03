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

#define MAX_LINE_NUMBER 1000
#define MAX_LINE_SIZE 200

struct digit {
    char* str;
    int i;
} digits[10];

/* i : current line index
 * k (in,out) : pointer to an index to the last digit incountered
 * val (out) : value of the last string-digit incountered
 * fst : if we are searching for the first (or the last) digit
 * return : if the digit is a string or not */
bool find_digit(char input[MAX_LINE_NUMBER][MAX_LINE_SIZE], int i, int* k, int* val, bool fst);

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

    uint64_t output = 0;

    for(int i=0; i<line_nb; i++)
    {
        int j = 0;
        while(input[i][j]<'0' || input[i][j]>'9') j++;
        output += (input[i][j]-'0')*10;
        for (int k = j+1; input[i][k]; k++) {
            if (input[i][k] >= '0' && input[i][k] <= '9') j = k;
        }
        output += input[i][j]-'0';
    }

    clock_t end_point = clock();
    
    printf("PART 1 :\n");
    printf("Output : %"PRIu64"\n", output);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    start_point = clock();

    output = 0;

    digits[1].str = "one";  digits[4].str = "four"; digits[7].str = "seven";
    digits[2].str = "two";  digits[5].str = "five"; digits[8].str = "eight";
    digits[3].str = "three";digits[6].str = "six";  digits[9].str = "nine";

    for (int i = 0; i < line_nb; i++)
    {
        int k = 0;
        int val = 0;
        bool is_string = find_digit(input, i, &k, &val, true);
        output += (!is_string*(input[i][k]-'0') + is_string*val)*10;
        is_string = find_digit(input, i, &k, &val, false);
        output += !is_string*(input[i][k]-'0') + is_string*val;
    }

    end_point = clock();
    
    printf("\nPART 2 :\n");
    printf("Output : %"PRIu64"\n", output);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}

bool find_digit(char input[MAX_LINE_NUMBER][MAX_LINE_SIZE], int i, int* k, int* val, bool fst)
{
    bool is_string = false;
    for (int j = 1; j < 10; j++) digits[j].i = 0;
    for (int j = !fst*(*k+1); input[i][j] && (!fst || !is_string); j++)
    {
        if (input[i][j] >= '0' && input[i][j] <= '9') {
            *k = j;
            is_string=false;
            if(fst) break;
        }
        for (int d = 1; d < 10; d++) {
            if (input[i][j] == digits[d].str[digits[d].i]) {
                digits[d].i++;
                if(!digits[d].str[digits[d].i]) {
                    *k = j;
                    is_string = true; *val = d;
                    if(fst) break;
                    digits[d].i = 0;
                }
            } else {
                digits[d].i = 0;
                digits[d].i += input[i][j] == digits[d].str[digits[d].i];
            }
        }
    }
    return is_string;
}
