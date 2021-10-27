#pragma once

#include <time.h>

#include "channel.h"

typedef enum video_type {
    STREAM,
    CLIP
} video_type;

typedef enum video_status {
    NEW,
    UPCOMING,
    LIVE,
    PAST,
    MISSING
} video_status;

typedef enum order {
    ASCENDING,
    DESCENDING
} order;

typedef struct video {
    char*       id;
    char*       title;
    video_type  type;
    char*       topic_id;
    time_t      published_at;
    time_t      available_at;
    video_status status;
    int         duration;
    int         songcount;
    channel_min* channel_info;
} video;

typedef struct query_video {
    int         limit;
    int         max_upcoming_hours;
    int         offset;
    order       order;
    char*       org;
    video_status status;
    char*       topic;
    video_type  type;
} query_video;

typedef enum query_video_param {
    NONE                = 0,
    LIMIT               = 1,
    MAX_UPCOMING_HOURS  = 2,
    OFFSET              = 4,
    ORDER               = 8,
    ORG                 = 16,
    STATUS              = 32,
    TOPIC               = 64,
    TYPE                = 128
} query_video_param;

video* colodex_get_video_from_id(const char* video_id);
video** colodex_get_videos(const query_video* query, query_video_param params);
video** colodex_get_videos_from_channel_id(const char* channel_id, const query_video* query, query_video_param params);
void colodex_free_video(video* vid);
void colodex_free_videos(video** vids);