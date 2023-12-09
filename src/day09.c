#include <stdint.h>
#include <stdio.h>
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

#define MAX_LINE_NUMBER 201
#define MAX_LINE_SIZE 200

bool eol(char c) {return !c || c == '\n' || c == '\r';}

int main(int argc, char**argv)
{
    // file parsing
    if(argc<2) bad_input(); // exit execution
    FILE* input_file = fopen(argv[1], "r");
    if(!input_file) bad_input(); // exit execution
    char input[MAX_LINE_NUMBER][MAX_LINE_SIZE];
    int line_nb = 0;
    while(line_nb<1000 && fgets(input[line_nb], MAX_LINE_SIZE, input_file)) line_nb++;
    fclose(input_file);
    
    // For the sake of speed, what follows suppose the input is valid
    clock_t start_point = clock();

    int64_t output1 = 0, output2 = 0;

    int64_t vals[50];

    for (int i = 0; i < line_nb; i++)
    {
        int val_nb = 0;
        for (char* ptr = input[i]; !eol(*ptr); ptr++) {
            vals[val_nb] = strtoll(ptr, &ptr, 10);
            val_nb++;
        }
        bool fun_is_null = false;
        for (int k = 0; !fun_is_null; k++)
        {
            fun_is_null = true;
            output1 += vals[val_nb-1];
            output2 += vals[0] * (1-2*(k%2));
            for (int j = 0; j < val_nb - 1; j++) {
                vals[j] = vals[j+1] - vals[j];
                fun_is_null &= !vals[j];
            }
            val_nb--;
        }
    }

    clock_t end_point = clock();

    printf("PART 1 output : %"PRIi64"\n", output1);
    printf("PART 2 output : %"PRIi64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}
