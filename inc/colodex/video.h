#pragma once

#include <time.h>

typedef enum videoType {
    STREAM,
    CLIP
} videoType;

typedef enum videoStatus {
    NEW,
    UPCOMING,
    LIVE,
    PAST,
    MISSING
} videoStatus;

typedef struct video {
    char*       id;
    char*       title;
    videoType   type;
    char*       topicId;
    time_t      publishedAt;
    time_t      availableAt;
    videoStatus status;
    int         duration;
} video;

video** colodex_query_videos(const char* channelId);
void colodex_free_videos(video** vids);