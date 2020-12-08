/*
	Program:
		Input and register n commentaries in a log file, until user type 'exit'
        Example of asctime_r, localtime_r, sprintf, strcasecomp, struct tm, system, time, time_t
	Compile:
		gcc main.c -o commentary_logger
	-------------------------------------
	Run:
		./commentary_logger
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *get_current_time(char *current_time_string)
{
    time_t current_time;
    time(&current_time);

    struct tm current_time_tm;
    // thread safe version of localtime
    localtime_r(&current_time, &current_time_tm);

    /* char *current_time_string = asctime(&current_time_tm); */
    // thread safe version of asctime
    asctime_r(&current_time_tm, current_time_string);

    // replace '\n' with '\0'
    current_time_string[strlen(current_time_string) -  1] = '\0';

    printf("length: %li", strlen(current_time_string));

    return current_time_string;
}

int main()
{
    printf("Type 'exit' to end program.\n");
    while(1)
    {
        char commentary[50];
        /* commentary length plus command length (with current_time) */
        char command_to_execute[98];

        printf("Input a commentary: ");
        fgets(commentary, 50, stdin); //  fgets ends string with '\n'
        commentary[strlen(commentary) - 1] = '\0'; // swap '\n' by '\0'

        if (strcasecmp("exit", commentary) == 0)
        {
            break;
        }

        char current_time_string[25];
        get_current_time(current_time_string);
        // &>> append string
        sprintf(command_to_execute, "echo '%s\t%s' &>> commentary.log", current_time_string, commentary);
        // >> overwrite file
        /* sprintf(command_to_execute, "echo '%s\t%s' &>> commentary.log", current_time, commentary); */

        // execute comment
        system(command_to_execute);

        printf("Commentary registered\n");
    }
    return 0;
}
