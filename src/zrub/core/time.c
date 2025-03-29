#include "zrub/core/time.h"


/**
 * @brief returns the epoch time (seconds elapsed since jan 1, 1970) 
 * @returns unsigned long long (64-bit) 
 */
bool zrub_time_epoch(uint64_t *tv)
{
    #if defined(WIN32) || defined(_WIN32)
    ZRUB_NOT_IMPLEMENTED(false);

    #elif defined(__linux__)
    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        fprintf(stderr, "clock_gettime failed to get CLOCK_REALTIME\n");
        return false;
    }

    *tv = ts.tv_sec;

    return true;
    #endif
}

/**
 * @brief converts struct zrub_time to int64.
 * 
 * @param time      struct zrub_time
 * @returns struct zrub_time value as int64
 */
static int64_t zrub_time_as_int64(const struct zrub_time time)
{
    return ((int64_t)time.year * 10000000000LL) +
           ((int64_t)time.month * 100000000LL) +
           ((int64_t)time.day * 1000000LL) +
           ((int64_t)time.hour * 10000LL) +
           ((int64_t)time.min * 100LL) +
           (int64_t)time.sec;
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

    time_data->day = (int16_t)(t->tm_mday);
    time_data->month = (int16_t)(t->tm_mon + 1);
    time_data->year = (int16_t)(t->tm_year + 1900);
    
    time_data->sec = (int16_t)(t->tm_sec);
    time_data->min = (int16_t)(t->tm_min);
    time_data->hour = (int16_t)(t->tm_hour);

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
void zrub_time_sleep(int32_t ms)
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
void zrub_time_set(struct zrub_time *time_data, int16_t day, int16_t month, 
    int16_t year, int16_t min, int16_t sec, int16_t hour)
{
    time_data->day = day;
    time_data->month = month;
    time_data->year = year;
    time_data->min = min;
    time_data->sec = sec;
    time_data->hour = hour;
}

int64_t zrub_time_tsdiff(struct timespec *end, struct timespec *start)
{
    int64_t sec_diff = end->tv_sec - start->tv_sec;
    int64_t nsec_diff = end->tv_nsec - start->tv_nsec;
    
    // Handle potential underflow in nanoseconds
    if (nsec_diff < 0) {
        sec_diff--;
        nsec_diff += 1000000000L; // 1 second = 1e9 ns
    }
    
    return sec_diff * 1000000000L + nsec_diff;
}
