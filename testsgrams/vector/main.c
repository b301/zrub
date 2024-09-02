#include "cslib/std/vector.h"
#include "cslib/std/string.h"
#include "cslib/pub.h"
#include <stdio.h>


int main()
{
    {
        cslib_vector_t *v1 = (cslib_vector_t*)malloc(sizeof(cslib_vector_t));

        cslib_allocate_vector(v1, 128);

        // check pushing
        printf("%d | %d | %d | %d\n",
            cslib_vector_push(v1, cslib_string_create("Hello", 128)),
            cslib_vector_push(v1, cslib_string_create("Friendo.", 128)),
            cslib_vector_push(v1, cslib_string_create("What is your problem?", 128)),
            cslib_vector_push(v1, cslib_string_create("Nothing.", 128))
        );

        // check popping
        char *item = (char*)cslib_vector_pop(v1, 2);

        printf("The item I popped is: %s\n", item);

        if (v1->items[2] == NULL)
        {
            printf("popped successfully!\n");
        }

        cslib_vector_naivefree(v1);
        free(item);

        cslib_vector_dumbfree(v1);
    }


    {
        cslib_vector_t *vec = (cslib_vector_t*)malloc(sizeof(cslib_vector_t));
        cslib_allocate_vector(vec, 4);

        CHECK(cslib_vector_push(vec, cslib_string_create("Hello", 128)) != VECTOR_CAPACITY_REACHED, "within capacity - ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");
        CHECK(cslib_vector_push(vec, cslib_string_create("darkness,", 128)) != VECTOR_CAPACITY_REACHED, "within capacity - ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");
        CHECK(cslib_vector_push(vec, cslib_string_create("my old", 128)) != VECTOR_CAPACITY_REACHED, "within capacity - ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");
        CHECK(cslib_vector_push(vec, cslib_string_create("friend.", 128)) != VECTOR_CAPACITY_REACHED, "within capacity - ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");

        char *should_not_fit = cslib_string_create("talk", 128);
        CHECK(cslib_vector_push(vec, should_not_fit) == VECTOR_CAPACITY_REACHED, "out of capacity - ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");

        free(should_not_fit);

        cslib_vector_resize(vec, 16);
        printf("capacity: %zu\n", vec->capacity);

        CHECK(cslib_vector_push(vec, cslib_string_create("And this is it.", 128)) != VECTOR_CAPACITY_REACHED, "ssize_t cslib_vector_push(cslib_vector_t *vec, void *item)");
        CHECK(vec->capacity == 16, "bool cslib_vector_resize(cslib_vector_t *vec, size_t new_capacity)");

        cslib_vector_naivefree(vec);
        cslib_vector_dumbfree(vec);
    }

    return 0;
}
