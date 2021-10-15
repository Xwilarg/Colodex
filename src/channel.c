#define _XOPEN_SOURCE

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

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
    ch = malloc(sizeof(channel));
    if (ch == NULL)
    {
        return NULL;
    }
    ch->id = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "id")->valuestring);
    ch->name = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    ch->englishName = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "english_name")->valuestring);

    char* chanType = cJSON_GetObjectItemCaseSensitive(json, "english_name")->valuestring;
    if (strcmp(chanType, "vtuber"))
    {
        ch->type = VTUBER;
    }
    else
    {
        ch->type = SUBBER;
    }

    ch->org = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "org")->valuestring);
    ch->suborg = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "suborg")->valuestring);
    ch->photo = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "photo")->valuestring);
    ch->banner = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "banner")->valuestring);
    ch->twitter = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "twitter")->valuestring);
    ch->videoCount = cJSON_GetObjectItemCaseSensitive(json, "video_count")->valueint;
    ch->subscriberCount = cJSON_GetObjectItemCaseSensitive(json, "subscriber_count")->valueint;
    ch->viewCount = cJSON_GetObjectItemCaseSensitive(json, "view_count")->valueint;
    ch->clipCount = cJSON_GetObjectItemCaseSensitive(json, "clip_count")->valueint;
    ch->lang = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "lang")->valuestring);

    struct tm time;
    char* timeStr = cJSON_GetObjectItemCaseSensitive(json, "published_at")->valuestring;
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
#else
    strptime(timeStr, "%Y-%m-%dT%H:%M:%S", &time);
#endif
    ch->publishedAt = mktime(&time);

    ch->inactive = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(json, "inactive"));
    ch->description = malloc_and_copy(cJSON_GetObjectItemCaseSensitive(json, "description")->valuestring);

    cJSON_Delete(json);

    return ch;
}