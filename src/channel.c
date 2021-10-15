#include <cjson/cJSON.h>

#include "channel.h"
#include "client.h"
#include "utils.h"

channel* get_channel(const char* channelId)
{
    char* baseUrl = "https://holodex.net/api/v2/channels/";
    size_t size = strlen(baseUrl) + strlen(channelId) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, channelId);
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