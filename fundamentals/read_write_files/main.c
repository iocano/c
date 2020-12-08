/*
    Program:
        Read books.csv, filter genres based on command line arguments
        and create separated files with books that meets the criteria
        Example of command line arguments, FILE, fclose, fopen, fprintf, fscanf, strspn, strstr, tolower
    Compile:
        gcc main.c -o filter_genres
    -------------------------------------
    Run:
        ./filter_genres GENRE1 GENRE1_FILE GENRE2 GENRE2_FILE UNCATEGORIZED_FILE
    Example:
        ./filter_genres fiction fiction_file.csv history history_file.csv uncategorized_file.csv
    Need:
        books.csv (format: title(string); author(string); genre(string); pages(int); publisher(string))
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

char* get_file_name(char *path);

char* string_to_lower_case(char *string);

int main(int argc, char *argv[])
{
    char *program_name = get_file_name(argv[0]);
    if(argc < 4)
    {
        fprintf(stderr, "Usage: %s PATH_TO_BOOKS_DOT_CSV GENRE_1 GENRE_1_FILE [GENRE_N GENRE_N_FILE] [GENRE_OTHERS_FILE] \n", program_name);
        exit(1);
    }
    FILE *books_dot_csv_file = fopen(argv[1], "r");
    if (!books_dot_csv_file)
    {
        fprintf(stderr, "Error with %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }
    argv+=2; // offset 2 positions
    argc-=2;

    // if genre_others_files is passed as argument an extra array position is needed
    int genre_file_count = argc / 2 + (argc % 2 != 0);
    FILE **genre_files = malloc(sizeof(char*) * genre_file_count);

    int genre_count = (argc / 2 == 0) ? 1 : argc / 2;
    char **genres = malloc(sizeof(char*) * genre_count);

    for (int i = 0; i < genre_count; i++)
    {
        genres[i] = *argv++;
        char *file_path = *argv++;
        genre_files[i] = fopen(file_path, "w");
        if(!genre_files[i])
        {
            fprintf(stderr, "Error with %s: %s\n", file_path, strerror(errno));
            exit(1);
        }

        argc-=2;
    }
    if( genre_file_count > genre_count )
    {
        genre_files[genre_file_count - 1] = fopen(*argv++, "w");
        argc--;
    }

    char title[100];
    char author[50];
    char genre[30];
    int pages;
    char publisher[50];
    int c = 0;

    while (fscanf(books_dot_csv_file, "%99[^;];%49[^;];%29[^;];%i;%49[^\n]\n", title, author, genre, &pages, publisher) == 5)
    {
        FILE *genre_file = NULL;
        for (int i = 0; i < genre_count; i++)
        {
            if(strspn(string_to_lower_case(genres[i]), string_to_lower_case(genre)) == strlen(genre))
            {
                genre_file = genre_files[i];
                break;
            }
        }
        if(genre_file == NULL && genre_file_count > genre_count)
        {
            genre_file = genre_files[genre_file_count - 1];
        }
        if (genre_file != NULL)
        {
            fprintf(genre_file, "%s;%s;%s;%i;%s\n", title, author, genre, pages, publisher);
        }
    }

    fclose(books_dot_csv_file);
    for (int i = 0; i < genre_file_count; i++)
    {
        fclose(genre_files[i]);
    }

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

char* string_to_lower_case(char *string)
{
    for (char *buffer = string; *buffer; buffer++)
    {
        *buffer = tolower(*buffer);
    }
    return string;
}
