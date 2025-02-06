#ifndef _ZRUBLIB_HASHMAP_H
#define _ZRUBLIB_HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "zrublib/pub.h"
#include "zrublib/types/linked_list.h"


typedef enum HashmapItemType
{
    HASHMAP_REGULAR,
    HASHMAP_LINKED_LIST
} zrublib_hashmap_item_type_t;

typedef struct HashmapItem
{
    char *key;
    void *value;
    zrublib_hashmap_item_type_t type;
} zrublib_hashmap_item_t;

typedef struct LinkedHashMap
{
    zrublib_hashmap_item_t **items;
    size_t capacity;
    size_t length;
} zrublib_hashmap_t;

bool zrublib_allocate_hashmap(zrublib_hashmap_t *map, size_t capacity);
void zrublib_hashmap_naivefree(zrublib_hashmap_t *map, bool naive);
void zrublib_hashmap_dumbfree(zrublib_hashmap_t *map);

bool zrublib_hashmap_resize(zrublib_hashmap_t *map, size_t new_capacity);

zrublib_hashmap_item_t *_zrublib_hashmap_item(char *key, void *value, zrublib_hashmap_item_type_t type);

bool zrublib_hashmap_set(zrublib_hashmap_t *map, char *key, void *value);
void _zrublib_hashmap_set(zrublib_hashmap_t *map, zrublib_hashmap_item_t *item);
void *zrublib_get_hashmap(zrublib_hashmap_t *map, char *key);

bool zrublib_hashmap_remove(zrublib_hashmap_t *map, char *key);

size_t zrublib_hashmap_hashfunc_1(char *key, size_t capacity);

void zrublib_hashmap_print(zrublib_hashmap_t *map);
void zrublib_hashmap_print_verbose(zrublib_hashmap_t *map);

#endif // _ZRUBLIB_HASHMAP_H
