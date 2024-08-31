#include "cslib/std/vector.h"
#include "cslib/std/string.h"
#include <stdio.h>


int main()
{
    cslib_vector_t *v1 = (cslib_vector_t*)malloc(sizeof(cslib_vector_t));

    cslib_allocate_vector(v1, 128);

    // check pushing
    printf("%d | %d | %d | %d\n",
        cslib_vector_push(v1, cslib_string_create("Hello", 128)),
        cslib_vector_push(v1, cslib_string_create("Friendo.", 128)),
        cslib_vector_push(v1, cslib_string_create("What is your problem?", 128)),
        cslib_vector_push(v1, cslib_string_create("Nothing.", 128))
    );

    // check popping
    char *item = (char*)cslib_vector_pop(v1, 2);

    printf("The item I popped is: %s\n", item);

    if (v1->items[2] == NULL)
    {
        printf("popped successfully!\n");
    }

    cslib_vector_naivefree(v1);
    free(item);

    cslib_vector_dumbfree(v1);

    return 0;
}
