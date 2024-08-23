#include "cslib/std/hashmap.h"
#include "cslib/std/string.h"
#include <stdio.h>


int main()
{
    size_t hash = cslib_hashmap_hashfunc_1("name", 128);
    printf("HashValue: %lu\n", hash);

    cslib_hashmap_t *map = (cslib_hashmap_t*)malloc(sizeof(cslib_hashmap_t));

    cslib_allocate_hashmap(map, 128);

    cslib_set_hashmap(
        map, 
        "name", 
        cslib_allocate_string("oded zrubavel", 128)
    );

    cslib_set_hashmap(
        map, 
        "eman", 
        cslib_allocate_string("flub zrubavel", 128)
    );

    cslib_set_hashmap(
        map, 
        "first_name", 
        cslib_allocate_string("oded", 128)
    );

    cslib_set_hashmap(
        map, 
        "last_name", 
        cslib_allocate_string("zrubavel", 128)
    );

    printf("name: %s\neman: %s\n", 
        cslib_get_hashmap(map, "name"),
        cslib_get_hashmap(map, "eman"));

    free(cslib_get_hashmap(map, "name"));
    free(cslib_get_hashmap(map, "eman"));
    free(cslib_get_hashmap(map, "first_name"));
    free(cslib_get_hashmap(map, "last_name"));

    cslib_testfree_hashmap(map, false);
    cslib_dumbfree_hashmap(map);

    return 0;
}
