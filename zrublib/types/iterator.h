#ifndef _ZRUBLIB_ITERATOR_H
#define _ZRUBLIB_ITERATOR_H

#include "zrublib/pub.h"
#include "zrublib/basic_types.h"
#include <stdlib.h>

#include "zrublib/pub.h"

/* only need to free the items array, not the items themselves! */
typedef struct Iterator
{
    void **items;
    size_t length;
} zrublib_iterator_t;

void zrublib_vector_iterator(zrublib_iterator_t *iterator, const zrublib_vector_t *vec);
void zrublib_hashmap_keys_iterator(zrublib_iterator_t *iterator, const zrublib_hashmap_t *map);
void zrublib_hashmap_values_iterator(zrublib_iterator_t *iterator, const zrublib_hashmap_t *map);

#endif // _ZRUBLIB_ITERATOR_H
