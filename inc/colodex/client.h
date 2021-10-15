#pragma once

#include <curl/curl.h>
#include <stdbool.h>

bool colodex_init(const char* apiKey);
void colodex_free(void);
char* request(const char* url);