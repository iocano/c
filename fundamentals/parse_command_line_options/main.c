/*
    Program:
        Print string n times, in upper case, or in reverse order, based on command line options
        Example of atoi, getopt, optarg, optind
    Compile:
        gcc main.c -o parse_options
    -------------------------------------
    Run:
        ./parse_options [-t ntimes] [-U] [-r] STRING_TO_PRINT
    Options:
        -t  <number>    Number of times the string will be printed
        -U              Print string in Upper case
        -r              Print string on reverse order
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char *program = argv[0];
    int times = 1;
    bool upper_case = false;
    bool print_backwards = false;

    char option;
    while ((option = getopt(argc, argv, "rUt:")) != -1)
    {
        switch (option)
        {
            case 't':
                times = atoi(optarg);
                break;
            case 'U':
                upper_case = true;
                break;
            case 'r':
                print_backwards = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-t ntimes] [-U] [-r] STRING_TO_PRINT\n", program);
                exit(EXIT_FAILURE);
        }
    }
    argc -= optind;
    argv += optind;

    if(argc != 1)
    {
        fprintf(stderr, "Usage: %s [-t ntimes] [-U] [-r] STRING_TO_PRINT\n", program);
        exit(EXIT_FAILURE);
    }

    int string_length = strlen(argv[0]);
    int wise = 0;
    char *string_to_print = argv[0];

    if (print_backwards == true)
    {
        for (int i = 0; i < times ; i++)
        {
            for (int i = string_length; i >= 0; i--)
            {
                char c = (upper_case == true) ? toupper(string_to_print[i]) : tolower(string_to_print[i]);
                printf("%c", c);
            }
            puts("");
        }
    }
    else
    {
        for (int i = 0; i < times ; i++)
        {
            for (int i = 0; i < string_length; i++)
            {
                char c = (upper_case == true) ? toupper(string_to_print[i]) : tolower(string_to_print[i]);
                printf("%c", c);
            }
            puts("");
        }
    }

    return 0;
}
