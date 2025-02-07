#ifndef _ZRUB_VECTOR_H
#define _ZRUB_VECTOR_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include "zrub/pub.h"

/*
This is a dumb vector implementation, every item allocated in this must
be deallocated by the user!
*/
typedef struct Vector
{
    void **items;
    size_t capacity;
    size_t length;
} zrub_vector_t;

#define ZRUB_VECTOR_CAPACITY_REACHED        -1
#define ZRUB_VECTOR_UNDEFINED_BEHAVIOR      -2

bool zrub_allocate_vector(zrub_vector_t *vec, size_t capacity);
bool zrub_reallocate_vector(zrub_vector_t *vec, size_t capacity);
void zrub_vector_dumbfree(zrub_vector_t *vec);
void zrub_vector_naivefree(zrub_vector_t *vec);

bool zrub_vector_resize(zrub_vector_t *vec, size_t new_capacity);

ssize_t zrub_vector_insert(zrub_vector_t *vec, void *item);
void *zrub_vector_remove(zrub_vector_t *vec, size_t index);
void *zrub_vector_get(zrub_vector_t *vec, ssize_t index);


#endif // _ZRUB_VECTOR_H
