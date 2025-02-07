#include "zrub/os/time.h"
#include "zrub/pub.h"


int main()
{
    /* test 1 */
    {
        zrub_time_t t1, ts;
        zrub_time_utcnow(&t1);
        zrub_time_utcnow(&ts);

        char *st1 = zrub_time_get_str(t1, ZRUB_TIME_DEFAULT);
        printf("test 1: time is %s\n", st1);

        zrub_time_sleep(2);

        zrub_time_t t2;
        zrub_time_utcnow(&t2);

        char *st2 = zrub_time_get_str(t2, ZRUB_TIME_DEFAULT);
        printf("test 1: time is %s\n", st2);

        TESTS_CHECK(zrub_time_gt(t2, t1), "test 1: t2 is greater than t1");
        TESTS_CHECK(zrub_time_lt(t1, t2), "test 1: t1 is lesser than t2");
        TESTS_CHECK(zrub_time_eq(t1, ts), "test 1: t1 is equal to ts");

        free(st1);
        free(st2);
    }

    /* test 2 */
    {
        zrub_time_t t1;
        zrub_time_utcnow(&t1);

        char str[128];
        zrub_time_set_str(t1, ZRUB_TIME_DEFAULT, str);

        printf("test 2: time is %s\n", str);
    }

    return 0;
}