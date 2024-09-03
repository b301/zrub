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

    /* test 2 */
    {
        cslib_hashmap_t *map = ALLOC_OBJECT(cslib_hashmap_t);
        cslib_allocate_hashmap(map, 8);

        cslib_hashmap_set(map, cslib_string_create("name", 8), cslib_string_create("hey there delilah", 8));
        cslib_hashmap_set(map, cslib_string_create("language", 8), cslib_string_create("english", 8));
        cslib_hashmap_set(map, cslib_string_create("author", 8), cslib_string_create("white plain t-shirts", 8));

        cslib_iterator_t keys, values;
        cslib_hashmap_keys_iterator(&keys, map);
        cslib_hashmap_values_iterator(&values, map);

        for (size_t i = 0; i < keys.length; i++)
        {
            printf("%s: %s\n", (char*)(keys.items[i]), (char*)(values.items[i]));
        }

        CHECK(keys.length == 3, "test 2: checking iterator length");
        CHECK(strncmp(keys.items[2], "language", 8) == 0, "test 2: checking index 2 value");

        free(keys.items);
        free(values.items);

        cslib_hashmap_naivefree(map, true);
        cslib_hashmap_dumbfree(map);
    }

    return 0;
}
