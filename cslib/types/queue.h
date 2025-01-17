#ifndef _CSLIB_QUEUE_T
#define _CSLIB_QUEUE_T

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cslib/pub.h"

typedef struct Queue {
    void **items;
    size_t capacity;
    size_t length;
    size_t fp;      /* Front pointer */
    size_t bp;      /* Back pointer */
} cslib_queue_t;

bool cslib_allocate_queue(cslib_queue_t *queue, size_t capacity);
bool cslib_reallocate_queue(cslib_queue_t *queue, size_t capacity);
void cslib_queue_dumbfree(cslib_queue_t *queue);

bool cslib_queue_push(cslib_queue_t *queue, void *item);
void *cslib_queue_pop(cslib_queue_t *queue);



#endif // _CSLIB_QUEUE_T