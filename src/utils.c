#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "internal/utils.h"

static char* malloc_and_copy(const char* input)
{
    if (input == NULL)
    {
        return NULL;
    }

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

char* parse_string(const cJSON* json, char* name)
{
    cJSON* elem = cJSON_GetObjectItemCaseSensitive(json, name);
    if (elem == NULL)
    {
        return NULL;
    }
    return malloc_and_copy(elem->valuestring);
}

int parse_int(const cJSON* json, char* name)
{
    cJSON* elem = cJSON_GetObjectItemCaseSensitive(json, name);
    if (elem == NULL)
    {
        return 0;
    }
    return elem->valueint;
}

time_t parse_date_time(const cJSON* json, char* name)
{
    struct tm time;
    memset(&time, 0, sizeof(struct tm));
    char* timeStr = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
#ifdef _WIN32
    sscanf_s(timeStr, "%d-%d-%dT%d:%d:%d",
        &time.tm_year,
        &time.tm_mon,
        &time.tm_mday,
        &time.tm_hour,
        &time.tm_min,
        &time.tm_sec
    );
    time.tm_year -= 1900;
    time.tm_mon--;
    return _mkgmtime(&time);
#else
    strptime(timeStr, "%Y-%m-%dT%H:%M:%S", &time);
    return timegm(&time);
#endif
}

bool parse_bool(const cJSON* json, char* name)
{
    return cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(json, name));
}

char** parse_string_array(const cJSON* json, char* name)
{
    cJSON* array = cJSON_GetObjectItemCaseSensitive(json, name);
    size_t arrSize = cJSON_GetArraySize(array);
    char** result = malloc(sizeof(char*) * (arrSize + 1));
    if (result == NULL)
    {
        return NULL;
    }
    cJSON* it = array->child;
    for (size_t i = 0; i < arrSize; i++)
    {
        result[i] = malloc_and_copy(it->valuestring);
        it = it->next;
    }
    result[arrSize] = NULL; // Last element is a NULL
    return result;
}