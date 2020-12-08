/*
    Program:
        Fill and display personal data
        Example of bitfield
    Compile:
        gcc main.c -o personal_data
    -------------------------------------
    Run:
        ./personal_data
*/
#include <stdio.h>
#include <string.h>

typedef struct
{
    unsigned is_male : 1;
    unsigned age : 7;
    unsigned is_healthy : 1;
    unsigned working_days: 3;
    unsigned hours_a_day : 4;
} person_t;

int main()
{
    person_t john_doe;

    john_doe.is_male = 1;
    john_doe.age = 25;
    john_doe.is_healthy = 1;
    john_doe.working_days = 5;
    john_doe.hours_a_day = 8;

    char *gender =  john_doe.is_male == 1 ? "Male" : "Female";
    char *health =  john_doe.is_healthy == 1 ? "Yes" : "No";

    printf("Person size: %12li bytes\n", sizeof(john_doe));
    printf("Gender: %20s\n", gender);
    printf("Is healthy: %15s\n", health);
    printf("Age: %21i\n", john_doe.age);
    printf("Working days in a week: %i\n", john_doe.working_days);
    printf("Hours per day: %10i\n", john_doe.hours_a_day);

    return 0;
}
