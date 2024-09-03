#ifndef _CSLIB_LINKED_LIST_H
#define _CSLIB_LINKED_LIST_H

#include <stdlib.h>

typedef struct LinkedNode
{
    void *value;
    void *next;
} cslib_linked_node_t;

cslib_linked_node_t *cslib_initialize_linked_node(void *value, cslib_linked_node_t *next);
cslib_linked_node_t *cslib_get_last_node(cslib_linked_node_t *node);

void cslib_linked_list_dumbfree(cslib_linked_node_t *node);
size_t cslib_linked_list_length(cslib_linked_node_t *node);

#endif // _CSLIB_LINKED_LIST_H
