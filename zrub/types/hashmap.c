#include "zrub/types/hashmap.h"
#include "hashmap.h"


bool zrub_allocate_hashmap(zrub_hashmap_t *map, size_t capacity)
{
    zrub_hashmap_item_t **items = (zrub_hashmap_item_t**)ZRUB_MALLOC(
        sizeof(zrub_hashmap_item_t*) * capacity);

    if (items == NULL)
    {
        ZRUB_LOG_ERROR("failed to allocate hashmap");
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

void zrub_hashmap_naivefree(zrub_hashmap_t *map, bool naive)
{
    zrub_hashmap_item_t *item, *node_item;
    zrub_linked_node_t *node, *temp;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == ZRUB_HASHMAP_REGULAR)
            {
                if (naive)
                {
                    free(item->key);
                    free(item->value);
                }
            }

            else if (item->type == ZRUB_HASHMAP_LINKED_LIST)
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

void zrub_hashmap_dumbfree(zrub_hashmap_t *map)
{
    free(map->items);
    free(map);
}

zrub_hashmap_item_t *_zrub_hashmap_item(char *key, void *value, zrub_hashmap_item_type_t type)
{
    zrub_hashmap_item_t *item = ALLOC_OBJECT(zrub_hashmap_item_t);

    item->key = key;
    item->value = value;
    item->type = type;

    return item;
}

/* copies map content into a new map and frees the old map */
bool zrub_hashmap_resize(zrub_hashmap_t *map, size_t new_capacity)
{
    zrub_hashmap_t *new_map = ALLOC_OBJECT(zrub_hashmap_t);

    if (!zrub_allocate_hashmap(new_map, new_capacity))
    {
        return false;
    }

    zrub_hashmap_item_t *item = NULL;
    zrub_hashmap_item_t *tmp = NULL;

    for (size_t i = 0; i < map->capacity; i++)
    {
        if (map->items[i] != NULL)
        {
            item = map->items[i];

            if (item->type == ZRUB_HASHMAP_REGULAR)
            {
                _zrub_hashmap_set(new_map, item);
            }
            else if (item->type == ZRUB_HASHMAP_LINKED_LIST)
            {
                zrub_linked_node_t *root = item->value;

                while (root != NULL)
                {
                    tmp = root->value;
                    _zrub_hashmap_set(new_map, tmp);

                    root = root->next;
                }

                zrub_linked_list_dumbfree(item->value);
                free(item);
            }
        }
    }    

    free(map->items);

    map->capacity = new_capacity;
    map->items = new_map->items;
    map->length = new_map->length;

    free(new_map);

    return true;
}

bool zrub_hashmap_set(zrub_hashmap_t *map, char *key, void *value)
{
    size_t hashed = zrub_hashmap_hashfunc_1(key, map->capacity);

    /* If it is to replace, maybe allocate only in later parts .. later optimization */
    zrub_hashmap_item_t *item = _zrub_hashmap_item(key, value, ZRUB_HASHMAP_REGULAR);

    if (map->items[hashed] == NULL)
    {
        map->items[hashed] = item;
        map->length++;

        return true;
    }

    zrub_hashmap_item_t *set_item = map->items[hashed];

    if (set_item->type == ZRUB_HASHMAP_REGULAR)
    {
        void *s1 = zrub_initialize_linked_node(set_item, NULL);
        void *s2 = zrub_initialize_linked_node(item, s1);

        /* create item for the linked list */
        zrub_hashmap_item_t *list_item = _zrub_hashmap_item(NULL, s2, ZRUB_HASHMAP_LINKED_LIST);
        map->items[hashed] = list_item;
        map->length++;

        return true;
    }
    else if (set_item->type == ZRUB_HASHMAP_LINKED_LIST)
    {
        void *s1 = zrub_initialize_linked_node(item, set_item->value);
        set_item->value = s1;

        map->length++;

        return true;
    }

    free(item);

    return false;
}

void _zrub_hashmap_set(zrub_hashmap_t *map, zrub_hashmap_item_t *item)
{
    if (item->type != ZRUB_HASHMAP_REGULAR)
    {
        return;
    }

    size_t hashed = zrub_hashmap_hashfunc_1(item->key, map->capacity);

    if (map->items[hashed] == NULL)
    {
        map->items[hashed] = item;
        map->length++;

        return;
    }

    zrub_hashmap_item_t *set_item = map->items[hashed];

    if (set_item->type == ZRUB_HASHMAP_REGULAR)
    {
        void *s1 = zrub_initialize_linked_node(set_item, NULL);
        void *s2 = zrub_initialize_linked_node(item, s1);

        /* create item for the linked list */
        zrub_hashmap_item_t *list_item = _zrub_hashmap_item(NULL, s2, ZRUB_HASHMAP_LINKED_LIST);
        map->items[hashed] = list_item;
        map->length++;
    }
    else if (set_item->type == ZRUB_HASHMAP_LINKED_LIST)
    {
        void *s1 = zrub_initialize_linked_node(item, set_item->value);
        set_item->value = s1;

        map->length++;
    }
}

void* zrub_get_hashmap(zrub_hashmap_t *map, char *key)
{
    size_t hashed = zrub_hashmap_hashfunc_1(key, map->capacity);
    zrub_hashmap_item_t *item = map->items[hashed];

    if (item == NULL)
    {
        return NULL;
    }

    if (item->type == ZRUB_HASHMAP_REGULAR)
    {
        if (strncmp( key, item->key, strlen(key) ) == 0)
        {
            return item->value;
        }

        return NULL;
    }
    else if (item->type == ZRUB_HASHMAP_LINKED_LIST)
    {
        zrub_hashmap_item_t *node_item;
        zrub_linked_node_t *node = item->value;

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

size_t zrub_hashmap_hashfunc_1(char *key, size_t capacity)
{
    size_t hash = 0;

    for (size_t i = 0; i < strlen(key); i++)
    {
        hash += key[i];
        hash %= capacity;
    }

    return hash;
}

bool zrub_hashmap_remove(zrub_hashmap_t *map, char *key)
{
    if (!map)
    {
        
    }

    size_t hash = zrub_hashmap_hashfunc_1(key, map->capacity);

    if (map->items[hash] == NULL)
    {
        return false;
    }

    zrub_hashmap_item_t *item = map->items[hash];

    free(item->key);
    free(item->value);
    free(item);

    map->items[hash] = NULL;
    map->length--;

    return true;
}
