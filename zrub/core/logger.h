#ifndef __ZRUB_LOGGER_H__
#define __ZRUB_LOGGER_H__

// TODO: IMPL THREAD SAFETY FOR FILE?
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "zrub/core/time.h"
#include "zrub/core/types.h"

// flags for the zrub_logger_initialize(...) function.
#define ZRUB_LOGGER_FLAG_DEBUG          (1 << 0)
#define ZRUB_LOGGER_FLAG_VERBOSE        (1 << 1)
#define ZRUB_LOGGER_FLAG_OUTPUTONLY     (1 << 2)
#define ZRUB_LOGGER_FLAG_TIME           (1 << 3)

// codes for the _zrub_log(...)
enum zrub_loglevel {
    LOGINFO,
    LOGERROR,
    LOGWARNING,
    LOGDEBUG,
    LOGCHECK
};

/** 
 * @struct zrub_logger
 * @brief structure that represents a logger and it's configuration.
 * 
 * if the flag `OUTPUTONLY` is set then the file attribute will point to null.
 */
struct zrub_logger {
    FILE *file;
    bool debug_mode;
    bool verbose_mode;
    bool output_only;
    bool show_time;
};

bool zrub_logger_initialize(struct zrub_logger *logger, char *logfile, i32 flags);
void _zrub_log(struct zrub_logger *logger, enum zrub_loglevel loglevel, char *format, ...);
void zrub_logger_finalize(struct zrub_logger *logger);


#endif // __ZRUB_LOGGER_H__
