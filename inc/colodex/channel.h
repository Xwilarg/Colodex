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
    char*       description;
    char*       photo;
    char*       thumbnail;
    char*       banner;
    char*       org;
    char*       suborg;
    char*       lang;
    time_t      publishedAt;
    int         viewCount;
    int         videoCount;
    int         subscriberCount;
    time_t      commentsCrawledAt;
    time_t      updatedAt;
    char*       ytUploadsId;
    time_t      crawledAt;
    channelType type;
    int         clipCount;
    char*       twitter;
    bool        inactive;
    time_t      createdAt;
    char**      topTopics;
} channel;

channel* colodex_get_channel(const char* channelId);
void colodex_free_channel(channel* ch);