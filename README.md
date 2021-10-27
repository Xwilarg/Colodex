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
#include "colodex/video.h"
#include "colodex/client.h"

int main()
{
    colodex_init(/* YOUR HOLODEX TOKEN */); // https://holodex.net/login

    query_video* query = (query_video*)malloc(sizeof(query_video));
    query->status = UPCOMING; // Only get upcoming videos
    query->type = STREAM; // Don't take the clippers, only the streamers
    query->order = ASCENDING; // Order with the most recent first
    query->limit = 5; // Only get 5 videos
    video** vids = colodex_get_videos(query, (query_video_param)(STATUS | TYPE | ORDER | LIMIT));
    free(query);

    for (video **it = vids; *it != NULL; it++)
    {
        struct tm* tm = localtime(&(*it)->available_at);
        printf("Upcoming stream from %s at %s: %s\n", (*it)->channel_info->name, strtok(asctime(tm), "\n"), (*it)->title);
    }

    // Don't forget to clean!
    colodex_free_videos(vids);
    colodex_free();
}
```
Sample output:
```
Upcoming stream from 天川はの/AmakawaHano at Wed Oct 27 23:05:00 2021: 𓊆 朝活 𓊇 17日目 💪筋トレが輝く朝 𓊆 天川はの￤Vtuber 𓊇
Upcoming stream from Walfie at Wed Oct 27 23:30:00 2021: Inside
Upcoming stream from ヒナミソラ - Hina Misora【WACTOR】 at Thu Oct 28 00:00:00 2021: Responder 30 preguntas【WACTOR/#ヒナミソラ】
Upcoming stream from kson ONAIR at Thu Oct 28 00:00:00 2021: 【GMMF🌎】元気に朝を迎える!10/28のさわやかNEWSをどうぞ! #GMMF #早朝から総長
Upcoming stream from Watson Amelia Ch. hololive-EN at Thu Oct 28 00:00:00 2021: 【Little Nightmare 2】SpoOoOOoky Game~
```
