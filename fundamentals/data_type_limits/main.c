/*
    Program:
        Show Max values, Min values, and Sizes for main types of c
    Compile:
        gcc main.c -o data_types_limits
    -------------------------------------
    Run:
        ./data_types_limits
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int main()
{
    printf("The value of CHAR_MAX is %i\n", CHAR_MAX);
    printf("The value of CHAR_MIN is %i\n", CHAR_MIN);
    printf("A char takes %i bytes\n\n", sizeof(char));

    printf("The value of SHRT_MAX is %i\n", SHRT_MAX);
    printf("The value of SHRT_MIN is %i\n", SHRT_MIN);
    printf("An short takes %i bytes\n\n", sizeof(short));

    printf("The value of INT_MAX is %i\n", INT_MAX);
    printf("The value of INT_MIN is %i\n", INT_MIN);
    printf("An int takes %i bytes\n\n", sizeof(int));

    printf("The value of LONG_MAX is %i\n", LONG_MAX);
    printf("The value of LONG_MIN is %i\n", LONG_MIN);
    printf("An long takes %i bytes\n\n", sizeof(long));

    printf("The value of FLT_MAX is %f\n", FLT_MAX);
    printf("The value of FLT_MIN is %.50f\n", FLT_MIN);
    printf("A float takes %i bytes\n\n", sizeof(float));

    printf("The value of DBL_MAX is %f\n", DBL_MAX);
    printf("The value of DBL_MIN is %f\n", DBL_MIN);
    printf("A double takes %i bytes\n\n", sizeof(double));
    return 0;
}
