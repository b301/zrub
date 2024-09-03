#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"
#include "cslib/pub.h"
#include <stdio.h>

int main()
{
    printf("testing hashmap\n");

    {
        cslib_hashmap_t *map = (cslib_hashmap_t *)malloc(sizeof(cslib_hashmap_t));

        cslib_allocate_hashmap(map, 16);

        cslib_hashmap_set(map, cslib_string_create("name", 128), cslib_string_create("oded zrubavel", 128));
        cslib_hashmap_set(map, cslib_string_create("eman", 128), cslib_string_create("flub zrubavel", 128));
        cslib_hashmap_set(map, cslib_string_create("mane", 128), cslib_string_create("vluv bubabelll", 128));
        cslib_hashmap_set(map, cslib_string_create("first_name", 128), cslib_string_create("oded", 128));
        cslib_hashmap_set(map, cslib_string_create("last_name", 128), cslib_string_create("zrubavel", 128));

        // cslib_hashmap_print_verbose(map);
        CHECK(map->length == 5, "test 1: ensure length is 5 after setting 5 items");

        cslib_hashmap_remove(map, "first_name");

        // cslib_hashmap_print_verbose(map);
        CHECK(map->length == 4, "test 1: ensure length is 4 after removing an item");

        cslib_hashmap_naivefree(map, true);
        cslib_hashmap_dumbfree(map);
    }

    /* test 2 */
    {
        cslib_hashmap_t *map = ALLOC_OBJECT(cslib_hashmap_t);
        cslib_allocate_hashmap(map, 4);
        CHECK(map->capacity == 4, "test 2: ensure the hashmap capacity is 4");

        cslib_hashmap_set(map, cslib_string_create("hello", 8), cslib_string_create("world", 8));
        CHECK(map->length == 1, "test 2: check setting \"hello\": \"world\"");

        // cslib_hashmap_print_verbose(map);

        cslib_hashmap_resize(map, 8);
        CHECK(map->capacity == 8, "test 2: ensure the hashmap capacity is correctly resized to 8");

        // cslib_hashmap_print_verbose(map);

        cslib_hashmap_naivefree(map, true);
        cslib_hashmap_dumbfree(map);
    }

    return 0;
}
