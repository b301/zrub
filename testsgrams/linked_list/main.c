#include "zrub/types/linked_list.h"
#include "zrub/pub.h"
#include <stdio.h>


int main()
{
    printf("testing linked list\n");

    /* test 1 */
    {
        void *s1 = zrub_initialize_linked_node(NULL, NULL);
        void *s2 = zrub_initialize_linked_node(NULL, s1);
        void *s3 = zrub_initialize_linked_node(NULL, s2);
        void *s4 = zrub_initialize_linked_node(NULL, s3);

        size_t length = zrub_linked_list_length(s4);
        CHECK(length == 4, "test 1: ensuring the list size is the expected size");

        free(s1);
        free(s2);
        free(s3);
        free(s4);
    }

    return 0;
}
