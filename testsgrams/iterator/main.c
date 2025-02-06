#include "zrublib/types/iterator.h"
#include "zrublib/types/string.h"
#include "zrublib/pub.h"


int main()
{
    printf("testing iterator\n");

    /* test 1 */
    {
        zrublib_vector_t *vec = ALLOC_OBJECT(zrublib_vector_t);
        zrublib_allocate_vector(vec, 4);

        zrublib_vector_insert(vec, zrublib_string_create("hello", 8));
        zrublib_vector_insert(vec, zrublib_string_create("there", 8));

        zrublib_iterator_t iterator; 
        zrublib_vector_iterator(&iterator, vec);

        CHECK(iterator.length == 2, "test 1: ensuring iterator is of length 2");
        CHECK(strncmp(iterator.items[0], "hello", 5) == 0, "test 1: ensuring items are correct");
        CHECK(strncmp(iterator.items[1], "there", 5) == 0, "test 1: ensuring items are correct");

        free(iterator.items);
        zrublib_vector_naivefree(vec);
        zrublib_vector_dumbfree(vec);
    }

    /* test 2 */
    {
        zrublib_hashmap_t *map = ALLOC_OBJECT(zrublib_hashmap_t);
        zrublib_allocate_hashmap(map, 8);

        zrublib_hashmap_set(map, zrublib_string_create("name", 8), zrublib_string_create("hey there delilah", 8));
        zrublib_hashmap_set(map, zrublib_string_create("language", 8), zrublib_string_create("english", 8));
        zrublib_hashmap_set(map, zrublib_string_create("author", 8), zrublib_string_create("white plain t-shirts", 8));

        zrublib_iterator_t keys, values;
        zrublib_hashmap_keys_iterator(&keys, map);
        zrublib_hashmap_values_iterator(&values, map);

        for (size_t i = 0; i < keys.length; i++)
        {
            printf("%s: %s\n", (char*)(keys.items[i]), (char*)(values.items[i]));
        }

        CHECK(keys.length == 3, "test 2: checking iterator length");
        CHECK(strncmp(keys.items[2], "language", 8) == 0, "test 2: checking index 2 value");

        free(keys.items);
        free(values.items);

        zrublib_hashmap_naivefree(map, true);
        zrublib_hashmap_dumbfree(map);
    }

    return 0;
}
