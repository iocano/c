/*
	Program:
		Print object based on type.
        Example of free, function pointer array, malloc, strcpy, strtof, strtol,
	Compile:
		gcc main.c -o function_pointer_array
	-------------------------------------
	Run:
		./function_pointer_array
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum value_type
{
    STRING,
    FLOAT,
    INT,
    ACCOUNT_T
}value_t;

typedef struct account_type
{
    char *user;
    char *password;
}account_t;

typedef struct object_type
{
    void *value;
    value_t type;
} object_t;

void print_string(object_t object);

void print_float(object_t object);

void print_int(object_t object);

void print_account(object_t object);

void* account_init(char *user, char *password);

void account_destroy(account_t *account);

int main()
{
    object_t objects[] =
    {
        {"Test string", STRING},
        {"3.5", FLOAT},
        {"100", INT},
        {account_init("John Doe", "1"), ACCOUNT_T}
    };
    size_t objects_length = sizeof(objects) / sizeof(object_t);

    // in enum value_type order: STRING, FLOAT, INT, ACCOUNT_T
    // array of functions (*print_functions[]) that receive one argument type object_t
    void (*print_functions[])(object_t) = {print_string, print_float, print_int, print_account};

    for (int i = 0; i < objects_length; i++)
    {
        (print_functions[objects[i].type])(objects[i]);
    }

    account_destroy(objects[3].value);
    return 0;
}

void print_string(object_t object)
{
    printf("%s\n", object.value);
}

void print_float(object_t object)
{
    printf("%.2f\n", strtof(object.value, NULL));
}

void print_int(object_t object)
{
    printf("%li\n", strtol(object.value, NULL, 10));
}

void print_account(object_t object)
{
    account_t *account = object.value;
    printf("User: %s, Password: %s\n", account->user, account->password);
}

void* account_init(char *user, char *password)
{
    account_t *account = malloc(sizeof(account_t));
    account->user = malloc(strlen(user) + 1);
    strcpy(account->user, user);
    account->password = malloc(strlen(password) + 1);
    strcpy(account->password, password);
    return account;
}

void account_destroy(account_t *account)
{
    free(account->user);
    free(account->password);
    free(account);
}
