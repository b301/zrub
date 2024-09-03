#include "cslib/std/iterator.h"
#include "cslib/std/string.h"
#include "cslib/pub.h"


int main()
{
    printf("testing iterator\n");

    /* test 1 */
    {
        cslib_vector_t *vec = ALLOC_OBJECT(cslib_vector_t);
        cslib_allocate_vector(vec, 4);

        cslib_vector_push(vec, cslib_string_create("hello", 8));
        cslib_vector_push(vec, cslib_string_create("there", 8));

        cslib_iterator_t iterator; 
        cslib_vector_iterator(&iterator, vec);

        CHECK(iterator.length == 2, "test 1: ensuring iterator is of length 2");
        CHECK(strncmp(iterator.items[0], "hello", 5) == 0, "test 1: ensuring items are correct");
        CHECK(strncmp(iterator.items[1], "there", 5) == 0, "test 1: ensuring items are correct");

        free(iterator.items);
        cslib_vector_naivefree(vec);
        cslib_vector_dumbfree(vec);
    }


    return 0;
}
