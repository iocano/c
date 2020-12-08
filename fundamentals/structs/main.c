/*
    Program:
        Initialize, update and print struct data
        Example of enum, localtime, strftime, struct, struct_tm, time
    Compile:
        gcc main.c -o structs
    -------------------------------------
    Run:
        ./structs
*/

#include <stdio.h>
#include <time.h>

typedef enum file_type
{
    PATH_FILE,
    PATH_LINK
}file_type_t;

typedef struct file_metadata
{
    char *author;
    time_t created;
    time_t modified;
}file_metadata_t;

typedef struct file
{
    char *path;
    char *name;
    char *extension;
    file_metadata_t metadata;
    file_type_t type;
}file_t;

void file_printf(file_t file)
{
    struct tm *created_time_info;
    struct tm *modified_time_info;

    created_time_info = localtime(&file.metadata.created);
    modified_time_info = localtime(&file.metadata.modified);

    char created_time_string[9];  // space for "HH:MM:SS\0"
    char modified_time_string[9];  // space for "HH:MM:SS\0"

    strftime(created_time_string, sizeof(created_time_string), "%H:%M:%S", created_time_info);
    strftime(modified_time_string, sizeof(modified_time_string), "%H:%M:%S", modified_time_info);

    printf("Path: %s\nName: %s\nExtension: %s\n", file.path, file.name, file.extension);
    printf("Author: %s\nCreated at: %s\nModified at: %s\n", file.metadata.author, created_time_string, modified_time_string);
}

int main(int argc, char *argv[])
{
    // initialization first form
    file_t file1 = {"path/to/", "file", "ext", {"JOHN DOE", time(NULL), time(NULL)}, PATH_FILE};

    /*
        initialization second form
        file_t file2 = {.path = "path/to/", .name = "file", .extension = "ext", .metadata = {.author= "JOHN DOE", .created = time(NULL), .modified = time(NULL)}, .type = PATH_FILE};
    */

    /*
        file_t file3;
        file3 = (file_t){"path/to/", "file", "ext", {"JOHN DOE", time(NULL), time(NULL)}, PATH_FILE};
    */

    /* update author */
    file1.metadata.author = "Jane Doe";
    /* update extension */
    file1.extension = "doc";

    file_printf(file1);

    return 0;
}
