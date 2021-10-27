#define _ITERATOR_DEBUG_LEVEL 0

#include <gtest/gtest.h>

extern "C" {
    #include "colodex/channel.h"
    #include "colodex/video.h"
    #include "colodex/client.h"
}

static char* getToken()
{
#ifdef _WIN32
    char *token;
    size_t len;
    errno_t err = _dupenv_s(&token, &len, "HOLODEX_TOKEN");
    if (err || token == NULL)
    {
        throw std::runtime_error("Token not found in environment");
    }
    return token;
#else
    char* token = getenv("HOLODEX_TOKEN");
    if (token == NULL)
    {
        throw std::runtime_error("Token not found in environment");
    }
    return token;
#endif
}

TEST(ChannelTest, Basic)
{
    char* token = getToken();
    colodex_init(token);

    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    EXPECT_STREQ("UCsUj0dszADCGbF3gNrQEuSQ", ch->id);
    EXPECT_STREQ("Tsukumo Sana Ch. hololive-EN", ch->name);
    EXPECT_STREQ("Tsukumo Sana", ch->english_name);
    EXPECT_EQ(VTUBER, ch->type);
    EXPECT_STREQ("Hololive", ch->org);
    EXPECT_STREQ("i English (Council)", ch->suborg);
    EXPECT_STREQ("tsukumosana", ch->twitter);
    EXPECT_FALSE(ch->inactive);
    colodex_free_channel(ch);
    
#ifdef _WIN32
    free(token); // getenv() shouldn't be freed
#endif
    colodex_free();
}

TEST(VideoTest, Song)
{
    char* token = getToken();
    colodex_init(token);

    video* vid = colodex_get_video_from_id("-AuQZrUHjhg");
    EXPECT_STREQ("-AuQZrUHjhg", vid->id);
    EXPECT_STREQ("[MV] Red - Calliope Mori #HololiveEnglish #HoloMyth", vid->title);
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_STREQ("Original_Song", vid->topic_id);
    EXPECT_EQ(1617543014, vid->published_at);
    EXPECT_EQ(1617543014, vid->available_at);
    EXPECT_EQ(223, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(1, vid->songcount);
    EXPECT_STREQ("UCL_qhgtOy0dy1Agp8vkySQg", vid->channel_info->id);
    EXPECT_STREQ("Mori Calliope Ch. hololive-EN", vid->channel_info->name);
    EXPECT_STREQ("Hololive", vid->channel_info->org);
    EXPECT_EQ(VTUBER, vid->channel_info->type);
    EXPECT_STREQ("https://yt3.ggpht.com/ytc/AKedOLQi2hR9UdCcWoDLz4sJYqAu9BkaYBGWex_th5ic=s800-c-k-c0x00ffffff-no-rj-mo", vid->channel_info->photo);
    EXPECT_STREQ("Mori Calliope", vid->channel_info->english_name);
    colodex_free_video(vid);
    
#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}

TEST(VideoTest, StreamWithUnicode)
{
    char* token = getToken();
    colodex_init(token);

    video* vid = colodex_get_video_from_id("Lm1k8TI790Y");
    EXPECT_STREQ("Lm1k8TI790Y", vid->id);
    EXPECT_STREQ("【R6S】私にかかれば負けnあ… ーRainbow Six Siege【獅白ぼたん/ホロライブ】", vid->title);
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_STREQ("Rainbow_Six", vid->topic_id);
    EXPECT_EQ(1600056002, vid->published_at);
    EXPECT_EQ(1600056002, vid->available_at);
    EXPECT_EQ(701, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(0, vid->songcount);
    EXPECT_STREQ("UCUKD-uaobj9jiqB-VXt71mA", vid->channel_info->id);
    EXPECT_STREQ("Botan Ch.獅白ぼたん", vid->channel_info->name);
    EXPECT_STREQ("Hololive", vid->channel_info->org);
    EXPECT_EQ(VTUBER, vid->channel_info->type);
    EXPECT_STREQ("https://yt3.ggpht.com/ytc/AKedOLQXr6MeUpHI0-yNZIAaGXHvBVowhCWMwGx-zXYs=s800-c-k-c0x00ffffff-no-rj", vid->channel_info->photo);
    EXPECT_STREQ("Shishiro Botan", vid->channel_info->english_name);
    colodex_free_video(vid);

#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}

TEST(VideoTest, StreamWithInvalidId)
{
    char* token = getToken();
    colodex_init(token);

    video* vid = colodex_get_video_from_id("0000000000000000");
    EXPECT_EQ(nullptr, vid);

#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}


TEST(VideosTest, OnlyUpcomingStreams)
{
    char* token = getToken();
    colodex_init(token);

    query_video* query = new query_video();
    query->status = UPCOMING;
    video** vids = colodex_get_videos_from_channel_id("UCLhUvJ_wO9hOvv_yYENu4fQ", query, STATUS);
    delete query;

    int index = 0;
    video* curr = vids[0];
    while (curr != NULL)
    {
        EXPECT_EQ(UPCOMING, curr->status);
        curr = vids[++index];
    }
    colodex_free_videos(vids);

#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}

// These tests might be redundant with the others but we want to be sure the examples in the README run well!

TEST(Readme, GetChannel)
{
    char* token = getToken();
    colodex_init(token);

    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    EXPECT_EQ(
        "The twitter ID of Tsukumo Sana from Hololive is tsukumosana\n",
        "The twitter ID of " + std::string(ch->english_name) + " from " + std::string(ch->org) + " is " + std::string(ch->twitter) + "\n"
    );
    colodex_free_channel(ch);

#ifdef _WIN32
    free(token); // getenv() shouldn't be freed
#endif
    colodex_free();
}

TEST(Readme, GetUpcomingStreams)
{
    char* token = getToken();
    colodex_init(token);

    query_video* query = new query_video();
    query->status = UPCOMING;
    query->limit = 10;
    video** vids = colodex_get_videos(query, (query_video_param)(STATUS | LIMIT));
    for (video **it = vids; *it != NULL; it++)
    {
        EXPECT_EQ(UPCOMING, (*it)->status);
        // Can't really check much here :(
    }
    colodex_free_videos(vids);

#ifdef _WIN32
    free(token); // getenv() shouldn't be freed
#endif
    colodex_free();
}