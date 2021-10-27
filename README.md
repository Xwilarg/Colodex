# Colodex
Library to use [Holodex](https://holodex.net/) in C

## Dependencies
[curl](https://github.com/curl/curl) 7.78.0 \
[cJSON](https://github.com/DaveGamble/cJSON) 1.7.15

## Install and build
To install and build Colodex, you'll need [Conan](https://conan.io/downloads.html) and [cmake](https://cmake.org/download/)
```
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build . --config Release
```

## Example
### Retrieve information about a channel from its ID
```c
#include <stdio.h> // printf
#include "colodex/channel.h"
#include "colodex/client.h"

int main()
{
    colodex_init(/* YOUR HOLODEX TOKEN */); // https://holodex.net/login

    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    printf("The twitter ID of %s from %s is %s\n", ch->english_name, ch->org, ch->twitter);
    
    // Don't forget to clean!
    colodex_free_channel(ch);
    colodex_free();
}
```
Expected output:
```
The twitter ID of Tsukumo Sana from Hololive is tsukumosana
```

### Get upcoming streams
```c
#include <stdio.h> // printf
#include <time.h> // localtime and asctime
#include <string.h> // strtok
#include "colodex/channel.h"
#include "colodex/client.h"

int main()
{
    colodex_init(/* YOUR HOLODEX TOKEN */); // https://holodex.net/login

    query_video* query = new query_video();
    query->status = UPCOMING; // Only get upcoming videos
    query->limit = 5; // Only get 10 videos
    video** vids = colodex_get_videos(query, (query_video_param)(STATUS | LIMIT));

    for (video **it = vids; *it != NULL; it++)
    {
        struct tm* tm = localtime(&(*it)->available_at);
        printf("Upcoming stream from %s at %s: %s\n", (*it)->channel_info->english_name, strtok(asctime(tm), "\n"), (*it)->title);
    }

    // Don't forget to clean!
    colodex_free_videos(vids);
    colodex_free();
}
```
Sample output:
```
Upcoming stream from Planya at Sun Aug 11 13:05:00 2024: Стрим из душа.
Upcoming stream from Kami Neko at Wed Jul 31 17:00:00 2024: 👑Free chat👑
Upcoming stream from Utatane Nasa at Tue Jul 30 17:00:00 2024: 【今週のスケジュール&フリートーク】
Upcoming stream from Shiotenshi Rieru at Sun Jun 30 17:00:00 2024: 随時更新Twitterタグ #次の正座待機 アーカイブはほぼ残りません
Upcoming stream from Yumeoi Kakeru at Fri Jun 28 13:00:00 2024: 夢追翔のフリーチャット【ふりーちゃっと / FreeChat】
```
