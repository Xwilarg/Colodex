#include <stdlib.h>
#include <string.h>

#include "utils.h"

char* malloc_and_copy(const char* input)
{
    char* output = malloc(sizeof(input));
    #ifdef _WIN32
        strcpy_s(output, sizeof(input), input);
    #else
        strcpy(output, sizeof(input), input);
    #endif
    return output;
}