#ifndef __ZRUB_TIME_H__
#define __ZRUB_TIME_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(__linux__)
#include <unistd.h>

#elif defined(_WIN32)
#include <windows.h>

#endif

#define __ZRUB_TIME_FORMAT_DEFAULT "%02d-%02d-%04d %02d:%02d:%02d"
#define __ZRUB_TIME_FORMAT_DATEONLY "%02d-%02d-%04d" 
#define __ZRUB_TIME_FORMAT_TIMEONLY "%02d:%02d:%02d"

#define ZRUB_TIME_DEFAULT   0
#define ZRUB_TIME_DATEONLY  1
#define ZRUB_TIME_TIMEONLY  2

/**
 * @struct zrub_time_t
 * @brief structure that represents time.
 */
typedef struct Time {
    short day;
    short month;
    short year;
    short min;
    short sec;
    short hour;
} zrub_time_t;

bool zrub_time_get(zrub_time_t *time_data, time_t time_t_data);
bool zrub_time_utcnow(zrub_time_t *time_data);

bool zrub_time_set_str(const zrub_time_t time_data, short time_format, char *str, size_t strlen);
void zrub_time_sleep(int seconds);

bool zrub_time_gt(zrub_time_t t1, zrub_time_t t2);
bool zrub_time_lt(zrub_time_t t1, zrub_time_t t2);
bool zrub_time_eq(zrub_time_t t1, zrub_time_t t2);

void zrub_time_set(zrub_time_t *time_data, short day, short month, 
    short year, short min, short sec, short hour);


#endif // __ZRUB_TIME_H__