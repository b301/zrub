#include "zrub/types/queue.h"


bool zrub_allocate_queue(zrub_queue_t *queue, size_t capacity)
{
    void **items = (void**)ZRUB_MALLOC(sizeof( void*) * capacity);

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

// bool zrub_reallocate_queue(zrub_queue_t *queue, size_t capacity)
// {
//     // TODO: IMPL
//     return false;
// }

void zrub_queue_dumbfree(zrub_queue_t *queue)
{
    free(queue->items);
    free(queue);
}

bool zrub_queue_push(zrub_queue_t *queue, void *item)
{
    if (queue->length != 0 && queue->fp == queue->bp)
    {
        fprintf(stderr, "zrub_queue_push::queue is full\n");
        return false;
    }

    queue->items[queue->fp] = item;
    queue->fp++;
    queue->length++;

    return true;
}

void *zrub_queue_pop(zrub_queue_t *queue)
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
