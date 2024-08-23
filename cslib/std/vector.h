#ifndef _CSLIB_VECTOR_H
#define _CSLIB_VECTOR_H

#include <stdbool.h>
#include <stdlib.h>   

/*
This is a dumb vector implementation, every item allocated in this must
be deallocated by the user!
*/
typedef struct Vector {
    void **items;
    size_t capacity;
    size_t length;
} cslib_vector_t;


bool cslib_allocate_vector(cslib_vector_t *vec, size_t capacity);
bool cslib_reallocate_vector(cslib_vector_t *vec, size_t capacity);
void cslib_dumbfree_vector(cslib_vector_t *vec);
void cslib_naivefree_vector(cslib_vector_t *vec);

size_t cslib_push_vector(cslib_vector_t *vec, void *item);
void* cslib_pop_vector(cslib_vector_t *vec, size_t index);


#endif // _CSLIB_VECTOR_H
