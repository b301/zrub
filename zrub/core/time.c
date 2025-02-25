#include "time.h"


/**
 * @brief converts zrub_time_t to int64.
 * 
 * @param time      zrub_time_t struct
 * @returns zrub_time_t object converted to int64
 */
static long long zrub_time_to_int64(zrub_time_t time)
{
    return ((long long)time.year * 10000000000LL) +
           ((long long)time.month * 100000000LL) +
           ((long long)time.day * 1000000LL) +
           ((long long)time.hour * 10000LL) +
           ((long long)time.min * 100LL) +
           (long long)time.sec;
}

/**
 * @brief gets the time as of now.
 * 
 * @param time_data     zrub_time_t struct
 * @param time_t_data   time_t struct
 * @returns true if managed to set zrub_time_t
 */
bool zrub_time_get(zrub_time_t *time_data, time_t time_t_data)
{
    struct tm *t = gmtime(&time_t_data);

    if (t == NULL) {
        fprintf(stderr, "[%s]::failed to convert to utc\n", __func__);
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

/**
 * @brief gets the universal time coordinated (utc).
 * 
 * @param time_data     zrub_time_t struct
 * @returns true if managed to retrieve the time
 */
bool zrub_time_utcnow(zrub_time_t *time_data) 
{
    time_t ct = time(NULL);
    if (ct == -1) {
        fprintf(stderr, "zrub_time_utc::failed to get the current time\n");
        return false;
    }

    if (!zrub_time_get(time_data, ct)) {
        return false;
    }

    return true;
}

/**
 * @brief sets a zrub_time_t to a string.
 * 
 * @param time_data     zrub_time_t struct
 * @param time_format   time format code
 * @param str           string buffer
 * @param strlen        length of string buffer
 * @returns true if set the string, false otherwise. 
 */
bool zrub_time_set_str(const zrub_time_t time_data, short time_format, char *str, size_t strlen)
{
    if (!str) 
    {
        return false;
    }

    switch (time_format)
    {
        case ZRUB_TIME_DEFAULT:
            snprintf(str, strlen, __ZRUB_TIME_FORMAT_DEFAULT, 
                time_data.day,
                time_data.month,
                time_data.year,
                time_data.hour,
                time_data.min,
                time_data.sec);
            break;

        case ZRUB_TIME_DATEONLY:
            snprintf(str, strlen, __ZRUB_TIME_FORMAT_DATEONLY, 
                time_data.day,
                time_data.month,
                time_data.year);
            break;

        case ZRUB_TIME_TIMEONLY:
            snprintf(str, strlen, __ZRUB_TIME_FORMAT_TIMEONLY, 
                time_data.hour,
                time_data.min,
                time_data.sec);
            break;

        default:
            break;
    }

    return true;
}

/**
 * @brief checks whether time is greater than another
 * 
 * @param t1        zrub_time_t struct
 * @param t2        zrub_time_t struct
 * @returns true if greater, false if equal or less
 */
bool zrub_time_gt(zrub_time_t t1, zrub_time_t t2)
{
    return zrub_time_to_int64(t1) > zrub_time_to_int64(t2);
}

/**
 * @brief checks whether time is lesser than another
 * 
 * @param t1        zrub_time_t struct
 * @param t2        zrub_time_t struct
 * @returns true if lesser, false if equal or greater
 */
bool zrub_time_lt(zrub_time_t t1, zrub_time_t t2)
{
    return zrub_time_to_int64(t1) < zrub_time_to_int64(t2);
}

/**
 * @brief checks whether time is equals to another
 * 
 * @param t1        zrub_time_t struct
 * @param t2        zrub_time_t struct
 * @returns true if equal, false if greater or lesser
 */
bool zrub_time_eq(zrub_time_t t1, zrub_time_t t2)
{
    return zrub_time_to_int64(t1) == zrub_time_to_int64(t2);
}

/**
 * @brief this function sleeps
 * 
 * @param ms   miliseconds to sleep
 */
void zrub_time_sleep(int ms)
{
    #if defined(__linux__)
    struct timespec ts;
    struct timespec er;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
 
    if (nanosleep(&ts, &er) == -1)
    {
        fprintf(stderr, "[%s]::sleep interrupted %lu ms early", __func__, (er.tv_sec) / 1000 + ((er.tv_nsec % 1000) / 1000000));
    }

    #elif defined(_WIN32) || defined(WIN32)
    Sleep(ms);

    #endif
}

/**
 * @brief sets zrub_time_t struct data
 * 
 * @param time_data         zrub_time_t struct
 */
void zrub_time_set(zrub_time_t *time_data, short day, short month, 
    short year, short min, short sec, short hour)
{
    time_data->day = day;
    time_data->month = month;
    time_data->year = year;
    time_data->min = min;
    time_data->sec = sec;
    time_data->hour = hour;
}