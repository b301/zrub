#ifndef _ZRUBLIB_QUEUE_T
#define _ZRUBLIB_QUEUE_T

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "zrublib/pub.h"

typedef struct Queue {
    void **items;
    size_t capacity;
    size_t length;
    size_t fp;      /* Front pointer */
    size_t bp;      /* Back pointer */
} zrublib_queue_t;

bool zrublib_allocate_queue(zrublib_queue_t *queue, size_t capacity);
bool zrublib_reallocate_queue(zrublib_queue_t *queue, size_t capacity);
void zrublib_queue_dumbfree(zrublib_queue_t *queue);

bool zrublib_queue_push(zrublib_queue_t *queue, void *item);
void *zrublib_queue_pop(zrublib_queue_t *queue);



#endif // _ZRUBLIB_QUEUE_T