/*
	Program:
		Show use of pointers as arguments
        Example of dereference, free, malloc, pointer, reference, sizeof, strcpy
	Compile:
		gcc main.c -o pointer_as_arguments
	-------------------------------------
	Run:
		./pointer_as_arguments
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_data(char *first_name, char *last_name, int *age, int *id)
{
    strcpy(first_name, "John");
    strcpy(last_name, "Doe");
    // derefence pointers to set values
    *age = 20;
    *id = 9453234;
}

int main()
{
    // dynamically-allocated data is released when it is freed with free function
    int *age = malloc(sizeof(int));
    // derefence pointer to set value
    *age = 0; // avoid garbage value
    char *first_name = malloc(sizeof(char) * 20);

    // Local variables are released when their containing function ends
    int id = 0; // no a pointer type but id reference can be passed as argument
    char last_name[20]; // also a char pointer for 20 chars

    // set_data function only receive pointers as arguments, first_name, last_name and age are pointers
    // but id isn't, so need it be referenced using the '&' operator
    set_data(first_name, last_name, age, &id);

    // first_name and last_name are readed as pointers by printf,
    // age need dereference because %i read an int (age is a pointer to int), finally id is int type
    printf("First name: %s\nLast name: %s\nAge: %i\nId: %i\n", first_name, last_name, *age, id);

    // free memory
    free(age);
    free(first_name);
    return 0;
}
