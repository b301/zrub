#include "cslib/os/time.h"


int main()
{
    {
        cslib_time_t t1, ts;
        cslib_time_utcnow(&t1);
        cslib_time_utcnow(&ts);

        char *st1 = cslib_time_as_str(t1);
        printf("test 1: time is %s\n", st1);

        cslib_time_sleep(2);

        cslib_time_t t2;
        cslib_time_utcnow(&t2);

        char *st2 = cslib_time_as_str(t2);
        printf("test 1: time is %s\n", st2);

        CHECK(cslib_time_gt(t2, t1), "test 1: t2 is greater than t1");
        CHECK(cslib_time_lt(t1, t2), "test 1: t1 is lesser than t2");
        CHECK(cslib_time_eq(t1, ts), "test 1: t1 is equal to ts");

        free(st1);
        free(st2);
    }

    return 0;
}