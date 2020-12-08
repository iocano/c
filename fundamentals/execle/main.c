/*
    Program:
        Search a key word in array of RSS Feed URL's loaded from a file and store them in a file
        Example of dup2, dynamic array, execle, exit, fclose, File, fileno, fopen, fork, memcpy, pid_t, sprintf, va_end, va_list, va_start, vfprintf, waitpid
    Compile:
        gcc main.c -o feeds_crawler
    -------------------------------------
    Run:
        ./feeds_crawler
    Needs:
        feeds_finder (compiled from ../../curl_and_expat/main.c)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>

typedef struct dynamic_2d_array
{
    void **address;
    int rows;
}dynamic_2d_array_t;

dynamic_2d_array_t* dynamic_2d_array_init()
{
    dynamic_2d_array_t *array = malloc(sizeof(dynamic_2d_array_t));
    array->address = malloc(0);
    array->rows = 0;
    return array;
}

void dynamic_2d_array_push(char *string, dynamic_2d_array_t *array)
{
    array->address = realloc(array->address, sizeof(char*) * (array->rows + 1));

    size_t byte_count = (strlen(string) + 1) * sizeof(char);
    array->address[array->rows] = malloc(byte_count);
    memcpy(array->address[array->rows], string, byte_count);
    array->rows++;
}

void dynamic_2d_array_free(dynamic_2d_array_t *array_2d)
{
    for(int i = 0; i < array_2d->rows; i++)
    {
        free(array_2d->address[i]);
    }
    free(array_2d->address);
    free(array_2d);
}

void load_lines_from_file(dynamic_2d_array_t *array_2d, FILE *file, unsigned line_legth)
{
    char line[line_legth];
    while(fgets(line, line_legth, file) != NULL)
    {
        line[strlen(line) - 1 ] = 0;
        dynamic_2d_array_push(line, array_2d);
    }
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

FILE *open_file(const char *file_name, const char *mode)
{
    FILE *file = fopen(file_name, mode);
    exit_if(!file, "Can't open %s.\n %s\n", file_name, strerror(errno));
    return file;
}

int main(int argc, char *argv[])
{
    exit_if(argc != 2, "Usage: %s SEARCH_PATTERN\n",argv[0]);

    char *search_pattern = argv[1];

    dynamic_2d_array_t *urls_array = dynamic_2d_array_init();

    FILE *input_urls = open_file("urls.txt", "r");

    load_lines_from_file(urls_array, input_urls, 1000);

    fclose(input_urls);

    FILE *feeds_found_file = open_file("feeds_found.txt", "w");

    char enviroment_var[255];

    for (int i = 0; i < urls_array->rows; i++)
    {
        // send formated output to char*
        sprintf(enviroment_var, "RSS_FEED_URL=%s", urls_array->address[i]);

        // execle environment variable array ends with NULL
        char *environment_vars[] = {enviroment_var, NULL};

        pid_t pid = fork();
        exit_if(pid == -1, "Can't fork process.\n %s\n", strerror(errno));

        // child process
        if (pid == 0)
        {
            // redirect standard output(1) to file
            exit_if(dup2(fileno(feeds_found_file), 1) == -1, "Can't redirect Standard Output.\n%s\n", strerror(errno));

            fclose(feeds_found_file);

            // execute script
            char *program = "./feeds_finder";
            int execle_result = execle(program, program, search_pattern, NULL, environment_vars);
            exit_if(execle_result == -1, "Can't run %s.\n%s\n", program, strerror(errno));
        }
        // parent process
        else
        {
            // wait child process to end
            int pid_status;
            exit_if(waitpid(pid, &pid_status, 0) == -1, "Error waiting for child process.\n %s\n", strerror(errno));
        }
    }

    dynamic_2d_array_free(urls_array);
    fclose(feeds_found_file);
    return 0;
}
