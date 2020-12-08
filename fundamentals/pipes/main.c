/*
    Program:
        Search a key word in RSS Feed URL, then open links found on browser
        Example of close, dup2, execle, fork, pipe, sprintf, system, va_end, va_list, va_start, vfprintf
    Compile:
        gcc main.c -o feeds_opener
    -------------------------------------
    Run:
        ./feeds_opener
    Needs:
        feeds_finder (compiled from ../../curl_and_expat/main.c)
*/

#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void open_url(char *url);

void exit_if(bool condition, const char *format, ...);

int main(int argc, char *argv[])
{
    exit_if(argc != 2, "Usage: %s SEARCH_PATTERN\n", argv[0]);

    char *search_pattern = argv[1];

    char *environment_vars[] = {"RSS_FEED_URL=http://feeds.bbci.co.uk/mundo/rss.xml", NULL};

    int fd[2];
    exit_if(pipe(fd) == -1, "Can't create pipe: %s", strerror(errno));

    pid_t pid = fork();
    exit_if(pid == -1, "Can't fork process: %s\n", strerror(errno));

    if (!pid)
    {
        // close read descriptor in child
        close(fd[0]); // fd[0] is the descriptor that read from pipe
        // redirect standard output to the descriptor that write
        dup2(fd[1], 1);

        // execute command
        char *feeds_finder = "./feeds_finder";
        int execle_result = execle(feeds_finder, feeds_finder, search_pattern, NULL, environment_vars);
        exit_if(execle_result == -1, "Can't run %s: %s", feeds_finder, strerror(errno));
    }
    // close write descriptor in parent
    close(fd[1]);
    // redirect standard input to the descriptor thar read
    dup2(fd[0], 0);

    char line[255];
    while (fgets(line, 255, stdin))
    {
        if (line[0] == '\t')
        {
            open_url(line + 1); // line + 1 is the string after '\t'
        }
    }
    return 0;
}

void exit_if(bool condition, const char *format, ...)
{
    if(condition)
    {
        va_list argument_list;
        va_start(argument_list, format);
        vfprintf(stderr, format, argument_list);
        va_end(argument_list);
        exit(EXIT_FAILURE);
    }
}

void open_url(char *url)
{
    char launch[255];
    sprintf(launch, "firefox '%s' &", url);
    system(launch);
}
