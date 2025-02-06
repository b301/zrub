#include "zrublib/types/iterator.h"
#include "iterator.h"


void zrublib_vector_iterator(zrublib_iterator_t *iterator, const zrublib_vector_t *vec)
{
    void **items = (void **)ZRUBLIB_MALLOC(sizeof(void *) * vec->length);
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

void zrublib_hashmap_keys_iterator(zrublib_iterator_t *iterator, const zrublib_hashmap_t *map)
{
    void **keys = (void **)ZRUBLIB_MALLOC(sizeof(void *) * map->length);
    size_t c = 0;

    zrublib_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == HASHMAP_REGULAR)
            {
                keys[c] = item->key;
                c++;
            }
            else if (item->type == HASHMAP_LINKED_LIST)
            {
                zrublib_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    keys[c] = ((zrublib_hashmap_item_t *)(node->value))->key;
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

void zrublib_hashmap_values_iterator(zrublib_iterator_t *iterator, const zrublib_hashmap_t *map)
{
    void **values = (void **)ZRUBLIB_MALLOC(sizeof(void *) * map->length);
    size_t c = 0;

    zrublib_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == HASHMAP_REGULAR)
            {
                values[c] = item->value;
                c++;
            }
            else if (item->type == HASHMAP_LINKED_LIST)
            {
                zrublib_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    values[c] = ((zrublib_hashmap_item_t *)(node->value))->value;
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
