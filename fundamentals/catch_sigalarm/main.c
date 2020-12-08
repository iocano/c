/*
	Program:
		Fires an alarm every second for five seconds.
        Example of alarm, sigaction, SIGALARM, sigemptyset,
	Compile:
		gcc main.c -o alarm
	-------------------------------------
	Run:
		./alarm
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int seconds = 5;

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    // sigaction standard handler signature: void(int signal_number)
    action.sa_handler = handler;
    // Clear all signals from SET
    sigemptyset(&action.sa_mask);
    // set no flags to sigaction
    action.sa_flags = 0;
    // arguments (signal_number, new_sigaction, old_sigaction_storage)
    return sigaction(sig, &action, NULL);
}

void wake_up(int sig)
{
   if(--seconds > 0)
   {
       puts("one second gone");
       alarm(1);
   }
   else
   {
       exit(0);
   }
}

int main()
{
    // set wakeup as handle for SIGALARM
    catch_signal(SIGALRM, wake_up);
    // Schedule an alarm in seconds
    alarm(1);
    // long long task
    while (1)
    {
        ;
    }
    return 0;
}
