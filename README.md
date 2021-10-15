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
cmake --build .
```

## Example
### Retrieve information about a channel from its ID
```c
#include <stdio.h> // printf
#include "channel.h"
#include "client.h"

int main()
{
    colodex_init(/* YOUR HOLODEX TOKEN */); // https://holodex.net/login

    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    printf("The twitter ID of %s from %s is %s\n", ch->englishName, ch->org, ch->twitter);
    
    // Don't forget to clean!
    colodex_free_channel(ch);
    colodex_free();
}
```
Expected output:
```
The twitter ID of Tsukumo Sana from Hololive is tsukumosana
```
