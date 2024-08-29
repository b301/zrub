#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"
#include <stdio.h>


int main()
{
    cslib_hashmap_t *map = (cslib_hashmap_t*)malloc(sizeof(cslib_hashmap_t));

    cslib_allocate_hashmap(map, 128);

    cslib_hashmap_set(
        map, 
        cslib_create_string("name", 128), 
        cslib_create_string("oded zrubavel", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_create_string("eman", 128), 
        cslib_create_string("flub zrubavel", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_create_string("first_name", 128), 
        cslib_create_string("oded", 128)
    );

    cslib_hashmap_set(
        map, 
        cslib_create_string("last_name", 128), 
        cslib_create_string("zrubavel", 128)
    );

    cslib_hashmap_print(map);

    cslib_hashmap_remove(map, "first_name");

    cslib_hashmap_print(map);

    cslib_hashmap_naivefree(map, true);
    cslib_hashmap_dumbfree(map);

    return 0;
}
