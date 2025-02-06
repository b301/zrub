#define CSLIB_STANDARD
#include "cslib/cslib.h"


int main()
{
    cslib_time_t time;
    char *timestr = (char *)malloc(128);

    for (size_t i = 0; i < 3; i++)
    {
        cslib_time_utcnow(&time);
        cslib_time_set_str(time, CSLIB_TIME_TIMEONLY, timestr);

        printf("%s\n", timestr);

        cslib_time_sleep(1);
    }

    free(timestr);

    return 0;
}