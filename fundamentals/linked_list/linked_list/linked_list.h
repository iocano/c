/*
    Library:
        Linked list
        Example of free, linked list, malloc, memcpy, node
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "node.h"

typedef enum linked_list_status
{
    LINKED_LIST_STATUS_SUCCESS = 1,
    LINKED_LIST_STATUS_EMPTY_LIST = 2,
    LINKED_LIST_STATUS_NULL_NODE = 3,
    LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS = 4,
}linked_list_status_t;

typedef struct linked_list
{
    node_t *head;
    node_t *tail;
    int count;
    linked_list_status_t last_status;
}linked_list_t;

/*
    Initialize linked_list_t struct
*/
linked_list_t *linked_list_create();

/*
    Add a new node to head of linked_list
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_NULL_NODE
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_add_first(void *data, int data_size, linked_list_t *list);

/*
   Add a new node at 'index' on linked_list
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS
        | LINKED_LIST_STATUS_NULL_NODE
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_add_at(int index, void *data, int data_size, linked_list_t *list);

/*
    Add a new node to tail of list
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_NULL_NODE
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_add_last(void *data, int data_size, linked_list_t *list);

/*
    Remove first element of list. If pop_data is not NULL data is copied to it
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_remove_first(linked_list_t *list, void *pop_data);

/*
    Remove element at 'index' of linked_list. If pop_data is not NULL data is copied to it
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_remove_at(int index, linked_list_t *list, void *pop_data);

/*
    Remove last element of list. If pop_data is not NULL data is copied to it
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_remove_last(linked_list_t *list, void *pop_data);

/*
    Remove node of list. If pop_data is not NULL data is copied to it
    Return:
        true: success
        false: fail
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_NULL_NODE
        | LINKED_LIST_STATUS_SUCCESS
*/
bool linked_list_remove_node(node_t *node, linked_list_t *list, void *pop_data);

/*
    Return list element at 'index'
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS
        | LINKED_LIST_STATUS_SUCCESS
*/
void* linked_list_get_at(int index, linked_list_t *list);

/*
    Return list first element
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS
        | LINKED_LIST_STATUS_SUCCESS
*/
void* linked_list_get_first(linked_list_t *list);

/*
    Return list last element
    Set list last_status variable with:
        LINKED_LIST_STATUS_EMPTY_LIST
        | LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS
        | LINKED_LIST_STATUS_SUCCESS
*/
void* linked_list_get_last(linked_list_t *list);

/*
   Call 'callback' function with each list element as argument, starting from last element of list
*/
void linked_list_foreach_reverse(void (*callback)(void *node_data), linked_list_t *list);

/*
   Call 'callback' function with each list element as argument, starting from first element of list
*/
void linked_list_foreach(void (*callback)(void *node_data), linked_list_t *list);

/*
   Free all resources used by list
*/
void linked_list_destroy(linked_list_t *list);

/*
   Return linked_list_status_t status as string
*/
const char* linked_list_status_string(linked_list_status_t status);

#endif /* LINKED_LIST_H */
