#include "cslib/std/iterator.h"
#include "iterator.h"

void cslib_vector_iterator(cslib_iterator_t *iterator, const cslib_vector_t *vec)
{
    void **items = (void **)malloc(sizeof(void *) * vec->length);
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

void cslib_hashmap_keys_iterator(cslib_iterator_t *iterator, const cslib_hashmap_t *map)
{
    void **keys = (void **)malloc(sizeof(void *) * map->length);
    size_t c = 0;

    cslib_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == REGULAR)
            {
                keys[c] = item->key;
                c++;
            }
            else if (item->type == LINKED_LIST)
            {
                cslib_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    keys[c] = ((cslib_hashmap_item_t *)(node->value))->key;
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

void cslib_hashmap_values_iterator(cslib_iterator_t *iterator, const cslib_hashmap_t *map)
{
    void **values = (void **)malloc(sizeof(void *) * map->length);
    size_t c = 0;

    cslib_hashmap_item_t *item;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == REGULAR)
            {
                values[c] = item->value;
                c++;
            }
            else if (item->type == LINKED_LIST)
            {
                cslib_linked_node_t *node = item->value;

                while (node != NULL)
                {
                    values[c] = ((cslib_hashmap_item_t *)(node->value))->value;
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
