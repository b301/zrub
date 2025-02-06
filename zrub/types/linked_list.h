#ifndef _ZRUB_LINKED_LIST_H
#define _ZRUB_LINKED_LIST_H

#include <stdlib.h>

#include "zrub/pub.h"


typedef struct LinkedNode
{
    void *value;
    void *next;
} zrub_linked_node_t;

zrub_linked_node_t *zrub_initialize_linked_node(void *value, zrub_linked_node_t *next);
zrub_linked_node_t *zrub_get_last_node(zrub_linked_node_t *node);

void zrub_linked_list_dumbfree(zrub_linked_node_t *node);
size_t zrub_linked_list_length(zrub_linked_node_t *node);

#endif // _ZRUB_LINKED_LIST_H
