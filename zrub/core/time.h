#ifndef __ZRUB_TIME_H__
#define __ZRUB_TIME_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "zrub/core/types.h"

#if defined(__linux__)
#include <unistd.h>

#elif defined(_WIN32)
#include <windows.h>

#endif

#define __ZRUB_TIME_FORMAT_DEFAULT "%02d-%02d-%04d %02d:%02d:%02d"
#define __ZRUB_TIME_FORMAT_DATEONLY "%02d-%02d-%04d" 
#define __ZRUB_TIME_FORMAT_TIMEONLY "%02d:%02d:%02d"

/**
 * @enum zrub_timeformat
 * @brief specifies the time format when converting to a string
 */
enum zrub_timeformat {
    TIMEDEFAULT,
    TIMEDATEONLY,
    TIMETIMEONLY
};

/**
 * @struct zrub_time
 * @brief structure that represents time.
 */
struct zrub_time {
    i16 day;
    i16 month;
    i16 year;
    i16 min;
    i16 sec;
    i16 hour;
};

bool zrub_time_get(struct zrub_time *time_data, time_t time_t_data);
bool zrub_time_utcnow(struct zrub_time *time_data);

bool zrub_time_set_str(const struct zrub_time time_data, enum zrub_timeformat time_format, char *str, size_t strlen);
void zrub_time_sleep(i32 ms);

bool zrub_time_gt(struct zrub_time t1, struct zrub_time t2);
bool zrub_time_lt(struct zrub_time t1, struct zrub_time t2);
bool zrub_time_eq(struct zrub_time t1, struct zrub_time t2);

void zrub_time_set(struct zrub_time *time_data, i16 day, i16 month, 
    i16 year, i16 min, i16 sec, i16 hour);


#endif // __ZRUB_TIME_H__
