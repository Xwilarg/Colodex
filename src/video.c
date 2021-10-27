#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "colodex/video.h"
#include "internal/client.h"
#include "internal/utils.h"
#include "internal/channel.h"

static video_type parse_video_type(const cJSON* json, const char* name)
{
    char* value = cJSON_GetObjectItemCaseSensitive(json, name)->valuestring;
    if (strcmp(value, "stream") == 0) return STREAM;
    if (strcmp(value, "clip") == 0) return CLIP;
    fprintf(stderr, "Unknown video type %s\n", value);
    return -1;
}

static video_status parse_video_status(const cJSON* json, const char* name)
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
    vid->duration = parse_int(json, "duration");
    vid->songcount = parse_int(json, "songcount");
    // TODO: live_tl_count
    cJSON* chJson = cJSON_GetObjectItemCaseSensitive(json, "channel");
    vid->channel_info = parse_channel_min(chJson);
    return vid;
}

// Appends "append" and "appendArg" to "text"
// If text, will malloc it, else will realloc it
// Used to create an URL with arguments
static char* malloc_or_append(char* text, const char* append, const char* appendArg)
{
    size_t size;
    if (text == NULL)
    {
        size = strlen(append) + strlen(appendArg) + 1;
        text = malloc(size);
        snprintf(text, size, "%s%s", append, appendArg);
        return text;
    }
    size = strlen(text) + strlen(append) + strlen(appendArg) + 1;
    text = realloc(text, size);
#ifdef _WIN32
    strcat_s(text, size, append);
    strcat_s(text, size, appendArg);
#else
    strcat(text, append);
    strcat(text, appendArg);
#endif
    return text;
}

// Create parameter part of URL given query info
static char* create_url(const query_video* query, query_video_param params)
{
    if (query == NULL)
    {
        char* empty = malloc(1);
        empty[0] = '\0';
        return empty;
    }
    char* url = NULL;
    if ((params & MAX_UPCOMING_HOURS) != 0) // Remove video that come further than the giving hour
    {
        char buffer[10];
#ifdef _WIN32
        sprintf_s(buffer, 10, "%d", query->max_upcoming_hours);
#else
        sprintf(buffer, "%d", query->max_upcoming_hours);
#endif
        url = malloc_or_append(url, "&max_upcoming_hours=", buffer);
    }
    if ((params & STATUS) != 0) // Filter by video status
    {
        char* status;
        switch (query->status)
        {
            case NEW:
                status = "new";
                break;

            case UPCOMING:
                status = "upcoming";
                break;

            case LIVE:
                status = "live";
                break;

            case PAST:
                status = "past";
                break;

            case MISSING:
                status = "missing";
                break;

            default:
                fprintf(stderr, "Unknown video type %d\n", query->status);
                return NULL;
        }
        url = malloc_or_append(url, "&status=", status);
    }
    if ((params & LIMIT) != 0) // Limit the number of result
    {
        char buffer[10];
#ifdef _WIN32
        sprintf_s(buffer, 10, "%d", query->limit);
#else
        sprintf(buffer, "%d", query->limit);
#endif
        url = malloc_or_append(url, "&limit=", buffer);
    }
    return url;
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
        cJSON_Delete(json);
        return NULL;
    }

    video* vid = parse_video(json->child);
    cJSON_Delete(json);
    return vid;
}

// Internal method for parsing of multiple video
static video** get_videos(cJSON* json)
{
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
        vid[i] = parse_video(it);
        it = it->next;
    }
    vid[arrSize] = NULL;
    cJSON_Delete(json);

    return vid;
}

video** colodex_get_videos(const query_video* query, query_video_param params)
{
    char* baseUrl = "https://holodex.net/api/v2/videos";
    char* args = create_url(query, params);
    if(strlen(args) > 1) // There are query parameters
    {
        args[0] = '?'; // create_url put '&' in front parameter but this method need the first one to use a '?'
    }
    size_t size = strlen(baseUrl) + strlen(args) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s", baseUrl, args);
    char* resp = request(url);
    cJSON* json = cJSON_Parse(resp);
    free(resp);
    free(url);
    free(args);

    return get_videos(json);
}

video** colodex_get_videos_from_channel_id(const char* channel_id, const query_video* query, query_video_param params)
{
    char* baseUrl = "https://holodex.net/api/v2/videos?channel_id=";
    char* args = create_url(query, params);
    size_t size = strlen(baseUrl) + strlen(channel_id) + strlen(args) + 1;
    char* url = malloc(size);
    if (url == NULL)
    {
        return NULL;
    }
    snprintf(url, size, "%s%s%s", baseUrl, channel_id, args);
    char* resp = request(url);
    cJSON* json = cJSON_Parse(resp);
    free(resp);
    free(url);
    free(args);

    return get_videos(json);
}

void colodex_free_video(video* vid)
{
    free(vid->id);
    free(vid->title);
    free(vid->topic_id);
    free_channel_min(vid->channel_info);
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