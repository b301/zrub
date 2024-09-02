#include "cslib/std/hashmap.h"
#include "hashmap.h"


bool cslib_allocate_hashmap(cslib_hashmap_t *map, size_t capacity)
{
    cslib_hashmap_item_t **items = (cslib_hashmap_item_t**)malloc(
        sizeof(cslib_hashmap_item_t*) * capacity);

    if (items == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < capacity; i++)
    {
        items[i] = NULL;
    }

    map->items = items;
    map->capacity = capacity;
    map->length = 0;

    return true;
}

void cslib_hashmap_naivefree(cslib_hashmap_t *map, bool naive)
{
    cslib_hashmap_item_t *item, *node_item;
    cslib_linked_node_t *node, *temp;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == REGULAR)
            {
                if (naive)
                {
                    free(item->key);
                    free(item->value);
                }
            }

            else if (item->type == LINKED_LIST)
            {
                /* free the nodes */
                node = item->value;

                while (node != NULL)
                {
                    temp = node;
                    node = node->next;

                    node_item = temp->value;

                    if (naive)
                    {
                        free(node_item->key);
                        free(node_item->value);
                    }

                    free(temp->value);
                    free(temp);
                }
            }

            free(item);
        }
    }
}

void cslib_hashmap_dumbfree(cslib_hashmap_t *map)
{
    free(map->items);
    free(map);
}

cslib_hashmap_item_t *_cslib_hashmap_item(char *key, void *value, cslib_hashmap_item_type_t type)
{
    cslib_hashmap_item_t *item = ALLOC_OBJECT(cslib_hashmap_item_t);

    item->key = key;
    item->value = value;
    item->type = type;

    return item;
}

bool cslib_hashmap_resize(cslib_hashmap_t *map, size_t new_capacity)
{
    cslib_hashmap_item_t **items = (cslib_hashmap_item_t**)malloc(
        sizeof(cslib_hashmap_item_t*) * new_capacity);

    

    return false;
}

bool cslib_hashmap_set(cslib_hashmap_t *map, char *key, void *value)
{
    size_t hashed = cslib_hashmap_hashfunc_1(key, map->capacity);

    /* If it is to replace, maybe allocate only in later parts .. later optimization */
    cslib_hashmap_item_t *item = _cslib_hashmap_item(key, value, REGULAR);

    if (map->items[hashed] == NULL)
    {
        map->items[hashed] = item;
        map->length++;

        return true;
    }

    cslib_hashmap_item_t *set_item = map->items[hashed];

    if (set_item->type == REGULAR)
    {
        void *s1 = cslib_initialize_linked_node(set_item, NULL);
        void *s2 = cslib_initialize_linked_node(item, s1);

        /* create item for the linked list */
        cslib_hashmap_item_t *list_item = _cslib_hashmap_item(NULL, s2, LINKED_LIST);
        map->items[hashed] = list_item;
        map->length++;

        return true;
    }
    else if (set_item->type == LINKED_LIST)
    {
        void *s1 = cslib_initialize_linked_node(item, set_item->value);
        set_item->value = s1;

        map->length++;

        return true;
    }

    free(item);

    return false;
}

void* cslib_get_hashmap(cslib_hashmap_t *map, char *key)
{
    size_t hashed = cslib_hashmap_hashfunc_1(key, map->capacity);
    cslib_hashmap_item_t *item = map->items[hashed];

    if (item == NULL)
    {
        return NULL;
    }

    if (item->type == REGULAR)
    {
        if (strncmp( key, item->key, strlen(key) ) == 0)
        {
            return item->value;
        }

        return NULL;
    }
    else if (item->type == LINKED_LIST)
    {
        cslib_hashmap_item_t *node_item;
        cslib_linked_node_t *node = item->value;

        while (node != NULL)
        {
            node_item = node->value;

            if (strncmp( key, node_item->key, strlen(key) ) == 0)
            {
                return node_item->value;
            }

            node = node->next;
        }
    }

    return NULL;
}

size_t cslib_hashmap_hashfunc_1(char *key, size_t capacity)
{
    size_t hash = 0;

    for (size_t i = 0; i < strlen(key); i++)
    {
        hash += key[i];
        hash %= capacity;
    }

    return hash;
}

void cslib_hashmap_print(cslib_hashmap_t *map)
{
    cslib_hashmap_item_t *item;
    cslib_linked_node_t *node;

    printf("{");
    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == REGULAR)
            {
                printf("\"%s\": \"%s\",",
                    item->key,
                    item->value
                );
            }

            else if (item->type == LINKED_LIST)
            {
                node = item->value;

                while (node != NULL)
                {
                    printf("\"%s\": \"%s\",",
                        ((cslib_hashmap_item_t*)(node->value))->key,
                        ((cslib_hashmap_item_t*)(node->value))->value
                    );

                    node = node->next;
                }
            }

            else
            {
                printf("Type not implemented.\n");
            }
        }
    }

    printf("}\n");
}

bool cslib_hashmap_remove(cslib_hashmap_t *map, char *key)
{
    size_t hash = cslib_hashmap_hashfunc_1(key, map->capacity);

    if (map->items[hash] == NULL)
    {
        return false;
    }

    cslib_hashmap_item_t *item = map->items[hash];

    free(item->key);
    free(item->value);
    free(item);

    map->items[hash] = NULL;
    map->length--;
}
