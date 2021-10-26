#define _ITERATOR_DEBUG_LEVEL 0

#include <gtest/gtest.h>

extern "C" {
    #include "channel.h"
    #include "video.h"
    #include "client.h"
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
    EXPECT_EQ("UCsUj0dszADCGbF3gNrQEuSQ", std::string(ch->id));
    EXPECT_EQ("Tsukumo Sana Ch. hololive-EN", std::string(ch->name));
    EXPECT_EQ("Tsukumo Sana", std::string(ch->englishName));
    EXPECT_EQ(VTUBER, ch->type);
    EXPECT_EQ("Hololive", std::string(ch->org));
    EXPECT_EQ("i English (Council)", std::string(ch->suborg));
    EXPECT_EQ("tsukumosana", std::string(ch->twitter));
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
    EXPECT_EQ("-AuQZrUHjhg", std::string(vid->id));
    EXPECT_EQ("[MV] Red - Calliope Mori #HololiveEnglish #HoloMyth", std::string(vid->title));
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_EQ("Original_Song", std::string(vid->topicId));
    EXPECT_EQ(1617539414, vid->publishedAt);
    EXPECT_EQ(1617539414, vid->availableAt);
    EXPECT_EQ(223, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(1, vid->songcount);
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
    EXPECT_EQ("Lm1k8TI790Y", std::string(vid->id));
    EXPECT_EQ("【R6S】私にかかれば負けnあ… ーRainbow Six Siege【獅白ぼたん/ホロライブ】", std::string(vid->title));
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_EQ("Rainbow_Six", std::string(vid->topicId));
    EXPECT_EQ(1600052402, vid->publishedAt);
    EXPECT_EQ(1600052402, vid->availableAt);
    EXPECT_EQ(701, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(0, vid->songcount);
    colodex_free_video(vid);

#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}