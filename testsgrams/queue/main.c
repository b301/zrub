#include "cslib/types/queue.h"
#include "cslib/types/string.h"
#include "cslib/pub.h"

int main()
{
    {
        cslib_queue_t *q = ALLOC_OBJECT(cslib_queue_t);
        cslib_allocate_queue(q, 256);

        CHECK(cslib_queue_push(q, cslib_string_create("Hello", 8)), "test 1: queue push item");
        CHECK(cslib_queue_push(q, cslib_string_create("My", 8)), "test 1: queue push item");
        CHECK(cslib_queue_push(q, cslib_string_create("Name", 8)), "test 1: queue push item");
        CHECK(cslib_queue_push(q, cslib_string_create("Is", 8)), "test 1: queue push item");
        CHECK(cslib_queue_push(q, cslib_string_create("Oded", 8)), "test 1: queue push item");

        CHECK(q->fp == 5, "test 1: fp == 5");
        CHECK(q->bp == 0, "test 1: bp == 0");

        size_t ql = q->length;
        for (size_t i = 0; i < ql; i++)
        {
            void *item = cslib_queue_pop(q);
            printf("%02lld %s\n", i, (char*)item);

            free(item);
        }

        CHECK(q->fp == q->bp, "test 1: fp == bp");
        cslib_queue_dumbfree(q);
    }

    return 0;
}