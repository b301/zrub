#ifndef _ZRUBLIB_LINKED_LIST_H
#define _ZRUBLIB_LINKED_LIST_H

#include <stdlib.h>

#include "zrublib/pub.h"


typedef struct LinkedNode
{
    void *value;
    void *next;
} zrublib_linked_node_t;

zrublib_linked_node_t *zrublib_initialize_linked_node(void *value, zrublib_linked_node_t *next);
zrublib_linked_node_t *zrublib_get_last_node(zrublib_linked_node_t *node);

void zrublib_linked_list_dumbfree(zrublib_linked_node_t *node);
size_t zrublib_linked_list_length(zrublib_linked_node_t *node);

#endif // _ZRUBLIB_LINKED_LIST_H
