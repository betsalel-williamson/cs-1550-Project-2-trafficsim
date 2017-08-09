//
// Created by School on 6/8/16.
//

#include <cstdio>
#include "mother.h"

void mother::print_buffer(unsigned short * buffer, int buffer_size) {
    int i;

    // printf("0x"); -- uncomment if you want to start with "0x"

    for (i = 0; i < buffer_size; i++) {
        printf("%02x", (unsigned int) (buffer[i]));
        if (i % 4 == 3) printf(" ");    // groups of 8: makes more readable
        // uncomment if you want "all one line"
        if (i % 32 == 31) printf("\n"); // ditto
    }

    printf("\n");
}

