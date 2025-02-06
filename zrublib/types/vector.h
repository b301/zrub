#ifndef _ZRUBLIB_VECTOR_H
#define _ZRUBLIB_VECTOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include "zrublib/pub.h"

/*
This is a dumb vector implementation, every item allocated in this must
be deallocated by the user!
*/
typedef struct Vector
{
    void **items;
    size_t capacity;
    size_t length;
} zrublib_vector_t;

#define VECTOR_CAPACITY_REACHED -1
#define VECTOR_UNDEFINED_BEHAVIOR -2

bool zrublib_allocate_vector(zrublib_vector_t *vec, size_t capacity);
bool zrublib_reallocate_vector(zrublib_vector_t *vec, size_t capacity);
void zrublib_vector_dumbfree(zrublib_vector_t *vec);
void zrublib_vector_naivefree(zrublib_vector_t *vec);

bool zrublib_vector_resize(zrublib_vector_t *vec, size_t new_capacity);

ssize_t zrublib_vector_insert(zrublib_vector_t *vec, void *item);
void *zrublib_vector_remove(zrublib_vector_t *vec, size_t index);
void *zrublib_vector_get(zrublib_vector_t *vec, ssize_t index);


#endif // _ZRUBLIB_VECTOR_H
