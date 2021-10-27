#include <string.h>
#include <stdlib.h>

#include "video.h"
#include "client.h"
#include "utils.h"

static video_type parse_video_type(const cJSON* json, const char* name)
{
    char* value = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
    if (strcmp(value, "stream") == 0) return STREAM;
    if (strcmp(value, "clip") == 0) return CLIP;
    fprintf(stderr, "Unknown video type %s\n", value);
    return -1;
}

static video_status parseVideoStatus(const cJSON* json, const char* name)
{
    char* value = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
    if (strcmp(value, "new") == 0) return NEW;
    if (strcmp(value, "upcoming") == 0) return UPCOMING;
    if (strcmp(value, "live") == 0) return LIVE;
    if (strcmp(value, "past") == 0) return PAST;
    if (strcmp(value, "missing") == 0) return MISSING;
    fprintf(stderr, "Unknown video status %s\n", value);
    return -1;
}

static video* parse_video(cJSON* json)
{
    video* vid = malloc(sizeof(video));
    if (vid == NULL)
    {
        return NULL;
    }

    vid->id = parse_string(json, "id");
    vid->title = parse_string(json, "title");
    vid->type = parse_video_type(json, "type");
    vid->topic_id = parse_string(json, "topic_id");
    vid->published_at = parse_date_time(json, "published_at");
    vid->available_at = parse_date_time(json, "available_at");
    vid->status = parse_video_status(json, "status");
    vid->duration = parseint(json, "duration");
    vid->songcount = parse_int(json, "songcount");
    // TODO: live_tl_count
    return vid;
}

video* colodex_get_video_from_id(const char* video_id)
{
    char* baseUrl = "https://holodex.net/api/v2/videos?id=";
    size_t size = strlen(baseUrl) + strlen(video_id) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, video_id);
    char* resp = request(url);
    cJSON* json = cJSON_Parse(resp);
    free(resp);
    free(url);

    size_t arrSize = cJSON_GetArraySize(json);

    if (arrSize == 0) // No video with this ID
    {
        return NULL;
    }

    video* vid;
    vid = malloc(sizeof(video));
    if (vid == NULL)
    {
        return NULL;
    }

    return parseVideo(json->child);
}

video** colodex_get_video_from_channel_id(const char* channel_id)
{
    char* baseUrl = "https://holodex.net/api/v2/videos?channel_id=";
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

    size_t arrSize = cJSON_GetArraySize(json);
    video** vid;
    vid = malloc(sizeof(video*) * (arrSize + 1));
    if (vid == NULL)
    {
        return NULL;
    }
    cJSON* it = json->child;
    for (size_t i = 0; i < arrSize; i++)
    {
        vid[i] = parseVideo(it);
        it = it->next;
    }
    vid[arrSize] = NULL;
    return vid;
}

void colodex_free_video(video* vid)
{
    free(vid->id);
    free(vid->title);
    free(vid->topic_id);
    free(vid);
}

void colodex_free_videos(video** vids)
{
    for (int i = 0; vids[i] != NULL; i++)
    {
        colodex_free_video(vids[i]);
    }
    free(vids);
}