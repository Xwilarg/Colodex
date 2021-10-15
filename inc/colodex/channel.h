#pragma once

#include <time.h>
#include <stdbool.h>

typedef enum channelType {
    VTUBER,
    SUBBER
} channelType;

typedef struct channel {
    char*       id;
    char*       name;
    char*       englishName;
    channelType type;
    char*       org;
    char*       suborg;
    char*       photo;
    char*       banner;
    char*       twitter;
    int         videoCount;
    int         subscriberCount;
    int         viewCount;
    int         clipCount;
    char*       lang;
    time_t      publishedAt;
    bool        inactive;
    char*       description;
} channel;

channel* get_channel(const char* channel_id);