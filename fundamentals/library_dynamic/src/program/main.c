/*
    Program:
        Print file name (if exists) of string path
    Compile opt 1
        gcc main.c -I ../library/include -L ../library -lpath -o program
    Compile opt 2
        copy library to /usr/local/lib/: cp libpath.so /usr/local/lib
        grant read permission: sudo chmod +r /usr/local/lib/libpath.so
        Or
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:Path/To/Lib

        gcc main.c -I ../library/include -lpath -o program
    Or
        Run make file
    -------------------------------------
    Before run:
        copy library to /usr/local/lib/: cp libpath.so /usr/local/lib
        grant read permission: sudo chmod +r /usr/local/lib/libpath.so
        Or
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:Path/To/Lib
    -------------------------------------
    Run:
        ./program
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <path.h> // header directory specified in compilation time


int main(int argc, char *argv[])
{
    char *path = path_get_file_name("folder/file");
    puts(path);
    return 0;
}
