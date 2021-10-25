#include <string.h>
#include <stdlib.h>

#include "video.h"
#include "client.h"
#include "utils.h"

static videoType parseVideoType(const cJSON* json, const char* name)
{
    char* value = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
    if (strcmp(value, "stream") == 0) return STREAM;
    if (strcmp(value, "clip") == 0) return CLIP;
    fprintf(stderr, "Unknown video type %s\n", value);
    return -1;
}

static videoStatus parseVideoStatus(const cJSON* json, const char* name)
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

static video* parseVideo(cJSON* json)
{
    video* vid = malloc(sizeof(video));
    if (vid == NULL)
    {
        return NULL;
    }

    vid->id = parseString(json, "id");
    vid->title = parseString(json, "title");
    vid->type = parseVideoType(json, "type");
    vid->topicId = parseString(json, "topic_id");
    vid->publishedAt = parseDateTime(json, "published_at");
    vid->availableAt = parseDateTime(json, "available_at");
    vid->status = parseVideoStatus(json, "status");
    vid->duration = parseInt(json, "duration");
    vid->songcount = parseInt(json, "songcount");
    // TODO: live_tl_count
    return vid;
}

video* colodex_get_video_from_id(const char* videoId)
{
    char* baseUrl = "https://holodex.net/api/v2/videos?id=";
    size_t size = strlen(baseUrl) + strlen(videoId) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, videoId);
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

video** colodex_get_video_from_channel_id(const char* channelId)
{
    char* baseUrl = "https://holodex.net/api/v2/videos?channel_id=";
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
    free(vid->topicId);
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