#ifndef __ZRUB_LOGGER_H__
#define __ZRUB_LOGGER_H__

// TODO: IMPL THREAD SAFETY?
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "time.h"

#define ZRUB_LOGGER_FLAG_DEBUG          (1 << 0)
#define ZRUB_LOGGER_FLAG_VERBOSE        (1 << 1)
#define ZRUB_LOGGER_FLAG_OUTPUTONLY     (1 << 2)
#define ZRUB_LOGGER_FLAG_TIME           (1 << 3)

#define ZRUB_LOG_CODE_INFO      0
#define ZRUB_LOG_CODE_ERROR     1
#define ZRUB_LOG_CODE_WARNING   2
#define ZRUB_LOG_CODE_DEBUG     3
#define ZRUB_LOG_CODE_CHECK     4

typedef struct Logger {
    FILE *file;
    bool debug_mode;
    bool verbose_mode;
    bool output_only;
    bool show_time;
} zrub_logger_t;

bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags);
void _zrub_log(zrub_logger_t *logger, short level, char *format, ...);
void zrub_logger_finalize(zrub_logger_t *logger);


#endif // __ZRUB_LOGGER_H__
