#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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

bool eol(char c) {return !c || c == '\n' || c == '\r';}

#define MAX_LINE_NUMBER 300
#define MAX_LINE_SIZE 300

struct Range
{
    int64_t start, len;
    bool guard;
};

struct Range* ranges;
uint64_t capacity;
uint64_t length;

void pushback(int64_t start, int64_t len, bool guard)
{
    if (capacity == length) {
        capacity *= 2;
        ranges = realloc(ranges, capacity*sizeof(struct Range));
    }
    ranges[length].start = start;
    ranges[length].len = len;
    ranges[length].guard = guard;
    length++;
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

    // PART 1

    int64_t ids[20];
    char guards[20];

    int seed_nb = 0;

    char* ptr = input[0]+7;
    while(!eol(*ptr)) {
        ids[seed_nb] = strtoll(ptr, &ptr, 10);
        seed_nb++; ptr ++;
    }

    for (int i = 2; i < line_nb; i++)
    {
        ptr = input[i];
        if (*ptr > '9' || *ptr < '0'){
            if (*ptr < 'a') memset(guards, 0, seed_nb);
            continue;
        }
        int64_t dest = strtoll(ptr, &ptr, 10);
        int64_t src = strtoll(ptr+1, &ptr, 10);
        int64_t len = strtoll(ptr+1, &ptr, 10);
        for (int k = 0; k < seed_nb; k++)
        {
            if (guards[k]) continue;
            int64_t delta = ids[k] - src;
            if (delta >= 0 && delta < len) {
                ids[k] = dest + delta;
                guards[k] = 1;
            }
        }
    }

    output1 = ids[0];
    for (int k = 0; k < seed_nb; k++)
        output1 = output1 > ids[k] ? ids[k] : output1;
    
    clock_t end_point = clock();

    printf("PART 1 output : %"PRIu64"\n", output1);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    // PART 2
    /* i made a vector of ranges that get iterated over for each line of the input
     * then each range get split if there is an intersection with the range given by the input
     * each time a range is fully included in a range of the input, it is not ereased from the vector
     * instead its length just get nullified, as erasing an element of a vector is slow
     */

    start_point = clock();
    
    ranges = malloc(2*seed_nb*sizeof(struct Range));
    capacity = 2*seed_nb;
    length = seed_nb/2;

    ptr = input[0]+7;
    for(int i = 0; i < seed_nb/2; i++) {
        ranges[i].start = strtoll(ptr, &ptr, 10);
        ranges[i].len = strtoll(ptr+1, &ptr, 10);
    }

    for (int i = 1; i < line_nb; i++)
    {
        ptr = input[i];
        if (*ptr > '9' || *ptr < '0'){
            for (int k = 0; k < length; k++) ranges[k].guard = false;
            continue;
        }
        int64_t dest = strtoll(ptr, &ptr, 10);
        int64_t src = strtoll(ptr+1, &ptr, 10);
        int64_t len = strtoll(ptr+1, &ptr, 10);
        for (int k = 0; k < length; k++)
        {
            if (ranges[k].guard || ranges[k].len <= 0) continue;
            int64_t delta = ranges[k].start - src;

            if (delta >= 0 && delta < len)
            {
                int64_t new_start = dest + delta;
                int64_t new_len = len - delta <= ranges[k].len ? len - delta : ranges[k].len;
                ranges[k].start += new_len;
                ranges[k].len -= new_len;
                pushback(new_start, new_len, true);
            }

            if (delta < 0 && -delta < ranges[k].len)
            {
                if (src + len < ranges[k].start + ranges[k].len) {
                    pushback(dest, len, true);
                    pushback(src + len, ranges[k].len + delta - len, false);
                } else {
                    pushback(dest, ranges[k].len + delta, true);
                }
                ranges[k].len = -delta;
            }
        }
    }

    output2 = INT64_MAX;
    for (int k = 0; k < length; k++)
        if (ranges[k].len > 0 && ranges[k].start < output2) output2 = ranges[k].start;

    end_point = clock();

    printf("PART 2 output : %"PRIu64"\n", output2);
    printf("Time : %lf micro-seconds\n", (double)(end_point - start_point)/CLOCKS_PER_SEC*1000000.0);

    return 0;
}
