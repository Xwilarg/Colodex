#pragma once

#include <cjson/cJSON.h>
#include "colodex/channel.h"

channel_min* parse_channel_min(cJSON* json);
void free_channel_min(channel_min* ch);