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

// Colors are represented by their first character
inline char color_to_index(char c) {return (c=='b') + 2*(c=='g');}
inline char color_to_length(char c){return color_to_index(c) + 3;}
inline char color_to_max(char c) {return color_to_index(c)+12 + (c=='b') - (c=='g');}

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
    time_t start_point = time(NULL);

    uint64_t output1 = 0;
    uint64_t output2 = 0;

    // for part 2
    char required[3];

    for (int i = 0; i < line_nb; i++)
    {
        if (input[i][0] != 'G') break; // eof
        int j = 8 + (i >= 9) + (i >= 99);
        bool eol = false;
        bool impossible = false;
        memset(required, 0, 3);
        while(!eol)
        {
            char* temp;
            int number = strtoll(input[i]+j, &temp, 10);
            j = temp - input[i] + 1;
            int index = color_to_index(input[i][j]);
            if (number > required[index]) required[index] = number;
            impossible |= number > color_to_max(input[i][j]);
            j += color_to_length(input[i][j]);
            eol |= !input[i][j];
            j += 2;
        }
        output1 += !impossible * (i+1);
        output2 += required[0]*required[1]*required[2];
    }

    time_t end_point = time(NULL);

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", difftime(end_point, start_point)*1000000.0);

    return 0;
}
