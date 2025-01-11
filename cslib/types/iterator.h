#ifndef _CSLIB_ITERATOR_H
#define _CSLIB_ITERATOR_H

#include "cslib/pub.h"
#include "cslib/basic_types.h"
#include <stdlib.h>

typedef struct Iterator
{
    void **items;
    size_t length;
} cslib_iterator_t;

void cslib_vector_iterator(cslib_iterator_t *iterator, const cslib_vector_t *vec);
void cslib_hashmap_keys_iterator(cslib_iterator_t *iterator, const cslib_hashmap_t *map);
void cslib_hashmap_values_iterator(cslib_iterator_t *iterator, const cslib_hashmap_t *map);

#endif // _CSLIB_ITERATOR_H
