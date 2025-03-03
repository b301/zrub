#include "time.h"


/**
 * @brief converts struct zrub_time to int64.
 * 
 * @param time      struct zrub_time
 * @returns struct zrub_time value as int64
 */
static i64 zrub_time_as_int64(const struct zrub_time time)
{
    return ((i64)time.year * 10000000000LL) +
           ((i64)time.month * 100000000LL) +
           ((i64)time.day * 1000000LL) +
           ((i64)time.hour * 10000LL) +
           ((i64)time.min * 100LL) +
           (i64)time.sec;
}

/**
 * @brief gets the time as of now.
 * 
 * @param time_data     struct zrub_time
 * @param time_t_data   time_t struct
 * @returns true if managed to set struct zrub_time
 */
bool zrub_time_get(struct zrub_time *time_data, time_t time_t_data)
{
    struct tm *t = gmtime(&time_t_data);

    if (t == NULL) {
        fprintf(stderr, "[%s]::failed to convert to utc\n", __func__);
        return false;
    }

    time_data->day = (i16)(t->tm_mday);
    time_data->month = (i16)(t->tm_mon + 1);
    time_data->year = (i16)(t->tm_year + 1900);
    
    time_data->sec = (i16)(t->tm_sec);
    time_data->min = (i16)(t->tm_min);
    time_data->hour = (i16)(t->tm_hour);

    return true;
}

/**
 * @brief gets the universal time coordinated (utc).
 * 
 * @param time_data     struct zrub_time
 * @returns true if managed to retrieve the time
 */
bool zrub_time_utcnow(struct zrub_time *time_data) 
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
 * @brief sets a struct zrub_time to a string.
 * 
 * @param time_data     struct zrub_time
 * @param time_format   time format code
 * @param str           string buffer
 * @param strlen        length of string buffer
 * @returns true if set the string, false otherwise. 
 */
bool zrub_time_set_str(const struct zrub_time time_data, enum zrub_timeformat time_format, char *str, size_t strlen)
{
    if (!str) 
    {
        return false;
    }

    switch (time_format)
    {
        case TIMEDEFAULT:
            snprintf(str, strlen, __ZRUB_TIME_FORMAT_DEFAULT, 
                time_data.day,
                time_data.month,
                time_data.year,
                time_data.hour,
                time_data.min,
                time_data.sec);
            break;

        case TIMEDATEONLY:
            snprintf(str, strlen, __ZRUB_TIME_FORMAT_DATEONLY, 
                time_data.day,
                time_data.month,
                time_data.year);
            break;

        case TIMETIMEONLY:
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
 * @param t1        struct zrub_time
 * @param t2        struct zrub_time
 * @returns true if greater, false if equal or less
 */
bool zrub_time_gt(struct zrub_time t1, struct zrub_time t2)
{
    return zrub_time_as_int64(t1) > zrub_time_as_int64(t2);
}

/**
 * @brief checks whether time is lesser than another
 * 
 * @param t1        struct zrub_time
 * @param t2        struct zrub_time
 * @returns true if lesser, false if equal or greater
 */
bool zrub_time_lt(struct zrub_time t1, struct zrub_time t2)
{
    return zrub_time_as_int64(t1) < zrub_time_as_int64(t2);
}

/**
 * @brief checks whether time is equals to another
 * 
 * @param t1        struct zrub_time
 * @param t2        struct zrub_time
 * @returns true if equal, false if greater or lesser
 */
bool zrub_time_eq(struct zrub_time t1, struct zrub_time t2)
{
    return zrub_time_as_int64(t1) == zrub_time_as_int64(t2);
}

/**
 * @brief this function sleeps
 * 
 * @param ms   miliseconds to sleep
 */
void zrub_time_sleep(i32 ms)
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
 * @brief sets struct zrub_time data
 * 
 * @param time_data         struct zrub_time
 */
void zrub_time_set(struct zrub_time *time_data, i16 day, i16 month, 
    i16 year, i16 min, i16 sec, i16 hour)
{
    time_data->day = day;
    time_data->month = month;
    time_data->year = year;
    time_data->min = min;
    time_data->sec = sec;
    time_data->hour = hour;
}
