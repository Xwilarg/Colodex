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

video** colodex_get_video(const char* channelId)
{
    char* baseUrl = "https://holodex.net/api/v2/videos/";
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
        video* currVid = malloc(sizeof(video));

        currVid->id = parseString(json, "id");
        currVid->title = parseString(json, "title");
        currVid->type = parseVideoType(json, "type");
        currVid->topicId = parseString(json, "topic_id");
        currVid->publishedAt = parseDateTime(json, "published_at");
        currVid->availableAt = parseDateTime(json, "available_at");
        currVid->status = parseVideoStatus(json, "status");
        currVid->duration = parseInt(json, "duration");

        vid[i] = currVid;
        it = it->next;
    }
    vid[arrSize] = NULL;
    return vid;
}

void colodex_free_videos(video** vids)
{
    for (int i = 0; vids[i] != NULL; i++)
    {
        video* currVid = vids[i];
        free(currVid->id);
        free(currVid->title);
        free(currVid->topicId);
        free(currVid);
    }
    free(vids);
}