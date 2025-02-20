#ifndef _ZRUB_HASHMAP_H
#define _ZRUB_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "zrub/types/linked_list.h"
#include "zrub/pub.h"

typedef enum HashmapItemType
{
    ZRUB_HASHMAP_REGULAR,
    ZRUB_HASHMAP_LINKED_LIST
} zrub_hashmap_item_type_t;

typedef struct HashmapItem
{
    char *key;
    void *value;
    zrub_hashmap_item_type_t type;
} zrub_hashmap_item_t;

typedef struct LinkedHashMap
{
    zrub_hashmap_item_t **items;
    size_t capacity;
    size_t length;
} zrub_hashmap_t;

bool zrub_allocate_hashmap(zrub_hashmap_t *map, size_t capacity);
void zrub_hashmap_naivefree(zrub_hashmap_t *map, bool naive);
void zrub_hashmap_dumbfree(zrub_hashmap_t *map);

bool zrub_hashmap_resize(zrub_hashmap_t *map, size_t new_capacity);

zrub_hashmap_item_t *_zrub_hashmap_item(char *key, void *value, zrub_hashmap_item_type_t type);

bool zrub_hashmap_set(zrub_hashmap_t *map, char *key, void *value);
void _zrub_hashmap_set(zrub_hashmap_t *map, zrub_hashmap_item_t *item);
void *zrub_get_hashmap(zrub_hashmap_t *map, char *key);

bool zrub_hashmap_remove(zrub_hashmap_t *map, char *key);

size_t zrub_hashmap_hashfunc_1(char *key, size_t capacity);

#endif // _ZRUB_HASHMAP_H
