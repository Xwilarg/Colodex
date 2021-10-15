# Colodex
Library to use Holodex in C

## Dependencies
[curl](https://github.com/curl/curl) 7.78.0 \
[cJSON](https://github.com/DaveGamble/cJSON) 1.7.15

## Install and build
```
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
```

## Example
```c
#include <stdio.h> // printf
#include "channel.h"
#include "client.h"

int main()
{
    colodex_init(/* YOUR HOLODEX TOKEN */);
    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    printf("The twitter username of %s from %s is %s\n", ch->englishName, ch->org, ch->twitter);
    colodex_free_channel(ch);
    colodex_free();
}
```
