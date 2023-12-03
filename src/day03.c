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

    for (int i = 0; i < line_nb; i++)
    {
        for (int j = 0; input[i][j] > ' '; j++)
        {
            if ((input[i][j] < '0' || input[i][j] > '9') && input[i][j] != '.') // special character
            {
                int number_count = 0, gear_ratio = 1;
                for (int y = i-(i>0); y <= i+(i<line_nb-1); y++) {
                    for (int x = j-(j>0); x <= j+1; x++) {
                        if (input[y][x] < '0' || input[y][x] > '9') continue;
                        while (x > 0 && input[y][x-1] >= '0' && input[y][x-1] <= '9') x--;
                        char* end_ptr;
                        int number = strtoll(&input[y][x], &end_ptr, 10);
                        output1 += number;
                        x = end_ptr - input[y] - !!number;
                        number_count += !!number;
                        gear_ratio *= number + !number;
                    }
                }
                if (number_count == 2 && input[i][j] == '*') output2 += gear_ratio;
            }
        }
    }

    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

}
