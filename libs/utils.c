#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exit_if(int condition, const char *prefix)
{
    if (condition) {
        perror(prefix);
        exit(1);
    }
}