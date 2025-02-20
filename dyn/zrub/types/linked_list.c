#include "zrub/types/linked_list.h"


size_t zrub_linked_list_length(zrub_linked_node_t *node)
{
    if (node->next == NULL)
    {
        return 1;
    }

    return 1 + zrub_linked_list_length(node->next);
}

zrub_linked_node_t *zrub_initialize_linked_node(void *value, zrub_linked_node_t *next)
{
    zrub_linked_node_t *node = (zrub_linked_node_t *)ZRUB_MALLOC(sizeof(zrub_linked_node_t));

    node->value = value;
    node->next = next;

    return node;
}

zrub_linked_node_t *zrub_get_last_node(zrub_linked_node_t *node)
{
    zrub_linked_node_t *temp = node->next;

    while (temp != NULL)
    {
        node = temp;
        temp = temp->next;
    }

    return node;
}

void zrub_linked_list_dumbfree(zrub_linked_node_t *node)
{
    zrub_linked_node_t *tmp = NULL;

    while (node != NULL)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}
