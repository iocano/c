/*
    Dynamic library:
        Basic string path perations
    Compile witht fPIC flag:
        gcc -c path.c -I include/ -fPIC -o path.o
    Create library:
        gcc -shared path.o -o libpath.so
        ar -rcs libpath.a path.o
    Or
        Run make file
    Use:
        copy library to /usr/local/lib/: cp libpath.so /usr/local/lib
        grant read permission: sudo chmod +r /usr/local/lib/libpath.so
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

