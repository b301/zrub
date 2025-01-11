#include "queue.h"

bool cslib_allocate_queue(cslib_queue_t *queue, size_t capacity)
{
    void **items = (void**)malloc(sizeof( void*) * capacity);

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

bool cslib_reallocate_queue(cslib_queue_t *queue, size_t capacity)
{
    return false;
}

void cslib_queue_dumbfree(cslib_queue_t *queue)
{
    free(queue->items);
    free(queue);
}

bool cslib_queue_push(cslib_queue_t *queue, void *item)
{
    if (queue->length != 0 && queue->fp == queue->bp)
    {
        fprintf(stderr, "cslib_queue_push::queue is full\n");
        return false;
    }

    queue->items[queue->fp] = item;
    queue->fp++;
    queue->length++;

    return true;
}

void *cslib_queue_pop(cslib_queue_t *queue)
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
