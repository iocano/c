/*
    Program:
        daemon example: log "Daemon do" every 30 minutes
    Compile:
        gcc main.c -o daemon
    -------------------------------------
    Run:
        ./daemon
    End:
        search pid (ps -ae | grep daemon)
        kill <pid>
    View logs:
        grep daemon /var/log/syslog
*/

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <stdarg.h>

// add log
void exit_if(bool condition, int exit_status, bool log, const char *format, ...) 
{
    if (condition) {
        va_list argument_list;
        va_start(argument_list, format);
        if (log == false) 
        {
            vfprintf(stderr, format, argument_list);
        }
        else
        {
            char log_message[512];
            sprintf(log_message, format, argument_list);
            syslog(LOG_NOTICE, "%s", log_message);
        }
        va_end(argument_list);
        exit(exit_status);
    }
}

int main(void) 
{
    // fork a child
    pid_t pid = fork();

    // fork return -1 if error
    exit_if(pid < 0, EXIT_FAILURE, false, "");

    // ends parent
    exit_if(pid > 0, EXIT_SUCCESS, false, "");

    // files created after that will have no provileges initially revoked
    umask(0);

    syslog(LOG_NOTICE, "Daemon start!");

    // create a session and sets the process group id
    pid_t sid = setsid();

    // setsid return -1 on error
    exit_if(sid < 0, EXIT_FAILURE, true, "setsid fail");

    // change the current working directory
    exit_if((chdir("/") < 0), EXIT_FAILURE, true, "chdir fail");

    // close out the standard file descriptors
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    // close all open file descriptors
    for (int file_descriptors = sysconf(_SC_OPEN_MAX); file_descriptors >= 0; file_descriptors--) 
    {
        close(file_descriptors);
    }

    while (1) 
    {
        // daemon tasks
        syslog(LOG_NOTICE, "Daemon do!");

        // wait 30 seconds
        sleep(30); 
    }

    exit(EXIT_SUCCESS);
}
