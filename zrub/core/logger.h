#ifndef _ZRUB_LOGGER_H
#define _ZRUB_LOGGER_H


// TODO: IMPL THREAD SAFETY?

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "zrub/os/time.h"

typedef struct Logger {
    FILE *file;
    bool debug_mode;
    bool verbose_mode;
    bool output_only;
    bool show_time;
} zrub_logger_t;

#define ZRUB_LOGGER_DEBUG_MODE      (1 << 0)
#define ZRUB_LOGGER_VERBOSE_MODE    (1 << 1)
#define ZRUB_LOGGER_OUTPUT_ONLY     (1 << 2)
#define ZRUB_LOGGER_SHOW_TIME       (1 << 3)

#define ZRUB_LOG_INFO_CODE      0
#define ZRUB_LOG_ERROR_CODE     1
#define ZRUB_LOG_WARNING_CODE   2
#define ZRUB_LOG_DEBUG_CODE     3

bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags);
void _zrub_log(zrub_logger_t *logger, short level, char *format, ...);
void zrub_logger_finalize(zrub_logger_t *logger);

#endif // _ZRUB_LOGGER_H
