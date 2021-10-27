#pragma once

#include <time.h>
#include <stdbool.h>

typedef enum channel_type {
    VTUBER,
    SUBBER
} channel_type;

typedef struct channel {
    char*       id;
    char*       name;
    char*       english_name;
    char*       description;
    char*       photo;
    char*       thumbnail;
    char*       banner;
    char*       org;
    char*       suborg;
    char*       lang;
    time_t      published_at;
    int         view_count;
    int         video_count;
    int         subscriber_count;
    time_t      comments_crawled_at;
    time_t      updated_at;
    char*       yt_uploads_id;
    time_t      crawled_at;
    channel_type type;
    int         clip_count;
    char*       twitter;
    bool        inactive;
    time_t      created_at;
    char**      top_topics;
} channel;

channel* colodex_get_channel(const char* channel_id);
void colodex_free_channel(channel* ch);