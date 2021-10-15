#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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
    channel* ch = get_channel("UCUKD-uaobj9jiqB-VXt71mA");
    printf("%s\n", ch->id);
    free(ch);
    free(token);
}