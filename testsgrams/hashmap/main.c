#include "zrublib/types/hashmap.h"
#include "zrublib/types/string.h"
#include "zrublib/pub.h"
#include <stdio.h>

int main()
{
    printf("testing hashmap\n");

    /* test 1 */
    {
        zrublib_hashmap_t *map = (zrublib_hashmap_t *)ZRUBLIB_MALLOC(sizeof(zrublib_hashmap_t));

        zrublib_allocate_hashmap(map, 16);

        zrublib_hashmap_set(map, zrublib_string_create("name", 128), zrublib_string_create("oded zrubavel", 128));
        zrublib_hashmap_set(map, zrublib_string_create("eman", 128), zrublib_string_create("flub zrubavel", 128));
        zrublib_hashmap_set(map, zrublib_string_create("mane", 128), zrublib_string_create("vluv bubabelll", 128));
        zrublib_hashmap_set(map, zrublib_string_create("first_name", 128), zrublib_string_create("oded", 128));
        zrublib_hashmap_set(map, zrublib_string_create("last_name", 128), zrublib_string_create("zrubavel", 128));

        // zrublib_hashmap_print_verbose(map);
        CHECK(map->length == 5, "test 1: ensure length is 5 after setting 5 items");

        zrublib_hashmap_remove(map, "first_name");

        // zrublib_hashmap_print_verbose(map);
        CHECK(map->length == 4, "test 1: ensure length is 4 after removing an item");

        zrublib_hashmap_naivefree(map, true);
        zrublib_hashmap_dumbfree(map);
    }

    /* test 2 */
    {
        zrublib_hashmap_t *map = ALLOC_OBJECT(zrublib_hashmap_t);
        zrublib_allocate_hashmap(map, 4);
        CHECK(map->capacity == 4, "test 2: ensure the hashmap capacity is 4");

        zrublib_hashmap_set(map, zrublib_string_create("hello", 8), zrublib_string_create("world", 8));
        CHECK(map->length == 1, "test 2: check setting \"hello\": \"world\"");

        // zrublib_hashmap_print_verbose(map);

        zrublib_hashmap_resize(map, 8);
        CHECK(map->capacity == 8, "test 2: ensure the hashmap capacity is correctly resized to 8");

        // zrublib_hashmap_print_verbose(map);

        zrublib_hashmap_naivefree(map, true);
        zrublib_hashmap_dumbfree(map);
    }

    return 0;
}
