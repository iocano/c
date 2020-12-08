/*
	Program:
        Create n processes and wait for them to finish
        Example of pid_t, fork, getpid, getppid, waitpid, WEXITSTATUS
	Compile:
		gcc main.c -o fork
	-------------------------------------
	Run:
		./fork
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main()
{
    pid_t pids[5];
    for (int i = 0; i < 5; i++) // loop will run n times (n=5)
    {
        if ((pids[i] = fork()) == 0)
        {
            printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            return 0;
        }
        /*
            // Wait for child to finish before fork new process
            int pid_status;
            if (waitpid(pids[i], &pid_status, 0) == -1)
            {
                error("Error waiting for child process");
            }
            int status = WEXITSTATUS(pid_status);
            printf("Process %d exit with status %d\n", pids[i], status);
        */
    }

    for (int i = 0; i < 5; i++)
    {
        int pid_status;
        // Wait for child to finish
        if (waitpid(pids[i], &pid_status, 0) == -1)
        {
            error("Error waiting for child process");
        }
        // get child exit status
        int status = WEXITSTATUS(pid_status);
        printf("Process %d exit with status %d\n", pids[i], status);
    }
    return 0;
}
