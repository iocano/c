/*
    Program:
        Search a term in RSS FEED and print its URL
        Example of libcurl, libexpat, regex
    Compile:
        gcc main.c -lcurl -lexpat -o feeds_finder
    -------------------------------------
    Run:
        ./feeds_finder
    NEED:
        export RSS Feed URL to RSS_FEED_URL enviroment variable
    Example:
        export RSS_FEED_URL="http://feeds.bbci.co.uk/mundo/rss.xml"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include <curl/curl.h>
#include <expat.h>

typedef struct tag_content
{
    char *content;
    size_t content_length;
}tag_content_t;

typedef struct user_data
{
    bool is_proccess_ok;
    bool is_search_pattern_found;
    char *search_pattern;
    size_t tag_count;
    size_t tag_depth;
    tag_content_t tag;
}user_data_t;

bool is_match(char *pattern, const char *string, bool ignore_case);

static void tag_opening_handler(void *user_data, const XML_Char *tag_name, const XML_Char **atts);

/*
   Character data handler is called for every piece of a text in the XML document.
   It can be called multiple times inside each fragment (e.g. for non-ASCII strings).
   Content string received is NOT zero terminated. Use the length argument to deal with the end of the string
*/
static void tag_character_data_handler(void *user_data, const XML_Char *content, int content_length);

static void tag_closing_handler(void *user_data, const XML_Char *tag_name);

/*
   This callback function gets called by libcurl as soon as there is data received that needs to be saved.
   For most transfers, this callback gets called many times and each invoke delivers another chunk of data.
    Parameters:
        content_chunk:  data received by curl
        size:           always 1
        nmemb:          content_chunk size
        userp:          used to shared data between calls
                        (user_data_t variable is associated to XML_Parser variable with XML_SetUserData function on main)
*/
static size_t parse_stream_callback(void *content_chunk, size_t content_chunk_length, size_t nmemb, void *userp);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s SEARCH_PATTERN\n", argv[0]);
        exit(1);
    }
    const char *RSS_FEED_URL = getenv("RSS_FEED_URL");
    if(!RSS_FEED_URL)
    {
        fprintf(stderr, "Set RSS_FEED_URL environment variable first.\nE.g. export RSS_FEED_URL=\"http://feeds.bbci.co.uk/mundo/rss.xml\"\n");
        exit(1);

    }

    puts("--------------------------------------------------------------------------------");
    puts(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    puts(RSS_FEED_URL);
    puts(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

    CURL *curl_handle;
    CURLcode res;
    XML_Parser parser;
    user_data_t user_data;

    /* Initialize the state structure for parsing. */
    memset(&user_data, 0, sizeof(user_data_t));
    user_data.is_proccess_ok = true;
    user_data.search_pattern = argv[1];

    /* Initialize a namespace-aware parser. */
    parser = XML_ParserCreateNS(NULL, '\0');
    // Associate parser with state, used to pass information between handlers
    XML_SetUserData(parser, &user_data);
    // set handlers for start and end tags
    XML_SetElementHandler(parser, tag_opening_handler, tag_closing_handler);
    // set handler for text
    XML_SetCharacterDataHandler(parser, tag_character_data_handler);

    /* Initialize a libcurl handle. */
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, RSS_FEED_URL);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, parse_stream_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, parser);

    /* Perform the request and any follow-up parsing. */
    res = curl_easy_perform(curl_handle);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else if(user_data.is_proccess_ok)
    {
        /* Expat requires one final call to finalize parsing. */
        if(XML_Parse(parser, NULL, 0, 1) == 0)
        {
            int error_code = XML_GetErrorCode(parser);
            fprintf(stderr, "Finalizing parsing failed with error code %d (%s).\n", error_code, XML_ErrorString(error_code));
        }
        else
        {
            printf("Processed tags: %lu\n", user_data.tag_count);
        }
    }

    /* Clean up. */
    free(user_data.tag.content);
    XML_ParserFree(parser);
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return 0;
}

bool is_match(char *pattern, const char *string, bool ignore_case)
{
    regex_t regex;
    int result;
    bool is_match = false;
    char message_bufer[100];
    int compilation_flags = 0;

    if(ignore_case == true)
    {
        compilation_flags = REG_ICASE;
    }

    /* Compile regular expression */
    result = regcomp(&regex, pattern, compilation_flags);
    if (result)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    result = regexec(&regex, string, 0, NULL, 0);
    if (!result)
    {
        is_match = true;
    }
    else if (result == REG_NOMATCH)
    {
        is_match = false;
    }
    else
    {
        regerror(result, &regex, message_bufer, sizeof(message_bufer));
        fprintf(stderr, "Regex match failed: %s\n", message_bufer);
        regfree(&regex);
        exit(1);
    }

    regfree(&regex);
    return is_match;
}

static void tag_opening_handler(void *user_data, const XML_Char *tag_name, const XML_Char **atts)
{
    user_data_t *data = user_data;
    data->tag_count++;
    data->tag_depth++;
    /* Get a clean slate for reading in character data. */
    free(data->tag.content);
    data->tag.content = NULL;
    data->tag.content_length = 0;
}

static void tag_character_data_handler(void *user_data, const XML_Char *content, int content_length)
{
    user_data_t *data = user_data;
    data->tag.content = realloc(data->tag.content, data->tag.content_length + content_length + 1);
    if(!data->tag.content)
    {
        fprintf(stderr, "Not enough memory (realloc returned NULL).\n");
        data->is_proccess_ok = false;
        return;
    }

    memcpy(data->tag.content + data->tag.content_length, content, content_length);
    data->tag.content_length += content_length;
    data->tag.content[data->tag.content_length] = 0;
    if(is_match("^\\s", data->tag.content + data->tag.content_length - content_length, false))
    {
        data->tag.content_length -= content_length;
        data->tag.content[data->tag.content_length] = 0;
        data->tag.content = realloc(data->tag.content, data->tag.content_length + 1);
    }
}

static void tag_closing_handler(void *user_data, const XML_Char *tag_name)
{
    user_data_t *data =  user_data;

    char *tag_name_copy = strdup(tag_name);
    if (data->tag_depth == 4 && is_match("title", tag_name_copy, true) && is_match(data->search_pattern, data->tag.content, true))
    {
        data->is_search_pattern_found = true;
        printf("%s\n", data->tag.content);
    }
    else if( data->is_search_pattern_found == true && is_match("link", tag_name_copy, true))
    {
        data->is_search_pattern_found = false;
        printf("\t%s\n", data->tag.content);
    }
    free (tag_name_copy);
    data->tag_depth--;
    //clean tag content
    *data->tag.content = 0;
    data->tag.content_length = 0;

}

static size_t parse_stream_callback(void *content_chunk, size_t content_chunk_length, size_t nmemb, void *userp)
{
    XML_Parser parser =  userp;
    size_t real_size = content_chunk_length * nmemb;
    user_data_t *user_data =  XML_GetUserData(parser);
    /* Only parse if we're not already in a failure state. */
    if(user_data->is_proccess_ok && XML_Parse(parser, content_chunk, real_size, 0) == 0)
    {
        int error_code = XML_GetErrorCode(parser);
        fprintf(stderr, "Parsing response buffer of length %lu failed" " with error code %d (%s).\n", real_size, error_code, XML_ErrorString(error_code));
        user_data->is_proccess_ok = false;
    }
    return real_size;
}
