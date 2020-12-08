/*
    Program:
        Test generic array basic operations with one primitive type and one pointer type
    Compile:
        gcc main.c -o generic_array_test
    -------------------------------------
    Run:
        ./generic_array_test
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "generic_array.h"

// array of ints        (type, PREFIX)
GENERIC_ARRAY_CREATE_FOR(int, int)

GENERIC_ARRAY_CREATE_FOR(char*, string)

char* generate_random_string(unsigned minimum_length, unsigned maximum_length)
{
    if(minimum_length >= maximum_length)
    {
        fprintf(stderr, "Minimum string length is greater or equal to Maximum string length\n");
        exit(1);
    }

    char *random_string = malloc(maximum_length + 1);

    unsigned random_length = rand() % (maximum_length - minimum_length + 1) + minimum_length;

    int i;
    for(i = 0; i < random_length; i++)
    {
        char letter = rand() % 26 + 'a'; // 97 && 122
        random_string[i] = letter;
    }
    random_string[i] = 0;

    return random_string;
}

int main()
{
     srand(time(0));

    int_array_t *numbers = int_array_init(10);

    for (int i = 0; i < numbers->maximum_length; i++)
    {
        int_array_push(i * 99, numbers);
    }

    int_array_insert_at(9, 33, numbers);

    int number_at_2 = int_array_element_at(2, numbers);

    int* number_address_at_8 = int_array_address_at(8, numbers);

    int_array_set_value_at(5, 1970, numbers);

    int last_number = int_array_pop(numbers);

    int_array_remove_at(7, numbers);

    for (int i = 0; i < numbers->current_length; i++)
    {
        printf("%i \n", int_array_element_at(i, numbers));
    }

    int_array_free(numbers);

    puts("\n");

    string_array_t *strings = string_array_init(10);

    for(int i = 0; i < strings->maximum_length; i++)
    {
        string_array_push(generate_random_string(5, 10), strings);
        printf("%s \n", string_array_element_at(i, strings));
    }
    puts("------------------------------------");

    string_array_insert_at(9, generate_random_string(2,5), strings);

    char *string_at_2 = string_array_element_at(2, strings);

    char **string_address_at_8 = string_array_address_at(8, strings);

    char *new_string = generate_random_string(11, 12);
    char **string_address_at_5 = string_array_address_at(5, strings);
    free(*string_address_at_5); // free memory before replace
    string_array_set_value_at(5, new_string, strings);

    char *last_string = string_array_pop(strings);
    free(last_string);

    char **string_to_remove = string_array_address_at(7, strings);
    free(*string_to_remove); // free string before removed from array
    string_array_remove_at(7, strings);

    for(int i = 0; i < strings->current_length; i++)
    {
        printf("%s \n", string_array_element_at(i, strings));
        free(string_array_element_at(i, strings));
    }

    string_array_free(strings);

    return 0;
}
