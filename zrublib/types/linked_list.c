#include "zrublib/types/linked_list.h"


size_t zrublib_linked_list_length(zrublib_linked_node_t *node)
{
    if (node->next == NULL)
    {
        return 1;
    }

    return 1 + zrublib_linked_list_length(node->next);
}

zrublib_linked_node_t *zrublib_initialize_linked_node(void *value, zrublib_linked_node_t *next)
{
    zrublib_linked_node_t *node = (zrublib_linked_node_t *)ZRUBLIB_MALLOC(sizeof(zrublib_linked_node_t));

    node->value = value;
    node->next = next;

    return node;
}

zrublib_linked_node_t *zrublib_get_last_node(zrublib_linked_node_t *node)
{
    zrublib_linked_node_t *temp = node->next;

    while (temp != NULL)
    {
        node = temp;
        temp = temp->next;
    }

    return node;
}

void zrublib_linked_list_dumbfree(zrublib_linked_node_t *node)
{
    zrublib_linked_node_t *tmp = NULL;

    while (node != NULL)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}
