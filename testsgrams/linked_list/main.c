#include "zrublib/types/linked_list.h"
#include "zrublib/pub.h"
#include <stdio.h>


int main()
{
    printf("testing linked list\n");

    /* test 1 */
    {
        void *s1 = zrublib_initialize_linked_node(NULL, NULL);
        void *s2 = zrublib_initialize_linked_node(NULL, s1);
        void *s3 = zrublib_initialize_linked_node(NULL, s2);
        void *s4 = zrublib_initialize_linked_node(NULL, s3);

        size_t length = zrublib_linked_list_length(s4);
        CHECK(length == 4, "test 1: ensuring the list size is the expected size");

        free(s1);
        free(s2);
        free(s3);
        free(s4);
    }

    return 0;
}
