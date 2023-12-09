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

#define MAX_LINE_NUMBER 801
#define MAX_LINE_SIZE 400

bool eol(char c) {return !c || c == '\n' || c == '\r';}

struct node {
    int r, l;
    bool is_exit;
};

int str_to_node(const char* c)
{
    int output = 0;
    for (int i = 0; i < 3; i++)
        output = output * 26 + (c[i] - 'A');
    return output;
}

// Thanks euclide, my man
uint64_t gcd(uint64_t a, uint64_t b) {
    if (a % b == 0) return b;
    return gcd(b, a%b);
}

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

    uint64_t output1 = 0, output2 = 0;

    struct node nodes[26*26*26];
    uint64_t entries[100];
    int entries_nb = 0;

    for (int i = 2; i < line_nb; i++)
    {
        int k = str_to_node(input[i]);
        nodes[k].l = str_to_node(input[i]+7);
        nodes[k].r = str_to_node(input[i]+12);
        nodes[k].is_exit = (input[i][2] == 'Z');
        if (input[i][2] == 'A') {
            entries[entries_nb] = k;
            entries_nb++;
        }
    }

    int current = 0; // AAA
    for (int j = 0; current != 26*26*26-1; j++) {
        output1++;
        if (eol(input[0][j])) j = 0;
        current = nodes[current].l * (input[0][j] == 'L') + nodes[current].r * (input[0][j] == 'R');
    }

    for (int i = 0; i < entries_nb; i++) {
        int current = entries[i];
        entries[i] = 0;
        for (int j = 0; !nodes[current].is_exit ; j++) {
            entries[i]++;
            if (eol(input[0][j])) j = 0;
            current = nodes[current].l * (input[0][j] == 'L') + nodes[current].r * (input[0][j] == 'R');
        }
    }

    output2 = entries[0];
    for (int i = 1; i < entries_nb; i++) {
        output2 = output2/gcd(output2, entries[i]) * entries[i];
    }

    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}
