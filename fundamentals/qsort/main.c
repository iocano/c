/*
    Program:
        Sort and display an array of int, char* and custom struct.
        Example of qsort
    Compile:
        gcc  sort.c -o sort
    -------------------------------------
    Run:
        ./sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compare_int_asc(const void* number_a, const void* number_b)
{
    int a = *(int*) number_a;
    int b = *(int*) number_b;
    return a - b;
}

int compare_int_desc(const void* number_a, const void* number_b)
{
    return compare_int_asc(number_b, number_a);
}

void print_int_array(int *array, int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("%i, ", array[i]);
    }
    puts("");
}

int compare_strings_asc(const void* string_a, const void* string_b)
{
    char* a = *(char**) string_a;
    char* b = *(char**) string_b;
    return strcmp(a, b); // a is lesser than b
}

int compare_strings_desc(const void* string_a, const void* string_b)
{
    return compare_strings_asc(string_b, string_a);
}

void print_string_array(char **array, int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("%s, ", array[i]);
    }
    puts("");
}

typedef struct
{
    int width;
    int height;
} object_t;

int compare_object_area_asc(const void* object_a, const void* object_b)
{
    object_t a = *(object_t*) object_a;
    object_t b = *(object_t*) object_b;
    int area_a = a.width * a.height;
    int area_b = b.width * b.height;
    return area_a - area_b;
}

int compare_object_area_desc(const void* object_a, const void* object_b)
{
    return compare_object_area_asc(object_b, object_a);
}

void print_object_array(object_t *array, int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("{%i, %i}, ", array[i].width, array[i].height);
    }
    puts("");

}

int main()
{
    int numbers[] = {543, 323, 32, 554, 11, 3, 112};
    size_t numbers_length = sizeof(numbers) / sizeof(int);
    qsort(numbers, numbers_length, sizeof(int), compare_int_desc);
    print_int_array(numbers, numbers_length);

    char *strings[] = {"java", "php", "html", "c", "go", "javascript" };
    size_t strings_length = sizeof(strings) / sizeof(strings[0]);
    qsort(strings, 4,sizeof(char*), compare_strings_asc);
    print_string_array(strings, strings_length);

    object_t objects[] = {{6, 4}, {3,3}, {1,1}, {5,5}, {2,2}};
    size_t objects_length = sizeof(objects) / sizeof(object_t);
    qsort(objects, objects_length, sizeof(object_t), compare_object_area_asc);
    print_object_array(objects, objects_length);
}
