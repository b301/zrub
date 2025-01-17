#include "cslib/types/vector.h"
#include "cslib/types/string.h"
#include "cslib/pub.h"
#include <stdio.h>


int main()
{
    printf("testing vector\n");

    /* test 1: ssize_t cslib_vector_insert(cslib_vector_t *vec, void *item) */
    {
        cslib_vector_t *v1 = (cslib_vector_t*)CSLIB_MALLOC(sizeof(cslib_vector_t));

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
        cslib_vector_dumbfree(v1);
    }

    /* test 2: test if vector_capacity_reached is reached */
    {
        cslib_vector_t *vec = (cslib_vector_t*)CSLIB_MALLOC(sizeof(cslib_vector_t));
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

    /* test 3: void *cslib_vector_get(cslib_vector_t *vec, ssize_t index) */
    {
        cslib_vector_t *vec = (cslib_vector_t*)CSLIB_MALLOC(sizeof(cslib_vector_t));
        cslib_allocate_vector(vec, 4);

        cslib_vector_insert(vec, cslib_string_create("Hello", 128));
        cslib_vector_insert(vec, cslib_string_create("Friendo.", 128));
        cslib_vector_insert(vec, cslib_string_create("What is your problem?", 128));
        cslib_vector_insert(vec, cslib_string_create("Nothing.", 128));

        void *item = cslib_vector_get(vec, 2);

        printf("item `%s`\n", item);

        CHECK(item != NULL, "test 3: ensuring the item is not null");
        CHECK(!strncmp(item, "What is your problem?", strlen(item)), "test 3: ensuring the correct item is selected");

        cslib_vector_naivefree(vec);
        cslib_vector_dumbfree(vec);
    }

    return 0;
}
