#ifndef _CSLIB_HASHMAP_H
#define _CSLIB_HASHMAP_H

#include "cslib/std/linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum HashmapItemType {
    REGULAR,
    LINKED_LIST
} cslib_hashmap_item_type_t;

typedef struct HashmapItem {
    char *key;
    void *value;
    cslib_hashmap_item_type_t type;
} cslib_hashmap_item_t;

typedef struct LinkedHashMap {
    cslib_hashmap_item_t **items;
    size_t capacity;
} cslib_hashmap_t;


bool cslib_allocate_hashmap(cslib_hashmap_t *map, size_t capacity);
void cslib_hashmap_naivefree(cslib_hashmap_t *map, bool naive);
void cslib_hashmap_dumbfree(cslib_hashmap_t *map);

bool cslib_hashmap_set(cslib_hashmap_t *map, char *key, void *value);
void* cslib_get_hashmap(cslib_hashmap_t *map, char *key);

bool cslib_hashmap_remove(cslib_hashmap_t *map, char *key);

size_t cslib_hashmap_hashfunc_1(char *key, size_t capacity);

void cslib_hashmap_print(cslib_hashmap_t *map);


#endif // _CSLIB_HASHMAP_H
