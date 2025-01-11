#include "cslib/types/linked_list.h"

size_t cslib_linked_list_length(cslib_linked_node_t *node)
{
    if (node->next == NULL)
    {
        return 1;
    }

    return 1 + cslib_linked_list_length(node->next);
}

cslib_linked_node_t *cslib_initialize_linked_node(void *value, cslib_linked_node_t *next)
{
    cslib_linked_node_t *node = (cslib_linked_node_t *)malloc(sizeof(cslib_linked_node_t));

    node->value = value;
    node->next = next;

    return node;
}

cslib_linked_node_t *cslib_get_last_node(cslib_linked_node_t *node)
{
    cslib_linked_node_t *temp = node->next;

    while (temp != NULL)
    {
        node = temp;
        temp = temp->next;
    }

    return node;
}

void cslib_linked_list_dumbfree(cslib_linked_node_t *node)
{
    cslib_linked_node_t *tmp = NULL;

    while (node != NULL)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}
