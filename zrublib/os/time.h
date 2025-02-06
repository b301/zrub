#ifndef _ZRUBLIB_TIME_H
#define _ZRUBLIB_TIME_H

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#if defined(__linux__)
#include <unistd.h>

#elif defined(_WIN32)
#include <windows.h>

#endif

#include "zrublib/pub.h"
#include "zrublib/types/string.h"

#define __ZRUBLIB_TIME_FORMAT_DEFAULT "%02d-%02d-%04d %02d:%02d:%02d"
#define __ZRUBLIB_TIME_FORMAT_DATEONLY "%02d-%02d-%04d" 
#define __ZRUBLIB_TIME_FORMAT_TIMEONLY "%02d:%02d:%02d"

#define ZRUBLIB_TIME_DEFAULT 0
#define ZRUBLIB_TIME_DATEONLY 1
#define ZRUBLIB_TIME_TIMEONLY 2

typedef enum TimeFormat {
    DEFAULT,
    DATEONLY,
    TIMEONLY,
} zrublib_time_format_t;

typedef struct Time {
    short day;
    short month;
    short year;
    short min;
    short sec;
    short hour;
} zrublib_time_t;

bool zrublib_time_from_time_t(zrublib_time_t *time_data, time_t time_t_data);
bool zrublib_time_utcnow(zrublib_time_t *time_data);

void zrublib_time_set_str(const zrublib_time_t time_data, short time_format, char *dest);
char *zrublib_time_get_str(const zrublib_time_t time_data, short time_format);

void zrublib_time_sleep(int seconds);

bool zrublib_time_gt(zrublib_time_t t1, zrublib_time_t t2);
bool zrublib_time_lt(zrublib_time_t t1, zrublib_time_t t2);
bool zrublib_time_eq(zrublib_time_t t1, zrublib_time_t t2);

void zrublib_time_set(zrublib_time_t *time_data, short day, short month, 
    short year, short min, short sec, short hour);


#endif // _ZRUBLIB_TIME_H