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
    int         songcount;
} video;

video* colodex_get_video_from_id(const char* videoId);
video** colodex_get_video_from_channel_id(const char* channelId);
void colodex_free_video(video* vid);
void colodex_free_videos(video** vids);