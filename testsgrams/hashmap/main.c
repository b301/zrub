#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"
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
        cslib_string_create("first_name", 128), 
        cslib_string_create("oded", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_string_create("last_name", 128), 
        cslib_string_create("zrubavel", 128)
    );

    cslib_hashmap_print(map);

    cslib_hashmap_remove(map, "first_name");

    cslib_hashmap_print(map);

    cslib_hashmap_naivefree(map, true);
    cslib_hashmap_dumbfree(map);

    return 0;
}
