/*
	Program:
		Get and print environment var.
        Example of getenv
	Compile:
		gcc main.c -o program_get
	-------------------------------------
	Run:
		./program_get
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("arg 1: %s\n", argv[1]);

    char *number = getenv("NUMBER");
    if(!number)
    {
        fprintf(stderr, "Error with enviroment variable 'NUMBER'\n");
        return 1;
    }

    printf("Enviroment variable value: %s\n", getenv("NUMBER"));
    return 0;
}
