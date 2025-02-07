#include "zrub/types/vector.h"
#include "zrub/types/string.h"
#include "zrub/pub.h"
#include <stdio.h>


static void test_1(void)
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
    TESTS_CHECK(strncmp(item, "What is your problem?", 21) == 0 && v1->items[2] == NULL, 
        "ensuring the right item is popped");

    zrub_vector_naivefree(v1);
    zrub_vector_dumbfree(v1);
}

static void test_2(void)
{
    zrub_vector_t *vec = (zrub_vector_t*)ZRUB_MALLOC(sizeof(zrub_vector_t));
    zrub_allocate_vector(vec, 4);

    TESTS_CHECK(zrub_vector_insert(vec, zrub_string_create("Hello", 128)) != ZRUB_VECTOR_CAPACITY_REACHED, 
        "ensuring item is pushed");
    TESTS_CHECK(zrub_vector_insert(vec, zrub_string_create("darkness,", 128)) != ZRUB_VECTOR_CAPACITY_REACHED, 
        "ensuring item is pushed");
    TESTS_CHECK(zrub_vector_insert(vec, zrub_string_create("my old", 128)) != ZRUB_VECTOR_CAPACITY_REACHED, 
        "ensuring item is pushed");
    TESTS_CHECK(zrub_vector_insert(vec, zrub_string_create("friend.", 128)) != ZRUB_VECTOR_CAPACITY_REACHED, 
        "ensuring item is pushed");

    char *should_not_fit = zrub_string_create("talk", 128);
    TESTS_CHECK(zrub_vector_insert(vec, should_not_fit) == ZRUB_VECTOR_CAPACITY_REACHED, "ensuring vector is at it's capacity");

    free(should_not_fit);

    zrub_vector_resize(vec, 16);
    // printf("capacity: %zu\n", vec->capacity);

    TESTS_CHECK(zrub_vector_insert(vec, zrub_string_create("And this is it.", 128)) != ZRUB_VECTOR_CAPACITY_REACHED, "ensuring item is pushed");
    TESTS_CHECK(vec->capacity == 16, "ensuring vector resized to the expected size");

    zrub_vector_naivefree(vec);
    zrub_vector_dumbfree(vec);
}

static void test_3(void)
{
    zrub_vector_t *vec = (zrub_vector_t*)ZRUB_MALLOC(sizeof(zrub_vector_t));
    zrub_allocate_vector(vec, 4);

    zrub_vector_insert(vec, zrub_string_create("Hello", 128));
    zrub_vector_insert(vec, zrub_string_create("Friendo.", 128));
    zrub_vector_insert(vec, zrub_string_create("What is your problem?", 128));
    zrub_vector_insert(vec, zrub_string_create("Nothing.", 128));

    char *item = (char*)(zrub_vector_get(vec, 2));

    printf("item `%s`\n", item);

    TESTS_CHECK(item != NULL, "ensuring the item is not null");
    TESTS_CHECK(!strncmp(item, "What is your problem?", strlen(item)), "ensuring the correct item is selected");

    zrub_vector_naivefree(vec);
    zrub_vector_dumbfree(vec);
}


int main()
{
    test_1();
    test_2();
    test_3();


    return 0;
}
