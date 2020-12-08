/*
	Program:
        Basic thread horse race
        Example of pthread_create, pthread_join, pthread_mutex_lock, pthread_mutex_unlock, pthread_t, sleep
	Compile:
		gcc main.c -o program
	-------------------------------------
	Run:
		./program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void *horse_go(void *a);

void error(char *msg);

// allows to block shared resources and ensure that only one thread is using that resources at a time
pthread_mutex_t horse_winner_lock= PTHREAD_MUTEX_INITIALIZER;
char *horse_winner;

int main()
{
    // record information about threads
    pthread_t t0;
    pthread_t t1;

    // horse names
    char *horse_one = "horse one";
    char *horse_two = "horse two";

    /*
        Create threads
        pthread_create arguments:
            thread
            tread attributes: NULL to default
            routine: function to execute, signature -> void *(*routine) (void *)
            arg: argument passed to routine
    */
    if (pthread_create(&t0, NULL, horse_go, (void*)horse_one) != 0)
    {
        error("Can't create thread t0");
    }
    if (pthread_create(&t1, NULL, horse_go, (void*)horse_two) != 0)
    {
        error("Can't create thread t1");
    }

    /*
        Wait threads to finish
        pthread_join arguments:
            thread
            retval: return value from routine (routine use here return horse name)
    */
    void *result;
    if (pthread_join(t0, &result) == -1)
    {
        error("Can't join thread t0");
    }
    if (pthread_join(t1, &result) == -1)
    {
        error("Can't join thread t1");
    }

    printf("\nWinner: %s\n", horse_winner);

    return 0;
}

// routine to use on threads
void *horse_go(void *a)
{
    char *horse = a;
    for (int i = 0; i < 5; i++)
    {
        printf("%s go\n", horse);
        // sleep one second
        sleep(1);
    }

    pthread_mutex_lock(&horse_winner_lock);
    if(horse_winner == NULL)
    {
        horse_winner = horse;
    }
    pthread_mutex_unlock(&horse_winner_lock);
    return horse;
}

void error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
