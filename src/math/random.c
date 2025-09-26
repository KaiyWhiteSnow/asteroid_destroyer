#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// NOT WORKING
// Simple Linear Congruential Generator (LCG)
int seed = 123456789;  // initial seed 
int last_value = NULL;

int lcg(){
    int a = 1664525;
    int c = 1013904223;
    int m = __INT32_MAX__;
    seed = (a * seed + c) % m;
    return seed;
}


int next_number(int low, int high){
    while (1){
        // scale the LCG output into the desired range
        int value = low + (lcg() % (high - low + 1));
        if (value != last_value){
            last_value = value;
            return value;
        }
    }
}
