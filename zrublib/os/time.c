#include "zrublib/os/time.h"
#include "time.h"


static int64_t zrublib_time_to_int64(zrublib_time_t time)
{
    return ((int64_t)time.year * 10000000000LL) +
           ((int64_t)time.month * 100000000LL) +
           ((int64_t)time.day * 1000000LL) +
           ((int64_t)time.hour * 10000LL) +
           ((int64_t)time.min * 100LL) +
           (int64_t)time.sec;
}

bool zrublib_time_from_time_t(zrublib_time_t *time_data, time_t time_t_data)
{
    struct tm *t = gmtime(&time_t_data);

    if (t == NULL) {
        fprintf(stderr, "zrublib_time_from_time_t::failed to convert to utc\n");
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

bool zrublib_time_utcnow(zrublib_time_t *time_data) 
{
    time_t ct = time(NULL);
    if (ct == -1) {
        fprintf(stderr, "zrublib_time_utc::failed to get the current time\n");
        return false;
    }

    if (!zrublib_time_from_time_t(time_data, ct)) {
        return false;
    }

    return true;
}

void zrublib_time_set_str(const zrublib_time_t time_data, short time_format, char *dest)
{
    switch (time_format)
    {
        case ZRUBLIB_TIME_DEFAULT:
            snprintf(dest, 128, __ZRUBLIB_TIME_FORMAT_DEFAULT, 
                time_data.day,
                time_data.month,
                time_data.year,
                time_data.hour,
                time_data.min,
                time_data.sec);
            break;

        case ZRUBLIB_TIME_DATEONLY:
            snprintf(dest, 128, __ZRUBLIB_TIME_FORMAT_DATEONLY, 
                time_data.day,
                time_data.month,
                time_data.year);
            break;

        case ZRUBLIB_TIME_TIMEONLY:
            snprintf(dest, 128, __ZRUBLIB_TIME_FORMAT_TIMEONLY, 
                time_data.hour,
                time_data.min,
                time_data.sec);
            break;

        default:
            break;
    }
}

char *zrublib_time_get_str(const zrublib_time_t time_data, short time_format)
{
    /* FIXME: ensure zrublib_time_t is valid! */
    char *str = zrublib_string_create("\0", 128);
    zrublib_time_set_str(time_data, time_format, str);
    return str;
}

/* Check if t1 > t2 */
bool zrublib_time_gt(zrublib_time_t t1, zrublib_time_t t2)
{
    return zrublib_time_to_int64(t1) > zrublib_time_to_int64(t2);
}

/* Check if t1 < t2 */
bool zrublib_time_lt(zrublib_time_t t1, zrublib_time_t t2)
{
    return zrublib_time_to_int64(t1) < zrublib_time_to_int64(t2);
}

/* Check if t1 == t2 */
bool zrublib_time_eq(zrublib_time_t t1, zrublib_time_t t2)
{
    return zrublib_time_to_int64(t1) == zrublib_time_to_int64(t2);
}

void zrublib_time_sleep(int seconds)
{
    #if defined(__linux__)
    sleep(seconds);

    #elif defined(_WIN32) || defined(WIN32)
    Sleep(1000 * seconds);

    #endif
}

void zrublib_time_set(zrublib_time_t *time_data, short day, short month, 
    short year, short min, short sec, short hour)
{
    time_data->day = day;
    time_data->month = month;
    time_data->year = year;
    time_data->min = min;
    time_data->sec = sec;
    time_data->hour = hour;
}