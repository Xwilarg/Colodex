#include <stdlib.h>
#include <string.h>

#include "utils.h"

char* malloc_and_copy(const char* input)
{
    size_t size = strlen(input) + 1;
    char* output = malloc(size);
    if (output == NULL)
    {
        return NULL;
    }
    #ifdef _WIN32
        strcpy_s(output, size, input);
    #else
        strcpy(output, input);
    #endif
    return output;
}