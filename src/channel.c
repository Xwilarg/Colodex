#include <string.h>
#include <stdlib.h>

#include "channel.h"
#include "client.h"
#include "utils.h"

static channel_type parse_channel_type(const cJSON* json, const char* name)
{
    char* value = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
    if (strcmp(value, "vtuber") == 0) return VTUBER;
    if (strcmp(value, "subber") == 0) return SUBBER;
    fprintf(stderr, "Unknown channel type %s\n", value);
    return -1;
}

channel* colodex_get_channel(const char* channel_id)
{
    char* baseUrl = "https://holodex.net/api/v2/channels/";
    size_t size = strlen(baseUrl) + strlen(channel_id) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, channel_id);
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

    ch->id = parse_string(json, "id");
    ch->name = parse_string(json, "name");
    ch->english_name = parse_string(json, "english_name");
    ch->description = parse_string(json, "description");
    ch->photo = parse_string(json, "photo");
    ch->thumbnail = parse_string(json, "thumbnail");
    ch->banner = parse_string(json, "banner");
    ch->org = parse_string(json, "org");
    ch->suborg = parse_string(json, "suborg");
    ch->lang = parse_string(json, "lang");
    ch->published_at = parse_date_time(json, "published_at");
    ch->view_count = parse_int(json, "view_count");
    ch->video_count = parse_int(json, "video_count");
    ch->subscriber_count = parse_int(json, "subscriber_count");
    ch->comments_crawled_at = parse_date_time(json, "comments_crawled_at");
    ch->updated_at = parse_date_time(json, "updated_at");
    ch->yt_uploads_id = parse_string(json, "yt_uploads_id");
    ch->crawled_at = parse_date_time(json, "crawled_at");
    ch->type = parse_channel_type(json, "type");
    ch->clip_count = parse_int(json, "clip_count");
    ch->twitter = parse_string(json, "twitter");
    ch->inactive = parse_bool(json, "inactive");
    ch->created_at = parse_date_time(json, "created_at");
    ch->top_topics = parse_string_array(json, "top_topics");

    cJSON_Delete(json);

    return ch;
}

void colodex_free_channel(channel* ch)
{
    free(ch->id);
    free(ch->name);
    free(ch->english_name);
    free(ch->description);
    free(ch->photo);
    free(ch->thumbnail);
    free(ch->banner);
    free(ch->org);
    free(ch->suborg);
    free(ch->lang);
    free(ch->yt_uploads_id);
    free(ch->twitter);
    for (int i = 0; ch->top_topics[i] != NULL; i++)
    {
        free(ch->top_topics[i]);
    }
    free(ch->top_topics);
    free(ch);
}