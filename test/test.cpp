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
    EXPECT_EQ("UCsUj0dszADCGbF3gNrQEuSQ", std::string(ch->id));
    EXPECT_EQ("Tsukumo Sana Ch. hololive-EN", std::string(ch->name));
    EXPECT_EQ("Tsukumo Sana", std::string(ch->english_name));
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

    video* vid = colodex_get_video_from_id("-AuQZrUHjhg", NULL, NONE);
    EXPECT_EQ("-AuQZrUHjhg", std::string(vid->id));
    EXPECT_EQ("[MV] Red - Calliope Mori #HololiveEnglish #HoloMyth", std::string(vid->title));
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_EQ("Original_Song", std::string(vid->topic_id));
    EXPECT_EQ(1617543014, vid->published_at);
    EXPECT_EQ(1617543014, vid->available_at);
    EXPECT_EQ(223, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(1, vid->songcount);
    EXPECT_EQ("UCL_qhgtOy0dy1Agp8vkySQg", std::string(vid->channel_info->id));
    EXPECT_EQ("Mori Calliope Ch. hololive-EN", std::string(vid->channel_info->name));
    EXPECT_EQ("Hololive", std::string(vid->channel_info->org));
    EXPECT_EQ(VTUBER, vid->channel_info->type);
    EXPECT_EQ("https://yt3.ggpht.com/ytc/AKedOLQi2hR9UdCcWoDLz4sJYqAu9BkaYBGWex_th5ic=s800-c-k-c0x00ffffff-no-rj-mo", std::string(vid->channel_info->photo));
    EXPECT_EQ("Mori Calliope", std::string(vid->channel_info->english_name));
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

    video* vid = colodex_get_video_from_id("Lm1k8TI790Y", NULL, NONE);
    EXPECT_EQ("Lm1k8TI790Y", std::string(vid->id));
    EXPECT_EQ("【R6S】私にかかれば負けnあ… ーRainbow Six Siege【獅白ぼたん/ホロライブ】", std::string(vid->title));
    EXPECT_EQ(STREAM, vid->type);
    EXPECT_EQ("Rainbow_Six", std::string(vid->topic_id));
    EXPECT_EQ(1600056002, vid->published_at);
    EXPECT_EQ(1600056002, vid->available_at);
    EXPECT_EQ(701, vid->duration);
    EXPECT_EQ(PAST, vid->status);
    EXPECT_EQ(0, vid->songcount);
    EXPECT_EQ("UCUKD-uaobj9jiqB-VXt71mA", std::string(vid->channel_info->id));
    EXPECT_EQ("Botan Ch.獅白ぼたん", std::string(vid->channel_info->name));
    EXPECT_EQ("Hololive", std::string(vid->channel_info->org));
    EXPECT_EQ(VTUBER, vid->channel_info->type);
    EXPECT_EQ("https://yt3.ggpht.com/ytc/AKedOLQXr6MeUpHI0-yNZIAaGXHvBVowhCWMwGx-zXYs=s800-c-k-c0x00ffffff-no-rj", std::string(vid->channel_info->photo));
    EXPECT_EQ("Shishiro Botan", std::string(vid->channel_info->english_name));
    colodex_free_video(vid);

#ifdef _WIN32
    free(token);
#endif
    colodex_free();
}