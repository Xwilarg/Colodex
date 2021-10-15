#pragma once

#include <curl/curl.h>
#include <stdbool.h>

bool init_colodex(const char* apiKey);
void free_colodex(void);
char* request(const char* url);