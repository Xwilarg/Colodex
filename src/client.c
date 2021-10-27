#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

static struct curl_slist *m_auth_header = NULL;

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

bool colodex_init(const char* api_key)
{
    m_auth_header = NULL; // In case we are calling the function twice

    // Set header
    size_t size = 11 + strlen(api_key);
    char* header = malloc(size); // 10 is sizeof "X-APIKEY: " + 1 for \0
    if (header == NULL)
    {
        fprintf(stderr, "Could not call malloc, out of memory");
        return false;
    }
    snprintf(header, size, "%s%s", "X-APIKEY: ", api_key);
    m_auth_header = curl_slist_append(m_auth_header, header);
    free(header);
    return true;
}

void colodex_free(void)
{
    curl_slist_free_all(m_auth_header);
}

char* request(const char* url)
{
    if (m_auth_header == NULL)
    {
        fprintf(stderr, "init(const char* apiKey) must be called first\n");
        return NULL;
    }

    // Create handle for request
    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, m_auth_header);

    struct memory chunk = {0};
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&chunk);

    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); // Fix issues with local certificate

    CURLcode code = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "An error happened while processing the request: %s\n", curl_easy_strerror(code));
        return NULL;
    }
    return chunk.response;
}