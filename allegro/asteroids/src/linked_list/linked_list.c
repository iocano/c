#include <stdio.h>
#include <string.h>
#include "linked_list.h"

linked_list_t *linked_list_create()
{
    linked_list_t *list = malloc(sizeof(linked_list_t));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

bool linked_list_add_first(void *data, int data_size, linked_list_t *list)
{
    node_t *node = node_create(data, data_size, NULL, list->head);
    if (node == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_NULL_NODE;
        return false;
    }

    if (list->head == NULL)
    {
        list->tail = node;
    }
   else
    {
        list->head->previous = node;
    }
    list->head = node;
    list->count++;

    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return true;
}

bool linked_list_add_at(int index, void *data, int data_size, linked_list_t *list)
{
    if (index < 0 || index > list->count)
    {
        list->last_status = LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS;
        return false;
    }

    if (index == 0)
    {
        return linked_list_add_first(data, data_size, list);
    }

    if (index == list->count)
    {
        return linked_list_add_last(data, data_size, list);
    }

    // index > 0 && index < list->count - 2
    node_t *current = list->head->next; //start on index 1
    for (int i = 1; i < index; i++)
    {
        current = current->next;
    }
    node_t *node = node_create(data, data_size, current->previous, current);
    if (node == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_NULL_NODE;
        return false;
    }

    node->previous->next = node;
    node->next->previous = node;
    list->count++;
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return true;
}

bool linked_list_add_last(void *data, int data_size, linked_list_t *list)
{
    node_t *node = node_create(data, data_size, list->tail, NULL);
    if (node == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_NULL_NODE;
        return false;
    }

    if(list->tail == NULL)
    {
        list->head = node;
    }
    else
    {
        list->tail->next = node;
    }
    list->tail = node;
    list->count++;
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return true;
}

bool linked_list_remove_first(linked_list_t *list, void *pop_data)
{
    if (list->count == 0)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return false;
    }
    return linked_list_remove_node(list->head, list, pop_data);
}

bool linked_list_remove_at(int index, linked_list_t *list, void *pop_data)
{
    if (list->count == 0)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return false;
    }

    if (index < 0 || index >= list->count)
    {
        list->last_status = LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS;
        return false;
    }

    if(index == 0)
    {
        return linked_list_remove_first(list, pop_data);
    }
    if(index == list->count - 1)
    {
        return linked_list_remove_last(list, pop_data);
    }
    // index > 0 && index < list->count - 2
    node_t *current = list->head->next; //start on index 1
    for(int i = 1; i < index; i++)
    {
        current = current->next;
    }
    return linked_list_remove_node(current, list, pop_data);
}

bool linked_list_remove_last(linked_list_t *list, void *pop_data)
{
    if(list->count == 0)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return false;
    }
    return linked_list_remove_node(list->tail, list, pop_data);
}

bool linked_list_remove_node(node_t *node, linked_list_t *list, void *pop_data)
{
    if (list->count == 0)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return false;
    }

    if (node == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_NULL_NODE;
        return false;
    }

    if (node == list->head)
    {
        list->head = node->next;
        if(list->head)
        {
            list->head->previous = NULL;
        }
        else
        {
            list->tail = NULL;
        }
    }
    else if (node == list->tail)
    {
        list->tail = node->previous;
        if(list->tail)
        {
            list->tail->next = NULL;
        }
        else
        {
            list->head = NULL;
        }
    }
    else
    {
        node->previous->next = node->next;
        node->next->previous = node->previous;
    }
    if(pop_data != NULL)
    {
        memcpy(pop_data, node->data, node->data_size);
    }
    free(node->data);
    free(node);
    list->count--;
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return true;
}

void* linked_list_get_at(int index, linked_list_t *list)
{
    if (list->count == 0)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return NULL;
    }

    if (index < 0 || index >= list->count)
    {
        list->last_status = LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS;
        return NULL;
    }

    if(index == 0)
    {
        return linked_list_get_first(list);
    }

    if(index == list->count - 1)
    {
        return linked_list_get_last(list);
    }

    // index > 0 && index < list->count - 2
    node_t *current = list->head->next;
    for(int i = 1; i < index && current != NULL; i++)
    {
        current = current->next;
    }
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return current->data;
}

void* linked_list_get_first(linked_list_t *list)
{
    if (list->head == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return NULL;
    }
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return list->head->data;
}

void* linked_list_get_last(linked_list_t *list)
{
    if (list->tail == NULL)
    {
        list->last_status = LINKED_LIST_STATUS_EMPTY_LIST;
        return NULL;
    }
    list->last_status = LINKED_LIST_STATUS_SUCCESS;
    return list->tail->data;
}

const char* linked_list_status_string(linked_list_status_t status)
{
    switch (status)
    {
        case LINKED_LIST_STATUS_SUCCESS: return "OPERATION SUCCESS";
        case LINKED_LIST_STATUS_EMPTY_LIST: return "LIST IS EMPTY";
        case LINKED_LIST_STATUS_NULL_NODE: return "NODE IS NULL";
        case LINKED_LIST_STATUS_INDEX_OUT_OF_BOUNDS: return "INDEX OUT OF BOUNDS";
        default:
            return "STATUS UNKNOWN";
    }
}

void linked_list_foreach(void (*callback)(void *node_data), linked_list_t *list)
{
    for(node_t *current = list->head; current != NULL; current = current->next)
    {
        callback(current->data);
    }
}

void linked_list_foreach_reverse(void (*callback)(void *node_data), linked_list_t *list)
{
    for(node_t *current = list->tail; current != NULL; current = current->previous)
    {
        callback(current->data);
    }
}

void linked_list_destroy(linked_list_t *list)
{
    node_t *current = list->head;
    node_t *next = NULL;
    while(current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}
