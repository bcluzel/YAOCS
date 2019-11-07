#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void exit_if(int condition, const char *prefix)
{
    if (condition) {
        perror(prefix);
        exit(1);
    }
}

void int_to_four_char(unsigned int n, char *bytes){
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

unsigned int four_char_to_int(char *bytes){
    unsigned int n;
    n = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3] << 0;
    return n;
}



