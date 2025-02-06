#include "zrublib/types/vector.h"
#include "zrublib/types/string.h"
#include "zrublib/pub.h"
#include <stdio.h>


int main()
{
    printf("testing vector\n");

    /* test 1: ssize_t zrublib_vector_insert(zrublib_vector_t *vec, void *item) */
    {
        zrublib_vector_t *v1 = (zrublib_vector_t*)ZRUBLIB_MALLOC(sizeof(zrublib_vector_t));

        zrublib_allocate_vector(v1, 128);

        // check pushing
        zrublib_vector_insert(v1, zrublib_string_create("Hello", 128));
        zrublib_vector_insert(v1, zrublib_string_create("Friendo.", 128));
        zrublib_vector_insert(v1, zrublib_string_create("What is your problem?", 128));
        zrublib_vector_insert(v1, zrublib_string_create("Nothing.", 128));

        // check popping
        char *item = (char*)zrublib_vector_remove(v1, 2);
        CHECK(strncmp(item, "What is your problem?", 21) == 0 && v1->items[2] == NULL, 
            "test 1: ensuring the right item is popped");

        zrublib_vector_naivefree(v1);
        zrublib_vector_dumbfree(v1);
    }

    /* test 2: test if vector_capacity_reached is reached */
    {
        zrublib_vector_t *vec = (zrublib_vector_t*)ZRUBLIB_MALLOC(sizeof(zrublib_vector_t));
        zrublib_allocate_vector(vec, 4);

        CHECK(zrublib_vector_insert(vec, zrublib_string_create("Hello", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrublib_vector_insert(vec, zrublib_string_create("darkness,", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrublib_vector_insert(vec, zrublib_string_create("my old", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrublib_vector_insert(vec, zrublib_string_create("friend.", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");

        char *should_not_fit = zrublib_string_create("talk", 128);
        CHECK(zrublib_vector_insert(vec, should_not_fit) == VECTOR_CAPACITY_REACHED, "test 2: ensuring vector is at it's capacity");

        free(should_not_fit);

        zrublib_vector_resize(vec, 16);
        // printf("capacity: %zu\n", vec->capacity);

        CHECK(zrublib_vector_insert(vec, zrublib_string_create("And this is it.", 128)) != VECTOR_CAPACITY_REACHED, "test 2: ensuring item is pushed");
        CHECK(vec->capacity == 16, "test 2: ensuring vector resized to the expected size");

        zrublib_vector_naivefree(vec);
        zrublib_vector_dumbfree(vec);
    }

    /* test 3: void *zrublib_vector_get(zrublib_vector_t *vec, ssize_t index) */
    {
        zrublib_vector_t *vec = (zrublib_vector_t*)ZRUBLIB_MALLOC(sizeof(zrublib_vector_t));
        zrublib_allocate_vector(vec, 4);

        zrublib_vector_insert(vec, zrublib_string_create("Hello", 128));
        zrublib_vector_insert(vec, zrublib_string_create("Friendo.", 128));
        zrublib_vector_insert(vec, zrublib_string_create("What is your problem?", 128));
        zrublib_vector_insert(vec, zrublib_string_create("Nothing.", 128));

        void *item = zrublib_vector_get(vec, 2);

        printf("item `%s`\n", item);

        CHECK(item != NULL, "test 3: ensuring the item is not null");
        CHECK(!strncmp(item, "What is your problem?", strlen(item)), "test 3: ensuring the correct item is selected");

        zrublib_vector_naivefree(vec);
        zrublib_vector_dumbfree(vec);
    }

    return 0;
}
