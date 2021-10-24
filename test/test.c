#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "channel.h"
#include "client.h"

static char* getToken()
{
#ifdef _WIN32
    char *token;
    size_t len;
    errno_t err = _dupenv_s(&token, &len, "HOLODEX_TOKEN");
    if (err || token == NULL)
    {
        fprintf(stderr, "Token not found in environment");
        return NULL;
    }
    return token;
#else
    char* token = getenv("HOLODEX_TOKEN");
    if (token == NULL)
    {
        fprintf(stderr, "Token not found in environment");
        return NULL;
    }
    return token;
#endif
}

static void testChannel(void)
{
    channel* ch = colodex_get_channel("UCsUj0dszADCGbF3gNrQEuSQ");
    assert(strcmp(ch->id, "UCsUj0dszADCGbF3gNrQEuSQ") == 0);
    assert(strcmp(ch->name, "Tsukumo Sana Ch. hololive-EN") == 0);
    assert(strcmp(ch->englishName, "Tsukumo Sana") == 0);
    assert(ch->type == VTUBER);
    assert(strcmp(ch->org, "Hololive") == 0);
    assert(strcmp(ch->suborg, "i English (Council)") == 0);
    assert(strcmp(ch->twitter, "tsukumosana") == 0);
    assert(ch->inactive == false);

    colodex_free_channel(ch);
}

int main()
{
    char* token = getToken();
    if (token == NULL)
    {
        return 1;
    }
    colodex_init(token);

    testChannel();

#ifdef _WIN32
    free(token); // getenv() shouldn't be freed
#endif
    colodex_free();
    printf("OK\n");
}