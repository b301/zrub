#ifndef _ZRUB_ITERATOR_H
#define _ZRUB_ITERATOR_H

#include "zrub/pub.h"
#include "zrub/basic_types.h"
#include <stdlib.h>

#include "zrub/pub.h"

/* only need to free the items array, not the items themselves! */
typedef struct Iterator
{
    void **items;
    size_t length;
} zrub_iterator_t;

void zrub_vector_iterator(zrub_iterator_t *iterator, const zrub_vector_t *vec);
void zrub_hashmap_keys_iterator(zrub_iterator_t *iterator, const zrub_hashmap_t *map);
void zrub_hashmap_values_iterator(zrub_iterator_t *iterator, const zrub_hashmap_t *map);

#endif // _ZRUB_ITERATOR_H
