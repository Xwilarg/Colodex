#pragma once

#include <cjson/cJSON.h>

char* parseString(const cJSON* json, char* name);
int parseInt(const cJSON* json, char* name);
time_t parseDateTime(const cJSON* json, char* name);
bool parseBool(const cJSON* json, char* name);