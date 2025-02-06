#ifndef _CSLIB_TIME_H
#define _CSLIB_TIME_H

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#if defined(__linux__)
#include <unistd.h>

#elif defined(_WIN32)
#include <windows.h>

#endif

#include "cslib/pub.h"
#include "cslib/types/string.h"

#define __CSLIB_TIME_FORMAT_DEFAULT "%02d-%02d-%04d %02d:%02d:%02d"
#define __CSLIB_TIME_FORMAT_DATEONLY "%02d-%02d-%04d" 
#define __CSLIB_TIME_FORMAT_TIMEONLY "%02d:%02d:%02d"

#define CSLIB_TIME_DEFAULT 0
#define CSLIB_TIME_DATEONLY 1
#define CSLIB_TIME_TIMEONLY 2

typedef enum TimeFormat {
    DEFAULT,
    DATEONLY,
    TIMEONLY,
} cslib_time_format_t;

typedef struct Time {
    short day;
    short month;
    short year;
    short min;
    short sec;
    short hour;
} cslib_time_t;

bool cslib_time_from_time_t(cslib_time_t *time_data, time_t time_t_data);
bool cslib_time_utcnow(cslib_time_t *time_data);

void cslib_time_set_str(const cslib_time_t time_data, short time_format, char *dest);
char *cslib_time_get_str(const cslib_time_t time_data, short time_format);

void cslib_time_sleep(int seconds);

bool cslib_time_gt(cslib_time_t t1, cslib_time_t t2);
bool cslib_time_lt(cslib_time_t t1, cslib_time_t t2);
bool cslib_time_eq(cslib_time_t t1, cslib_time_t t2);

void cslib_time_set(cslib_time_t *time_data, short day, short month, 
    short year, short min, short sec, short hour);


#endif // _CSLIB_TIME_H