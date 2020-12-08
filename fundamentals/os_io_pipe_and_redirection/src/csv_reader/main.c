/*
    Program:
        Read books.csv through OS input redirection file and filter rows with pages greater than 200
    Compile:
        gcc main.c -o csv_reader
    Or
        run Makefile
    -------------------------------------
    Run:
        (./csv_reader | ./csv_to_json) < books.csv > books.json (executables and books.csv on same folder)
        Or execute run.sh script from os_io_pipe_and_redirection folder
    Need:
        books.csv (format: title(string); author(string); genre(string); pages(int); publisher(string))
        csv_to_json program
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char title[100];
    char author[50];
    char genre[30];
    int pages;
    char publisher[50];

    while (scanf("%99[^;];%49[^;];%29[^;];%i;%49[^\n]\n", title, author, genre, &pages, publisher) == 5)
    {
        if (pages >= 200)
        {
            printf("%s;%s;%s;%i;%s\n", title, author, genre, pages, publisher);
        }
    }
    return 0;
}
