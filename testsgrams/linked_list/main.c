#include "cslib/std/linked_list.h"
#include <stdio.h>


int main()
{
    void *s1 = cslib_initialize_linked_node(NULL, NULL);
    void *s2 = cslib_initialize_linked_node(NULL, s1);
    void *s3 = cslib_initialize_linked_node(NULL, s2);
    void *s4 = cslib_initialize_linked_node(NULL, s3);

    size_t listsize = cslib_linked_list_length(s1);

    printf("Size of the linked list: %lu\n", listsize);

    free(s1);
    free(s2);
    free(s3);
    free(s4);

    return 0;
}
