/*
    Program:
        Initialize and set structure fields
        Example of dereference, reference, struct array, struct pointer
    Compile:
        gcc main.c -o persons
    -------------------------------------
    Run:
        ./persons
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    char *last_name;
    int age;
} person_t;

person_t person_init()
{
    int string_length = 25;
    person_t person;
    person.name = malloc(string_length * sizeof(char));
    person.last_name = malloc(string_length * sizeof(char));
    return person;
}

void person_free(person_t *person)
{
    free(person->name);
    free(person->last_name);
}

void person_set_default_data(person_t *person)
{
    const char *default_name = "default name";
    const char *default_last_name = "default last name";
    strncpy(person->name, default_name, strlen(default_name) + 1);
    strncpy(person->last_name, default_last_name, strlen(default_last_name) + 1);
    person->age = -1;
}

void person_set_array_default_data(person_t persons[], int length)
{
    for(int i = 0; i < length; i++)
    {
        person_set_default_data(&persons[i]);
    }
}

void person_print(person_t person)
{
    printf("NAME: %s, LAST NAME: %s, AGE: %i\n", person.name, person.last_name, person.age);
    puts("");
}

void person_array_print(person_t persons[], int length )
{
    for (int i = 0; i < length ; i++)
    {
        person_print(persons[i]);
    }
}

void person_set_data(person_t *person, char *name, char *last_name, int age)
{
    strncpy(person->name, name, strlen(name) + 1);
    strncpy(person->last_name, last_name, strlen(last_name) + 1);
    person->age = age;
}

int main()
{
    person_t persons[2] = {person_init(), person_init()};
    person_set_array_default_data(persons, 2);
    person_array_print(persons, 2);
    person_set_data(&persons[0], "James", "Doe", 20);
    person_set_data(&persons[1], "Joe", "Doe", 20);
    person_array_print(persons, 2);
    person_free(&persons[0]);
    person_free(&persons[1]);

    person_t person_1 = person_init();
    person_set_default_data(&person_1);
    person_print(person_1);
    person_set_data(&person_1, "John", "Doe", 99);
    person_print(person_1);
    person_free(&person_1);

    person_t *person_2 = malloc(sizeof(person_t));
    *person_2 = person_init();
    person_set_default_data(person_2);
    person_print(*person_2);
    person_set_data(person_2, "John", "Doe", 99);
    person_print(*person_2);
    person_free(person_2);
    free(person_2);
    return 0;
}
