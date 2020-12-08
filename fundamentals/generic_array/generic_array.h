/*
    Header:
        Generic and dynamic array macro definition
    -------------------------------------
*/

#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GENERIC_ARRAY_DEFAULT_LENGTH 128

/*
   Generate array manipulation functions and types for 'TYPE' with prefix 'PREFIX'
*/
#define GENERIC_ARRAY_CREATE_FOR(TYPE, PREFIX)                                                     \
                                                                                                   \
typedef struct PREFIX##_array                                                                      \
{                                                                                                  \
    unsigned current_length;                                                                       \
    unsigned maximum_length;                                                                       \
    TYPE *address;                                                                                 \
} PREFIX##_array##_t;                                                                              \
                                                                                                   \
PREFIX##_array##_t* PREFIX##_array##_init(unsigned initial_length)                                 \
{                                                                                                  \
    PREFIX##_array##_t *array = malloc(sizeof(PREFIX##_array##_t));                                \
                                                                                                   \
    array->current_length = 0;                                                                     \
    array->maximum_length = (initial_length == 0)                                                  \
        ? GENERIC_ARRAY_DEFAULT_LENGTH                                                             \
        : initial_length;                                                                          \
    array->address = malloc (array->maximum_length * sizeof(TYPE));                                \
                                                                                                   \
    return array;                                                                                  \
}                                                                                                  \
                                                                                                   \
void PREFIX##_array##_push(TYPE value, PREFIX##_array##_t *array)                                  \
{                                                                                                  \
    if(array->current_length >= array->maximum_length)                                             \
    {                                                                                              \
        array->maximum_length += array->maximum_length;                                            \
        array->address = realloc(array->address, array->maximum_length * sizeof(TYPE));            \
    }                                                                                              \
    array->address[array->current_length++] = value;                                               \
}                                                                                                  \
                                                                                                   \
void PREFIX##_array##_insert_at(unsigned index, TYPE value, PREFIX##_array##_t *array)             \
{                                                                                                  \
    if(array->current_length == array->maximum_length)                                             \
    {                                                                                              \
        array->maximum_length += array->maximum_length;                                            \
        array->address = realloc(array->address, array->maximum_length * sizeof(TYPE));            \
    }                                                                                              \
    if(array->current_length > array->maximum_length)                                              \
    {                                                                                              \
        fprintf(stderr, "INDEX OUT OF BOUNDS\n");                                                  \
        exit(1);                                                                                   \
    }                                                                                              \
    TYPE* element_to_displace = array->address + index;                                            \
    TYPE* new_position = element_to_displace + 1;                                                  \
    size_t byte_count =  (array->current_length - index) * sizeof(TYPE);                           \
    memmove(new_position, element_to_displace, byte_count);                                        \
    array->address[index] = value;                                                                 \
    array->current_length++;                                                                       \
}                                                                                                  \
                                                                                                   \
TYPE PREFIX##_array##_element_at(unsigned index, PREFIX##_array##_t *array)                        \
{                                                                                                  \
    if (index >= array->maximum_length)                                                            \
    {                                                                                              \
        fprintf(stderr, "OUT OF BOUNDS\n");                                                        \
        exit(1);                                                                                   \
    }                                                                                              \
    return array->address[index];                                                                  \
}                                                                                                  \
                                                                                                   \
TYPE* PREFIX##_array##_address_at(unsigned index, PREFIX##_array##_t *array)                       \
{                                                                                                  \
    if (index >= array->maximum_length)                                                            \
    {                                                                                              \
        fprintf(stderr, "OUT OF BOUNDS\n");                                                        \
        exit(1);                                                                                   \
    }                                                                                              \
    return array->address + index;                                                                 \
}                                                                                                  \
                                                                                                   \
void PREFIX##_array##_set_value_at(unsigned index, TYPE value, PREFIX##_array##_t *array)          \
{                                                                                                  \
    if(index >= array->maximum_length)                                                             \
    {                                                                                              \
        fprintf(stderr, "OUT OF BOUNDS\n");                                                        \
        exit(1);                                                                                   \
    }                                                                                              \
    array->address[index] = value;                                                                 \
}                                                                                                  \
                                                                                                   \
TYPE PREFIX##_array##_pop(PREFIX##_array##_t *array)                                               \
{                                                                                                  \
    if (array->current_length ==  0)                                                               \
    {                                                                                              \
        fprintf(stderr, "Empty array\n");                                                          \
        exit(1);                                                                                   \
    }                                                                                              \
    return array->address[--array->current_length];                                                \
}                                                                                                  \
                                                                                                   \
void PREFIX##_array##_remove_at(unsigned index, PREFIX##_array##_t *array)                         \
{                                                                                                  \
    if(index >= array->maximum_length)                                                             \
    {                                                                                              \
        fprintf(stderr, "OUT OF BOUNDS\n");                                                        \
        exit(1);                                                                                   \
    }                                                                                              \
    TYPE *element_to_remove = array->address + index;                                              \
    TYPE *next_element = element_to_remove + 1;                                                    \
    unsigned byte_count = sizeof(int) * (array->current_length - index);                           \
    memmove(element_to_remove, next_element, byte_count);                                          \
    array->current_length--;                                                                       \
}                                                                                                  \
                                                                                                   \
void PREFIX##_array_free(PREFIX##_array_t *array)                                                  \
{                                                                                                  \
    free(array->address);                                                                          \
    free(array);                                                                                   \
}                                                                                                  \

#endif /* GENERIC_ARRAY_H */
