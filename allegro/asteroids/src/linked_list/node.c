#include "node.h"

node_t *node_create(void *data, int data_size, node_t *previous, node_t *next)
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->data = data;
    new_node->data_size = data_size;
    new_node->next = next;
    new_node->previous = previous;
    return new_node;
}

void* node_destroy(node_t *node)
{
    void* data = node->data;
    free(node);
    return data;
}
