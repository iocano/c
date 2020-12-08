/*
	Program:
		Call a replacement program passing an argument and set two enviroment variables (VAR1 and VAR2)
        Example of errno, execle, fprintf, strerror,
	Compile:
		gcc main.c -o main_program
	-------------------------------------
	Run:
		./main_program REPLACEMENT_PROGRAM
*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *replacement_program= argv[1];
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s PATH/TO/REPLACEMENT_PROGRAM\n", argv[0]);
        return 1;
    }
    // array of enviroment variables
    char *my_env[] = {"VAR1=value1", "VAR2=value2", NULL};

    if (execle(replacement_program, replacement_program, "argument1", NULL, my_env) == -1)
    {
        fprintf(stderr, "Can't run process: %s\n", strerror(errno));
        return 1;
    }
    /* If execle is success, this process ends */

    return 0;
}
