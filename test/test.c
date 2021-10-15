#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "channel.h"
#include "client.h"

int main()
{
#ifdef _WIN32
    char *token;
    size_t len;
    errno_t err = _dupenv_s(&token, &len, "HOLODEX_TOKEN");
    if (err || token == NULL)
    {
        fprintf(stderr, "Token not found in environment");
        return 1;
    }
#else
    char* token = getenv("HOLODEX_TOKEN");
    if (token == NULL)
    {
        fprintf(stderr, "Token not found in environment");
        return 1;
    }
#endif
    init(token);
    channel* ch = get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    assert(strcmp(ch->id, "UCsUj0dszADCGbF3gNrQEuSQ") == 0);
    assert(strcmp(ch->name, "Tsukumo Sana Ch. hololive-EN") == 0);
    assert(strcmp(ch->englishName, "Tsukumo Sana") == 0);
    assert(ch->type == VTUBER);
    assert(strcmp(ch->org, "Hololive") == 0);
    assert(strcmp(ch->suborg, "i English (Council)") == 0);
    assert(strcmp(ch->twitter, "tsukumosana") == 0);
    assert(ch->inactive == false);
    free_channel(ch);
#ifdef _WIN32
    free(token); // getenv() shouldn't be freed
#endif
    printf("OK\n");
}