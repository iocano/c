/*
	Program:
		Increment print a variable, until user interrupt it
        Example of interrupt process, sigaction, sigemptyset, SIGINT
	Compile:
		gcc main.c -o program
	-------------------------------------
	Run:
		./program
*/


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void shutdown_function(int sig);

int catch_signal(int sig, void (*handler)(int));

int *number;

int main()
{
    number = malloc(sizeof(int));
    *number = 0;

    if (catch_signal(SIGINT, shutdown_function) == -1)
    {
        fprintf(stderr, "Can't map the handler");
        exit(1);
    }

    while(1)
    {
        (*number)++;
        printf("Number: %i\n", *number);
    }

    return 0;
}

void shutdown_function(int sig)
{
    // free resources
    free(number);
    puts("\nResources released");
    exit(1);
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}
