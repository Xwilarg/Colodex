#pragma once

#include <cjson/cJSON.h>

char* parse_string(const cJSON* json, char* name);
int parse_int(const cJSON* json, char* name);
time_t parse_date_time(const cJSON* json, char* name);
bool parse_bool(const cJSON* json, char* name);
char** parse_string_array(const cJSON* json, char* name);