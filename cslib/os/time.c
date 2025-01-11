#include "cslib/os/time.h"

static int64_t cslib_time_to_int64(cslib_time_t time)
{
    return ((int64_t)time.year * 10000000000LL) +
           ((int64_t)time.month * 100000000LL) +
           ((int64_t)time.day * 1000000LL) +
           ((int64_t)time.hour * 10000LL) +
           ((int64_t)time.min * 100LL) +
           (int64_t)time.sec;
}

bool cslib_time_utcnow(cslib_time_t *time_data)
{
    time_t ct = time(NULL);
    if (ct == -1) {
        fprintf(stderr, "cslib_time_utc::failed to get the current time\n");
        return false;
    }

    struct tm *utc = gmtime(&ct);
    if (utc == NULL) {
        fprintf(stderr, "cslib_time_utc::failed to convert to utc\n");
        return false;
    }

    time_data->day = (short)(utc->tm_mday);
    time_data->month = (short)(utc->tm_mon + 1);
    time_data->year = (short)(utc->tm_year + 1900);
    
    time_data->sec = (short)(utc->tm_sec);
    time_data->min = (short)(utc->tm_min);
    time_data->hour = (short)(utc->tm_hour);

    return true;
}

char *cslib_time_as_str(cslib_time_t time_data)
{
    /* FIXME: ensure cslib_time_t is valid! */
    char *str = cslib_string_create("\0", 128);
    
    snprintf(str, 128, "%02d-%02d-%04d %02d:%02d:%02d", 
        time_data.day,
        time_data.month,
        time_data.year,
        time_data.hour,
        time_data.min,
        time_data.sec);

    return str;
}

/* Check if t1 > t2 */
bool cslib_time_gt(cslib_time_t t1, cslib_time_t t2)
{
    return cslib_time_to_int64(t1) > cslib_time_to_int64(t2);
}

/* Check if t1 < t2 */
bool cslib_time_lt(cslib_time_t t1, cslib_time_t t2)
{
    return cslib_time_to_int64(t1) < cslib_time_to_int64(t2);
}

/* Check if t1 == t2 */
bool cslib_time_eq(cslib_time_t t1, cslib_time_t t2)
{
    return cslib_time_to_int64(t1) == cslib_time_to_int64(t2);
}

void cslib_time_sleep(int seconds)
{
    #if defined(__linux__)
    sleep(seconds);

    #elif defined(_WIN32) || defined(WIN32)
    Sleep(1000 * seconds);

    #endif
}
