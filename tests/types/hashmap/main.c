#include "zrub/types/hashmap.h"
#include "zrub/types/string.h"
#include "zrub/pub.h"
#include <stdio.h>


static test_1(void)
{
    zrub_hashmap_t *map = (zrub_hashmap_t *)ZRUB_MALLOC(sizeof(zrub_hashmap_t));

    zrub_allocate_hashmap(map, 16);

    zrub_hashmap_set(map, zrub_string_create("name", 128), zrub_string_create("oded zrubavel", 128));
    zrub_hashmap_set(map, zrub_string_create("eman", 128), zrub_string_create("flub zrubavel", 128));
    zrub_hashmap_set(map, zrub_string_create("mane", 128), zrub_string_create("vluv bubabelll", 128));
    zrub_hashmap_set(map, zrub_string_create("first_name", 128), zrub_string_create("oded", 128));
    zrub_hashmap_set(map, zrub_string_create("last_name", 128), zrub_string_create("zrubavel", 128));

    // zrub_hashmap_print_verbose(map);
    TESTS_CHECK(map->length == 5, "test 1: ensure length is 5 after setting 5 items");

    zrub_hashmap_remove(map, "first_name");

    // zrub_hashmap_print_verbose(map);
    TESTS_CHECK(map->length == 4, "test 1: ensure length is 4 after removing an item");

    zrub_hashmap_naivefree(map, true);
    zrub_hashmap_dumbfree(map);
}

static void test_2(void)
{
    zrub_hashmap_t *map = ALLOC_OBJECT(zrub_hashmap_t);
    zrub_allocate_hashmap(map, 4);
    TESTS_CHECK(map->capacity == 4, "test 2: ensure the hashmap capacity is 4");

    zrub_hashmap_set(map, zrub_string_create("hello", 8), zrub_string_create("world", 8));
    TESTS_CHECK(map->length == 1, "test 2: check setting \"hello\": \"world\"");

    // zrub_hashmap_print_verbose(map);

    zrub_hashmap_resize(map, 8);
    TESTS_CHECK(map->capacity == 8, "test 2: ensure the hashmap capacity is correctly resized to 8");

    // zrub_hashmap_print_verbose(map);

    zrub_hashmap_naivefree(map, true);
    zrub_hashmap_dumbfree(map);
}


int main()
{
    test_1();
    test_2();


    return 0;
}
