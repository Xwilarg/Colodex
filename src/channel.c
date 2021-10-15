#include <string.h>
#include <stdlib.h>

#include "channel.h"
#include "client.h"
#include "utils.h"

channel* colodex_get_channel(const char* channelId)
{
    char* baseUrl = "https://holodex.net/api/v2/channels/";
    size_t size = strlen(baseUrl) + strlen(channelId) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, channelId);
    char* resp = request(url);
    cJSON* json = cJSON_Parse(resp);
    free(resp);
    free(url);

    channel* ch;
    ch = malloc(sizeof(channel));
    if (ch == NULL)
    {
        return NULL;
    }

    ch->id = parseString(json, "id");
    ch->name = parseString(json, "name");
    ch->englishName = parseString(json, "english_name");
    ch->description = parseString(json, "description");
    ch->photo = parseString(json, "photo");
    ch->thumbnail = parseString(json, "thumbnail");
    ch->banner = parseString(json, "banner");
    ch->org = parseString(json, "org");
    ch->suborg = parseString(json, "suborg");
    ch->lang = parseString(json, "lang");
    ch->publishedAt = parseDateTime(json, "published_at");
    ch->viewCount = parseInt(json, "view_count");
    ch->videoCount = parseInt(json, "video_count");
    ch->subscriberCount = parseInt(json, "subscriber_count");
    ch->commentsCrawledAt = parseDateTime(json, "comments_crawled_at");
    ch->updatedAt = parseDateTime(json, "updated_at");
    ch->ytUploadsId = parseString(json, "yt_uploads_id");
    ch->crawledAt = parseDateTime(json, "crawled_at");
    ch->type = strcmp(cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring, "vtuber") == 0 ? VTUBER : SUBBER;
    ch->clipCount = parseInt(json, "clip_count");
    ch->twitter = parseString(json, "twitter");
    ch->inactive = parseBool(json, "inactive");
    ch->createdAt = parseDateTime(json, "created_at");
    ch->topTopics = parseStringArray(json, "top_topics");

    cJSON_Delete(json);

    return ch;
}

void colodex_free_channel(channel* ch)
{
    free(ch->id);
    free(ch->name);
    free(ch->englishName);
    free(ch->description);
    free(ch->photo);
    free(ch->thumbnail);
    free(ch->banner);
    free(ch->org);
    free(ch->suborg);
    free(ch->lang);
    free(ch->ytUploadsId);
    free(ch->twitter);
    for (int i = 0; ch->topTopics[i] != NULL; i++)
    {
        free(ch->topTopics[i]);
    }
    free(ch->topTopics);
    free(ch);
}