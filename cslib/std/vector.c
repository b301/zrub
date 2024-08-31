#include "cslib/std/vector.h"


bool cslib_allocate_vector(cslib_vector_t *vec, size_t capacity)
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

    vec->items = items;
    vec->capacity = capacity;
    vec->length = 0;

    return true;
}

bool cslib_reallocate_vector(cslib_vector_t *vec, size_t capacity)
{
    // TODO:
    return false;
}

void cslib_vector_dumbfree(cslib_vector_t *vec)
{
    free(vec->items);
    free(vec);
}

void cslib_vector_naivefree(cslib_vector_t *vec)
{
    for (size_t i = 0; i < vec->capacity; i++)
    {
        if (vec->items[i] != NULL)
        {
            free(vec->items[i]);
            vec->items[i] = NULL;
        }
    }
}

ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)
{
    size_t vec_size = vec->capacity;

    if (vec->length == vec->capacity)
    {
        return VECTOR_CAPACITY_REACHED;
    }

    for (ssize_t i = 0; i < vec_size; i++)
    {
        if (vec->items[i] == NULL)
        {
            vec->items[i] = item;
            vec->length++;
            return i;
        }
    }
}

void* cslib_vector_pop(cslib_vector_t *vec, size_t index)
{
    if (vec->capacity <= index)
    {
        return NULL;
    }

    if (vec->items[index] == NULL)
    {
        return NULL;
    }

    void *item = vec->items[index];
    vec->items[index] = NULL;
    vec->length--;

    return item;
}
