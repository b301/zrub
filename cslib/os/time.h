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

typedef struct Time {
    short day;
    short month;
    short year;
    short min;
    short sec;
    short hour;
} cslib_time_t;

bool cslib_time_utcnow(cslib_time_t *time_data);
char *cslib_time_as_str(cslib_time_t time_data);
void cslib_time_sleep(int seconds);

bool cslib_time_gt(cslib_time_t t1, cslib_time_t t2);
bool cslib_time_lt(cslib_time_t t1, cslib_time_t t2);
bool cslib_time_eq(cslib_time_t t1, cslib_time_t t2);


#endif // _CSLIB_TIME_H