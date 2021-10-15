#include <cjson/cJSON.h>

#include "channel.h"
#include "client.h"
#include "utils.h"

channel* get_channel(const char* channel_id)
{
    char* baseUrl = "https://holodex.net/api/v2/channels/";
    int size = strlen(baseUrl) + strlen(channel_id) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
#ifdef _WIN32
    strcpy_s(url, size, baseUrl);
    strcat_s(url, size, channel_id);
#else
    strcpy(url, size, baseUrl);
    strcat(url, channel_id);
#endif
    cJSON* json = cJSON_Parse(request(url));

    channel* ch;
    ch = malloc(sizeof(ch));
    if (ch == NULL)
    {
        return NULL;
    }
    ch->id = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "id")->valuestring);

    return ch;
}