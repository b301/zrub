#ifndef _CSLIB_VECTOR_H
#define _CSLIB_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

/*
This is a dumb vector implementation, every item allocated in this must
be deallocated by the user!
*/
typedef struct Vector {
    void **items;
    size_t capacity;
    size_t length;
} cslib_vector_t;

#define VECTOR_CAPACITY_REACHED -1


bool cslib_allocate_vector(cslib_vector_t *vec, size_t capacity);
bool cslib_reallocate_vector(cslib_vector_t *vec, size_t capacity);
void cslib_vector_dumbfree(cslib_vector_t *vec);
void cslib_vector_naivefree(cslib_vector_t *vec);

bool cslib_vector_resize(cslib_vector_t *vec, size_t new_capacity);

ssize_t cslib_vector_push(cslib_vector_t *vec, void *item);
void* cslib_vector_pop(cslib_vector_t *vec, size_t index);


#endif // _CSLIB_VECTOR_H
