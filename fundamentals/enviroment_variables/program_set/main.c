/*
	Program:
	    Call execle with environment var.
        Example of execle
	Compile:
		gcc main.c -o program_set
	-------------------------------------
	Run:
		./program_set PROGRAM_GET
*/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s PROGRAM_GET\n", argv[0]);
        return 1;
    }

    char *my_env[] = {"NUMBER=1", NULL};
	char *command = argv[1];

    if(execle(command, command, "argument passed by 'program_set'", NULL, my_env) == -1)
    {
        fprintf(stderr, "Cant run program: %s\n", strerror(errno));
        return 1;
    }
}
