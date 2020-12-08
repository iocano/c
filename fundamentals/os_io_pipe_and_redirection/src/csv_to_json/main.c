/*
    Program:
        Convert csv_reader output to json file using OS output redirection
    Compile:
        gcc main.c -o csv_to_json
        Or run Makefile from os_io_pipe_and_redirection folder
    -------------------------------------
    Run:
        (./csv_reader | ./csv_to_json) < books.csv > books.json (executables and books.csv on same folder)
        Or execute run.sh script from os_io_pipe_and_redirection folder
    -------------------------------------
    Need:
        books.csv (has the following format : title(string); author(string); genre(string); pages(int); publisher(string))
        csv_reader
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
    char title[100];
    char author[50];
    char genre[30];
    int pages;
    char publisher[50];

    bool print_comma = false;

    puts("{\n\t\"data\" : [");
    while (scanf("%99[^;];%49[^;];%29[^;];%i;%49[^\n]\n", title, author, genre, &pages, publisher) == 5)
    {
        if(print_comma)
        {
            printf(",\n");
        }
        else
        {
            print_comma = true;
        }
        printf("\t\t{\"title\" : \"%s\", \"author\" : \"%s\", \"genre\" : \"%s\", \"pages\" : %i, \"publisher\" : \"%s\"}", title, author, genre, pages, publisher);
    }
    puts("\n\t]\n}");
    return 0;
}
