#ifndef NODE_H
#define NODE_H

#include <stdlib.h>

typedef struct node
{
    void *data;
    int data_size;
    struct node *next;
    struct node *previous;
} node_t;

node_t *node_create(void *data, int data_size, node_t *previous, node_t *next);

void* node_destroy(node_t *node);

#endif /* NODE_H */
