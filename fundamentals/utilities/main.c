/*
    Program:
        Utilities test
    Compile:
        gcc main.c -o utilities
    -------------------------------------
    Run:
        ./utilities
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

void print_no_printables(char* string)
{
    // plus '\0'
    int len = strlen(string) + 1;
	for (int i = 0; i < len; i++)
    {
		if (isprint(string[i]))
        {
			printf("<%i> %c", i, string[i]);
        }
		else
        {
			printf("<%i> \\x%02x", i, string[i]);
        }
    }
    printf("\ncharacter's total: %i\n", len);
}

void file_metada(FILE *file)
{
    struct stat attrib;
    stat("file.txt", &attrib);
    char time[50];
    // get modification time
    strftime(time, 50, "%Y-%m-%d %H:%M:%S", localtime(&attrib.st_mtime));
    printf ("%s\n", time);
}

void print_files_on_path(const char *path)
{
    struct dirent* in_file;
    DIR *directory = opendir(path);
    while((in_file = readdir(directory)))
    {
        if (!strcmp (in_file->d_name, ".") || !strcmp (in_file->d_name, ".."))
        {
             continue;
        }
        printf("%s\n", in_file->d_name);
    }
}

int main()
{
    /* char *test_string = "hello world"; */
    /* print_no_printables(test_string); */

    print_files_on_path("/home/alberto/Documents/test/");

    return 0;
}
