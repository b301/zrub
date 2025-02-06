#ifndef _ZRUB_LOGGER_H
#define _ZRUB_LOGGER_H


#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "zrub/os/time.h"
#include "zrub/pub.h"

typedef struct Logger {
    FILE *file;
    bool debug_mode;
    bool verbose_mode;
} zrub_logger_t;

#define ZRUB_LOGGER_DEBUG_MODE   (1 << 0)
#define ZRUB_LOGGER_VERBOSE_MODE (1 << 1)

#define ZRUB_LOG_INFO 0
#define ZRUB_LOG_ERROR 1
#define ZRUB_LOG_WARNING 2
#define ZRUB_LOG_DEBUG 3

bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags);
void zrub_log(zrub_logger_t *logger, short level, char *format, ...);
void zrub_logger_finalize(zrub_logger_t *logger);

#endif // _ZRUB_LOGGER_H
