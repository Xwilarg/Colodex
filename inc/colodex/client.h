#pragma once

#include <curl/curl.h>
#include <stdbool.h>

bool init(const char* apiKey);
char* request(const char* url);