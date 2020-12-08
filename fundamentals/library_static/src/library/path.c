/*
    Static library:
        Basic string path perations
    Compile:
        gcc -c path.c -I include/ -o path.o
    Create library:
        ar -rcs libpath.a path.o
    Or
        Run make file
    Use:
        copy library to /usr/local/lib/: cp libpath.a /usr/local/lib
        grant read permission: sudo chmod +r /usr/local/lib/libpath.a
        Or
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:Path/To/Lib
*/

#include <path.h> // include path is specified on compilation time

char* path_get_file_name(char *path)
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

