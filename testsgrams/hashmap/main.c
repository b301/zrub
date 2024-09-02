#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"
#include "cslib/pub.h"
#include <stdio.h>


int main()
{
    cslib_hashmap_t *map = (cslib_hashmap_t*)malloc(sizeof(cslib_hashmap_t));

    cslib_allocate_hashmap(map, 128);

    cslib_hashmap_set(
        map, 
        cslib_string_create("name", 128), 
        cslib_string_create("oded zrubavel", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_string_create("eman", 128), 
        cslib_string_create("flub zrubavel", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_string_create("mane", 128), 
        cslib_string_create("vluv bubabelll", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_string_create("first_name", 128), 
        cslib_string_create("oded", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_string_create("last_name", 128), 
        cslib_string_create("zrubavel", 128)
    );

    cslib_hashmap_print(map);
    CHECK(map->length == 5, "bool cslib_hashmap_set(cslib_hashmap_t *map, char *key, void *value)");

    cslib_hashmap_remove(map, "first_name");

    cslib_hashmap_print(map);
    CHECK(map->length == 4, "bool cslib_hashmap_set(cslib_hashmap_t *map, char *key, void *value)");

    cslib_hashmap_naivefree(map, true);
    cslib_hashmap_dumbfree(map);

    return 0;
}
