#define ZRUBLIB_STANDARD
#include "zrublib/zrublib.h"


int main()
{
    zrublib_time_t time;
    char *timestr = (char *)malloc(128);

    for (size_t i = 0; i < 3; i++)
    {
        zrublib_time_utcnow(&time);
        zrublib_time_set_str(time, ZRUBLIB_TIME_TIMEONLY, timestr);

        printf("%s\n", timestr);

        zrublib_time_sleep(1);
    }

    free(timestr);

    return 0;
}