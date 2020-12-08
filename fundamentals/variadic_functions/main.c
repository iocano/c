/*
	Program:
		Variadic function examples
	Compile:
		gcc main.c -o va_functions
	-------------------------------------
	Run:
		./va_functions
*/

#include <stdio.h>
#include <stdarg.h>

typedef struct
{
    double value;
}container_t;

double sum_values(int argc, ...)
{
    va_list argument_list;

    // Initialize argument list
    va_start(argument_list, argc);

    double sum = 0;
    for(int i = 0; i < argc; i++)
    {
        // get argument from list
        container_t value = va_arg(argument_list,  container_t);
        sum += value.value;
    }
    // clean up memory
    va_end(argument_list);
    return sum;
}

void print(const char* format, ...)
{
    va_list argument_list;
    va_start(argument_list, format);

    while (*format != '\0')
    {
        switch(*format++)
        {
            case 'i':
                printf("%i ", va_arg(argument_list, int));
                break;
            case 'c':
                printf("%c ", va_arg(argument_list, int));
                break;
            case 'f':
                printf("%f ", va_arg(argument_list, double));
                break;
        }
    }
    puts("");
    va_end(argument_list);
}

void print_v2(const char* format, ...)
{
    va_list argument_list;
    va_start(argument_list, format);
    vfprintf(stdout, format, argument_list);
    va_end(argument_list);
}

int main()
{
    container_t v1 = {1};
    container_t v2 = {2};
    container_t v3 = {3.0};

    // sum arguments
    // use struct to enforce type
    double sum = sum_values(3, v1, v2, v3);
    printf("Sum: %f\n", sum);

    // print arguments based on type
    print("cif",'a', 2, 3.14);

    // print based on format
    print_v2("Error: %i, Message: %s\n", 99, "Out of bounds");
}
