#include "zrub/os/time.h"
#include "zrub/pub.h"


static void test_1(void)
{
    zrub_time_t t1, ts;
    zrub_time_utcnow(&t1);
    zrub_time_utcnow(&ts);

    char *st1 = zrub_time_get_str(t1, ZRUB_TIME_DEFAULT);
    ZRUB_LOG_CHECK("time is %s", st1);

    zrub_time_sleep(2);

    zrub_time_t t2;
    zrub_time_utcnow(&t2);

    char *st2 = zrub_time_get_str(t2, ZRUB_TIME_DEFAULT);
    ZRUB_LOG_CHECK("time is %s", st2);

    TESTS_CHECK(zrub_time_gt(t2, t1), "t2 is greater than t1");
    TESTS_CHECK(zrub_time_lt(t1, t2), "t1 is lesser than t2");
    TESTS_CHECK(zrub_time_eq(t1, ts), "t1 is equal to ts");

    free(st1);
    free(st2);
}

static void test_2(void)
{
    zrub_time_t t1;
    zrub_time_utcnow(&t1);

    char str[128];
    zrub_time_set_str(t1, ZRUB_TIME_DEFAULT, str);

    ZRUB_LOG_CHECK("time is %s", str);
}


int main()
{
    test_1();
    test_2();

    return 0;
}