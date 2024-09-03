#ifndef _CSLIB_ITERATOR_H
#define _CSLIB_ITERATOR_H

#include "cslib/pub.h"
#include "cslib/std/vector.h"
#include <stdlib.h>

typedef struct Iterator
{
    void **items;
    size_t length;
} cslib_iterator_t;

bool cslib_vector_iterator(
    cslib_iterator_t *iterator,
    const cslib_vector_t *vec);

#endif // _CSLIB_ITERATOR_H
