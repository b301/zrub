#include "zrub/types/iterator.h"
#include "zrub/types/string.h"
#include "zrub/pub.h"


static void test_1(void)
{
    zrub_vector_t *vec = ALLOC_OBJECT(zrub_vector_t);
    zrub_allocate_vector(vec, 4);

    zrub_vector_insert(vec, zrub_string_create("hello", 8));
    zrub_vector_insert(vec, zrub_string_create("there", 8));

    zrub_iterator_t iterator; 
    zrub_vector_iterator(&iterator, vec);

    TESTS_CHECK(iterator.length == 2, "ensuring iterator is of length 2");
    TESTS_CHECK(strncmp(iterator.items[0], "hello", 5) == 0, "ensuring items are correct");
    TESTS_CHECK(strncmp(iterator.items[1], "there", 5) == 0, "ensuring items are correct");

    free(iterator.items);
    zrub_vector_naivefree(vec);
    zrub_vector_dumbfree(vec);
}

static void test_2(void)
{
    zrub_hashmap_t *map = ALLOC_OBJECT(zrub_hashmap_t);
    zrub_allocate_hashmap(map, 8);

    zrub_hashmap_set(map, zrub_string_create("name", 8), zrub_string_create("hey there delilah", 8));
    zrub_hashmap_set(map, zrub_string_create("language", 8), zrub_string_create("english", 8));
    zrub_hashmap_set(map, zrub_string_create("author", 8), zrub_string_create("white plain t-shirts", 8));

    zrub_iterator_t keys, values;
    zrub_hashmap_keys_iterator(&keys, map);
    zrub_hashmap_values_iterator(&values, map);

    for (size_t i = 0; i < keys.length; i++)
    {
        printf("%s: %s\n", (char*)(keys.items[i]), (char*)(values.items[i]));
    }

    TESTS_CHECK(keys.length == 3, "checking iterator length");
    TESTS_CHECK(strncmp(keys.items[2], "language", 8) == 0, "checking index 2 value");

    free(keys.items);
    free(values.items);

    zrub_hashmap_naivefree(map, true);
    zrub_hashmap_dumbfree(map);
}


int main()
{
    test_1();
    test_2();


    return 0;
}
