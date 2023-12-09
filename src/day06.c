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

bool eol(char c) {return !c || c == '\n' || c == '\r';}

#define MAX_LINE_NUMBER 3
#define MAX_LINE_SIZE 70

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

    uint64_t output1 = 1, output2 = 0;

    // Also you could just take a sheet of paper, a pen and 2 minutes of your life and work it out

    int races_nb = 0;
    int times[10];
    
    char* ptr = &input[0][5];
    while (!eol(*ptr)) {
        while(*ptr == ' ') ptr++;
        times[races_nb] = strtoll(ptr, &ptr, 10);
        races_nb++;
        ptr++;
    }
    ptr = &input[1][9];
    for (int i = 0; i < races_nb; i++) {
        while(*ptr == ' ') ptr++;
        double delta = sqrt((double)times[i]*times[i]-4*strtoll(ptr, &ptr, 10));
        double x1 = 0.5*(times[i]-delta), x2 = 0.5*(times[i]+delta);
        output1 *= ceil(x2-1) - (int)(x1+1) + 1;
        ptr++;
    }

    // PART 2

    int64_t time = 0, dist = 0;
    for (char* ptr = input[0]+5; !eol(*ptr); ptr++) {
        if (*ptr >= '0' && *ptr <= '9') time = 10*time + (*ptr - '0');
    }
    for (char* ptr = input[1]+9; !eol(*ptr); ptr++) {
        if (*ptr >= '0' && *ptr <= '9') dist = 10*dist + (*ptr - '0');
    }

    double delta = sqrt((double)time*time-4*dist);
    double x1 = 0.5*(time-delta), x2 = 0.5*(time+delta);
    output2 = ceil(x2-1) - (int64_t)(x1+1) + 1;


    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}

