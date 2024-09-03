#include "cslib/std/iterator.h"


bool cslib_vector_iterator
(
    cslib_iterator_t *iterator, 
    const cslib_vector_t *vec
)
{
    void **items = (void**)malloc(sizeof(void*) * vec->length);
    size_t c = 0;

    for (size_t i = 0; i < vec->capacity; i++)
    {
        if (vec->items[i] != NULL)
        {
            items[c] = vec->items[i];
            c++;

            if (c == vec->length)
            {
                break;
            }
        }
    }

    iterator->items = items;
    iterator->length = c;

    return true;
}
