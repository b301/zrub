#include "zrub/types/vector.h"
#include "zrub/types/string.h"
#include "zrub/pub.h"
#include <stdio.h>


int main()
{
    printf("testing vector\n");

    /* test 1: ssize_t zrub_vector_insert(zrub_vector_t *vec, void *item) */
    {
        zrub_vector_t *v1 = (zrub_vector_t*)ZRUB_MALLOC(sizeof(zrub_vector_t));

        zrub_allocate_vector(v1, 128);

        // check pushing
        zrub_vector_insert(v1, zrub_string_create("Hello", 128));
        zrub_vector_insert(v1, zrub_string_create("Friendo.", 128));
        zrub_vector_insert(v1, zrub_string_create("What is your problem?", 128));
        zrub_vector_insert(v1, zrub_string_create("Nothing.", 128));

        // check popping
        char *item = (char*)zrub_vector_remove(v1, 2);
        CHECK(strncmp(item, "What is your problem?", 21) == 0 && v1->items[2] == NULL, 
            "test 1: ensuring the right item is popped");

        zrub_vector_naivefree(v1);
        zrub_vector_dumbfree(v1);
    }

    /* test 2: test if vector_capacity_reached is reached */
    {
        zrub_vector_t *vec = (zrub_vector_t*)ZRUB_MALLOC(sizeof(zrub_vector_t));
        zrub_allocate_vector(vec, 4);

        CHECK(zrub_vector_insert(vec, zrub_string_create("Hello", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrub_vector_insert(vec, zrub_string_create("darkness,", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrub_vector_insert(vec, zrub_string_create("my old", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");
        CHECK(zrub_vector_insert(vec, zrub_string_create("friend.", 128)) != VECTOR_CAPACITY_REACHED, 
            "test 2: ensuring item is pushed");

        char *should_not_fit = zrub_string_create("talk", 128);
        CHECK(zrub_vector_insert(vec, should_not_fit) == VECTOR_CAPACITY_REACHED, "test 2: ensuring vector is at it's capacity");

        free(should_not_fit);

        zrub_vector_resize(vec, 16);
        // printf("capacity: %zu\n", vec->capacity);

        CHECK(zrub_vector_insert(vec, zrub_string_create("And this is it.", 128)) != VECTOR_CAPACITY_REACHED, "test 2: ensuring item is pushed");
        CHECK(vec->capacity == 16, "test 2: ensuring vector resized to the expected size");

        zrub_vector_naivefree(vec);
        zrub_vector_dumbfree(vec);
    }

    /* test 3: void *zrub_vector_get(zrub_vector_t *vec, ssize_t index) */
    {
        zrub_vector_t *vec = (zrub_vector_t*)ZRUB_MALLOC(sizeof(zrub_vector_t));
        zrub_allocate_vector(vec, 4);

        zrub_vector_insert(vec, zrub_string_create("Hello", 128));
        zrub_vector_insert(vec, zrub_string_create("Friendo.", 128));
        zrub_vector_insert(vec, zrub_string_create("What is your problem?", 128));
        zrub_vector_insert(vec, zrub_string_create("Nothing.", 128));

        void *item = zrub_vector_get(vec, 2);

        printf("item `%s`\n", item);

        CHECK(item != NULL, "test 3: ensuring the item is not null");
        CHECK(!strncmp(item, "What is your problem?", strlen(item)), "test 3: ensuring the correct item is selected");

        zrub_vector_naivefree(vec);
        zrub_vector_dumbfree(vec);
    }

    return 0;
}
