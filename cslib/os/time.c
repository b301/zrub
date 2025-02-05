#include "cslib/os/time.h"
#include "time.h"


static int64_t cslib_time_to_int64(cslib_time_t time)
{
    return ((int64_t)time.year * 10000000000LL) +
           ((int64_t)time.month * 100000000LL) +
           ((int64_t)time.day * 1000000LL) +
           ((int64_t)time.hour * 10000LL) +
           ((int64_t)time.min * 100LL) +
           (int64_t)time.sec;
}

bool cslib_time_from_time_t(cslib_time_t *time_data, time_t time_t_data)
{
    struct tm *t = gmtime(&time_t_data);

    if (t == NULL) {
        fprintf(stderr, "cslib_time_from_time_t::failed to convert to utc\n");
        return false;
    }

    time_data->day = (short)(t->tm_mday);
    time_data->month = (short)(t->tm_mon + 1);
    time_data->year = (short)(t->tm_year + 1900);
    
    time_data->sec = (short)(t->tm_sec);
    time_data->min = (short)(t->tm_min);
    time_data->hour = (short)(t->tm_hour);

    return true;
}

bool cslib_time_utcnow(cslib_time_t *time_data) 
{
    time_t ct = time(NULL);
    if (ct == -1) {
        fprintf(stderr, "cslib_time_utc::failed to get the current time\n");
        return false;
    }

    if (!cslib_time_from_time_t(time_data, ct)) {
        return false;
    }

    return true;
}

void cslib_time_set_str(char *dest, cslib_time_t time_data)
{
    snprintf(dest, 128, "%02d-%02d-%04d %02d:%02d:%02d", 
        time_data.day,
        time_data.month,
        time_data.year,
        time_data.hour,
        time_data.min,
        time_data.sec);
}

char *cslib_time_get_str(cslib_time_t time_data)
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
