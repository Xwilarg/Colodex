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

video* colodex_get_video_from_id(const char* video_id);
video** colodex_get_video_from_channel_id(const char* channel_id);
void colodex_free_video(video* vid);
void colodex_free_videos(video** vids);