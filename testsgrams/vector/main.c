#include "cslib/types/vector.h"
#include "cslib/types/string.h"
#include "cslib/pub.h"
#include <stdio.h>


int main()
{
    printf("testing vector\n");

    /* test 1*/
    {
        cslib_vector_t *v1 = (cslib_vector_t*)malloc(sizeof(cslib_vector_t));

        cslib_allocate_vector(v1, 128);

        // check pushing
        cslib_vector_insert(v1, cslib_string_create("Hello", 128));
        cslib_vector_insert(v1, cslib_string_create("Friendo.", 128));
        cslib_vector_insert(v1, cslib_string_create("What is your problem?", 128));
        cslib_vector_insert(v1, cslib_string_create("Nothing.", 128));

        // check popping
        char *item = (char*)cslib_vector_remove(v1, 2);
        CHECK(strncmp(item, "What is your problem?", 21) == 0 && v1->items[2] == NULL, 
            "test 1: ensuring the right item is popped");

        cslib_vector_naivefree(v1);
        free(item);

        cslib_vector_dumbfree(v1);
    }

    /* test 2 */
    {
        cslib_vector_t *vec = (cslib_vector_t*)malloc(sizeof(cslib_vector_t));
        cslib_allocate_vector(vec, 4);

        CHECK(cslib_vector_insert(vec, cslib_string_create("Hello", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(cslib_vector_insert(vec, cslib_string_create("darkness,", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(cslib_vector_insert(vec, cslib_string_create("my old", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(cslib_vector_insert(vec, cslib_string_create("friend.", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");

        char *should_not_fit = cslib_string_create("talk", 128);
        CHECK(cslib_vector_insert(vec, should_not_fit) == VECTOR_CAPACITY_REACHED, "test 2: ensuring vector is at it's capacity");

        free(should_not_fit);

        cslib_vector_resize(vec, 16);
        // printf("capacity: %zu\n", vec->capacity);

        CHECK(cslib_vector_insert(vec, cslib_string_create("And this is it.", 128)) != VECTOR_CAPACITY_REACHED, "test 2: ensuring item is pushed");
        CHECK(vec->capacity == 16, "test 2: ensuring vector resized to the expected size");

        cslib_vector_naivefree(vec);
        cslib_vector_dumbfree(vec);
    }

    return 0;
}
