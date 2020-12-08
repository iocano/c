/*
	Program:
		Reallocate memory and insert values SIZE times, then shows the time taken
        Example of clock, CLOCKS_PER_SEC, clock_t
	Compile:
		gcc main.c -o execution_time
	-------------------------------------
	Run:
		./execution_time
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000

int main()
{
    clock_t begin = clock();

    // int  *array = malloc(SIZE * sizeof(int));
    int  *array = malloc(0);

    for(int i = 0; i < SIZE; i++)
    {
        array = realloc(array, sizeof(int) *(i + 1));
        *(array + i) = i;
    }
    free(array);

    clock_t end = clock();

    /*
        CLOCKS_PER_SEC: Clock ticks per second
        This macro expands to an expression representing the number of clock ticks per second.
        Clock ticks are units of time of a constant but system-specific length, as those returned by function clock.
        Dividing a count of clock ticks by this expression yields the number of seconds
    */
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\ntime: %.2f \n", time_spent);

    return 0;
}
