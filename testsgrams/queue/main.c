#include "zrublib/types/queue.h"
#include "zrublib/types/string.h"
#include "zrublib/pub.h"

int main()
{
    {
        zrublib_queue_t *q = ALLOC_OBJECT(zrublib_queue_t);
        zrublib_allocate_queue(q, 256);

        CHECK(zrublib_queue_push(q, zrublib_string_create("Hello", 8)), "test 1: queue push item");
        CHECK(zrublib_queue_push(q, zrublib_string_create("My", 8)), "test 1: queue push item");
        CHECK(zrublib_queue_push(q, zrublib_string_create("Name", 8)), "test 1: queue push item");
        CHECK(zrublib_queue_push(q, zrublib_string_create("Is", 8)), "test 1: queue push item");
        CHECK(zrublib_queue_push(q, zrublib_string_create("Oded", 8)), "test 1: queue push item");

        CHECK(q->fp == 5, "test 1: fp == 5");
        CHECK(q->bp == 0, "test 1: bp == 0");

        size_t ql = q->length;
        for (size_t i = 0; i < ql; i++)
        {
            void *item = zrublib_queue_pop(q);
            printf("%02lld: %s\n", i, (char*)item);

            free(item);
        }

        CHECK(q->fp == q->bp, "test 1: fp == bp");
        zrublib_queue_dumbfree(q);
    }

    return 0;
}