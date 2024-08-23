#ifndef _CSLIB_HASHMAP_H
#define _CSLIB_HASHMAP_H

#include "cslib/std/linked_list.h"

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
void cslib_testfree_hashmap(cslib_hashmap_t *map, bool naive);
void cslib_dumbfree_hashmap(cslib_hashmap_t *map);

bool cslib_set_hashmap(cslib_hashmap_t *map, char *key, void *value);
void* cslib_get_hashmap(cslib_hashmap_t *map, char *key);

size_t cslib_hashmap_hashfunc_1(char *key, size_t capacity);


#endif // _CSLIB_HASHMAP_H
