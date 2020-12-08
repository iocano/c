#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "include/utils.h"

void exit_if(bool condition, const char *format, ...)
{
    if(condition)
    {
        va_list argument_list;
        va_start(argument_list, format);
        vfprintf(stderr, format, argument_list);
        va_end(argument_list);
        exit(EXIT_FAILURE);
    }
}

int random_int_inclusive(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

float random_float_inclusive(float min, float max)
{
    return min + (float)rand()/(float)RAND_MAX * (max - min);
}
