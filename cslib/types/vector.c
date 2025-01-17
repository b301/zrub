#include "cslib/types/vector.h"
#include "vector.h"


bool cslib_allocate_vector(cslib_vector_t *vec, size_t capacity)
{
    void **items = (void**)CSLIB_MALLOC(sizeof( void*) * capacity);

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
    // TODO: IMPL
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

bool cslib_vector_resize(cslib_vector_t *vec, size_t new_capacity)
{
    void **items = (void**)CSLIB_MALLOC(sizeof( void*) * new_capacity);

    if (items == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < vec->capacity; i++)
    {
        items[i] = vec->items[i];
    }

    for (size_t i = vec->capacity; i < new_capacity;i ++)
    {
        items[i] = NULL;
    }

    free(vec->items);

    vec->items = items;
    vec->capacity = new_capacity;

    return true;
}

ssize_t cslib_vector_insert(cslib_vector_t *vec, void *item)
{
    size_t vec_size = vec->capacity;

    if (vec->length == vec->capacity)
    {
        return VECTOR_CAPACITY_REACHED;
    }

    for (size_t i = 0; i < vec_size; i++)
    {
        if (vec->items[i] == NULL)
        {
            vec->items[i] = item;
            vec->length++;
            return i;
        }
    }

    fprintf(stderr, "ssize_t cslib_vector_insert(cslib_vector_t *vec, void *item) reached undefined behavior");
    return VECTOR_UNDEFINED_BEHAVIOR;
}

void* cslib_vector_remove(cslib_vector_t *vec, size_t index)
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

void *cslib_vector_get(cslib_vector_t *vec, ssize_t index)
{
    if (vec->length < index ||
        (index < 0 && vec->length < index * -1)) 
    {
        return NULL;
    }

    return vec->items[index];
}
