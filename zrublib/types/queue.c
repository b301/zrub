#include "queue.h"


bool zrublib_allocate_queue(zrublib_queue_t *queue, size_t capacity)
{
    void **items = (void**)ZRUBLIB_MALLOC(sizeof( void*) * capacity);

    if (items == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < capacity; i++)
    {
        items[i] = NULL;
    }

    queue->items = items;
    queue->capacity = capacity;
    queue->length = 0;
    queue->fp = 0;
    queue->bp = 0;

    return true;
}

// bool zrublib_reallocate_queue(zrublib_queue_t *queue, size_t capacity)
// {
//     // TODO: IMPL
//     return false;
// }

void zrublib_queue_dumbfree(zrublib_queue_t *queue)
{
    free(queue->items);
    free(queue);
}

bool zrublib_queue_push(zrublib_queue_t *queue, void *item)
{
    if (queue->length != 0 && queue->fp == queue->bp)
    {
        fprintf(stderr, "zrublib_queue_push::queue is full\n");
        return false;
    }

    queue->items[queue->fp] = item;
    queue->fp++;
    queue->length++;

    return true;
}

void *zrublib_queue_pop(zrublib_queue_t *queue)
{
    if (queue->length == 0)
    {
        return NULL;   
    }

    void *item = queue->items[queue->bp];
    queue->items[queue->bp] = NULL;
    queue->bp++;
    queue->length--;

    return item;
}
