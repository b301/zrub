#ifndef __ZRUB_LOGGER_H__
#define __ZRUB_LOGGER_H__


extern struct zrub_logger g_zrub_global_logger;

#define _ZRUB_LOG_FUNCMACRO "[%s]::"

#define ZRUB_LOG_INFO(format, ...)      _zrub_log(&g_zrub_global_logger, LOGINFO, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_ERROR(format, ...)     _zrub_log(&g_zrub_global_logger, LOGERROR, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_WARNING(format, ...)   _zrub_log(&g_zrub_global_logger, LOGWARNING, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_DEBUG(format, ...)     _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_CHECK(format, ...)     _zrub_log(&g_zrub_global_logger, LOGCHECK, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_PERF(format, ...)      _zrub_log(&g_zrub_global_logger, LOGPERF, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)

#define ZRUB_TESTS_CHECK(statement, message)    \
if (!(statement)) {                             \
    ZRUB_LOG_CHECK("%s didn't pass", message);  \
    return;                                     \
}                                               \
ZRUB_LOG_CHECK("%s passed", message)

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
    LOGCHECK,
    LOGPERF
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

bool zrub_logger_initialize(struct zrub_logger *logger, char *logfile, int32_t flags);
void _zrub_log(struct zrub_logger *logger, enum zrub_loglevel loglevel, char *format, ...);
void zrub_logger_finalize(struct zrub_logger *logger);


#endif // __ZRUB_LOGGER_H__
