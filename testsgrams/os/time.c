#include "cslib/os/time.h"


int main()
{
    /* test 1 */
    {
        cslib_time_t t1, ts;
        cslib_time_utcnow(&t1);
        cslib_time_utcnow(&ts);

        char *st1 = cslib_time_get_str(t1);
        printf("test 1: time is %s\n", st1);

        cslib_time_sleep(2);

        cslib_time_t t2;
        cslib_time_utcnow(&t2);

        char *st2 = cslib_time_get_str(t2);
        printf("test 1: time is %s\n", st2);

        CHECK(cslib_time_gt(t2, t1), "test 1: t2 is greater than t1");
        CHECK(cslib_time_lt(t1, t2), "test 1: t1 is lesser than t2");
        CHECK(cslib_time_eq(t1, ts), "test 1: t1 is equal to ts");

        free(st1);
        free(st2);
    }

    /* test 2 */
    {
        cslib_time_t t1;
        cslib_time_utcnow(&t1);

        char str[128];
        cslib_time_set_str(str, t1);

        printf("test 2: time is %s\n", str);
    }

    return 0;
}