#ifndef _ZRUB_QUEUE_T
#define _ZRUB_QUEUE_T

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "zrub/pub.h"

typedef struct Queue {
    void **items;
    size_t capacity;
    size_t length;
    size_t fp;      /* Front pointer */
    size_t bp;      /* Back pointer */
} zrub_queue_t;

bool zrub_allocate_queue(zrub_queue_t *queue, size_t capacity);
bool zrub_reallocate_queue(zrub_queue_t *queue, size_t capacity);
void zrub_queue_dumbfree(zrub_queue_t *queue);

bool zrub_queue_push(zrub_queue_t *queue, void *item);
void *zrub_queue_pop(zrub_queue_t *queue);



#endif // _ZRUB_QUEUE_T