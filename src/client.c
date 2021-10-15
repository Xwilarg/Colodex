#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

static struct curl_slist *m_authHeader = NULL;

struct memory {
   char *response;
   size_t size;
};

// From https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory*)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
    {
        fprintf(stderr, "Could not call malloc, out of memory");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

bool init(const char* apiKey)
{
    m_authHeader = NULL; // In case we are calling the function twice

    // Set header
    size_t size = 11 + strlen(apiKey);
    char* header = malloc(size); // 10 is sizeof "X-APIKEY: " + 1 for \0
    if (header == NULL)
    {
        fprintf(stderr, "Could not call malloc, out of memory");
        return false;
    }
    snprintf(header, size, "%s%s", "X-APIKEY: ", apiKey);
    m_authHeader = curl_slist_append(m_authHeader, header);
    return true;
}

char* request(const char* url)
{
    if (m_authHeader == NULL)
    {
        fprintf(stderr, "init(const char* apiKey) must be called first\n");
        return NULL;
    }

    // Create handle for request
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, m_authHeader);

    struct memory chunk = {0};
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&chunk);

    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); // Fix issues with local certificate

    CURLcode code = curl_easy_perform(handle);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "An error happened while processing the request: %s\n", curl_easy_strerror(code));
        return NULL;
    }
    return chunk.response;
}