#include <cjson/cJSON.h>

#include "channel.h"
#include "client.h"
#include "utils.h"

channel* get_channel(const char* channel_id)
{
    char* baseUrl = "https://holodex.net/api/v2/channels/";
    int size = sizeof(baseUrl) + sizeof(channel_id) - 1;
    char* url = malloc(size);
#ifdef _WIN32
    strcpy_s(url, size, channel_id);
#else
    strcpy(url, size, channel_id);
#endif
    cJSON* json = cJSON_Parse(request(url));

    channel* ch;
    ch = malloc(sizeof(ch));
    ch->id = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "id")->valuestring);

    return ch;
}