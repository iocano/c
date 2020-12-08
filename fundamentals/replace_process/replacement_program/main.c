/*
	Program:
		Get and print enviroment variables (VAR1 and VAR2), program name and one command line argument
        Example of command line arguments, enviroment variables, getenv
	Compile:
		gcc main.c -o replacement_program
	-------------------------------------
	Run:
        EXPORT VAR1=VALUE1
        EXPORT VAR2=VALUE2
		./replacement_program ANY_STRING
    NOTE:
        To be called by 'main_program'
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_file_name(char *path);

int main(int argc, char *argv[])
{
    char *var1 = getenv("VAR1");
    if(!var1)
    {
        fprintf(stderr, "No enviroment variable 'VAR1'");
        return 1;
    }

    char *var2 = getenv("VAR1");
    if(!var2)
    {
        fprintf(stderr, "No enviroment variable 'VAR2'");
        return 1;
    }

    // Program require an extra argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s ANY_STRING\n", argv[0]);
        return 1;
    }
    char *program_name = get_file_name(argv[0]);

    printf("Program name: %s\nArgument passed: %s\n", program_name, argv[1]);
    printf("Enviroment variable1 set on main_program: %s\n", var1);
    printf("Enviroment variable2 set on main_program: %s\n", var2);
    return 0;
}

char* get_file_name(char *path)
{
    char *buffer = path + strlen(path);
    while (buffer != path && *--buffer != '/')
    {
        ;
    }
    if (*buffer == '/')
    {
        return buffer + 1;
    }
    return buffer;
}
