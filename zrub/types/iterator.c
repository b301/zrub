#include "zrub/types/iterator.h"
#include "iterator.h"


void zrub_vector_iterator(zrub_iterator_t *iterator, const zrub_vector_t *vec)
{
    void **items = (void **)ZRUB_MALLOC(sizeof(void *) * vec->length);
    size_t c = 0;

    for (size_t i = 0; i < vec->capacity; i++)
    {
        if (vec->items[i] != NULL)
        {
            items[c] = vec->items[i];
            c++;

            if (c == vec->length)
            {
                break;
            }
        }
    }

    iterator->items = items;
    iterator->length = c;
}

void zrub_hashmap_keys_iterator(zrub_iterator_t *iterator, const zrub_hashmap_t *map)
{
    void **keys = (void **)ZRUB_MALLOC(sizeof(void *) * map->length);
    size_t c = 0;

    zrub_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == ZRUB_HASHMAP_REGULAR)
            {
                keys[c] = item->key;
                c++;
            }
            else if (item->type == ZRUB_HASHMAP_LINKED_LIST)
            {
                zrub_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    keys[c] = ((zrub_hashmap_item_t *)(node->value))->key;
                    c++;

                    node = node->next;
                }
            }

            if (map->length == c)
            {
                break;
            }
        }
    }

    iterator->items = keys;
    iterator->length = map->length;
}

void zrub_hashmap_values_iterator(zrub_iterator_t *iterator, const zrub_hashmap_t *map)
{
    void **values = (void **)ZRUB_MALLOC(sizeof(void *) * map->length);
    size_t c = 0;

    zrub_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == ZRUB_HASHMAP_REGULAR)
            {
                values[c] = item->value;
                c++;
            }
            else if (item->type == ZRUB_HASHMAP_LINKED_LIST)
            {
                zrub_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    values[c] = ((zrub_hashmap_item_t *)(node->value))->value;
                    c++;

                    node = node->next;
                }
            }

            if (map->length == c)
            {
                break;
            }
        }
    }

    iterator->items = values;
    iterator->length = map->length;
}
