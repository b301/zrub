#include "zrub/types/queue.h"
#include "zrub/types/string.h"
#include "zrub/pub.h"

int main()
{
    {
        zrub_queue_t *q = ALLOC_OBJECT(zrub_queue_t);
        zrub_allocate_queue(q, 256);

        CHECK(zrub_queue_push(q, zrub_string_create("Hello", 8)), "test 1: queue push item");
        CHECK(zrub_queue_push(q, zrub_string_create("My", 8)), "test 1: queue push item");
        CHECK(zrub_queue_push(q, zrub_string_create("Name", 8)), "test 1: queue push item");
        CHECK(zrub_queue_push(q, zrub_string_create("Is", 8)), "test 1: queue push item");
        CHECK(zrub_queue_push(q, zrub_string_create("Oded", 8)), "test 1: queue push item");

        CHECK(q->fp == 5, "test 1: fp == 5");
        CHECK(q->bp == 0, "test 1: bp == 0");

        size_t ql = q->length;
        for (size_t i = 0; i < ql; i++)
        {
            void *item = zrub_queue_pop(q);
            printf("%02lld: %s\n", i, (char*)item);

            free(item);
        }

        CHECK(q->fp == q->bp, "test 1: fp == bp");
        zrub_queue_dumbfree(q);
    }

    return 0;
}