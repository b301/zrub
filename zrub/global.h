#ifndef __ZRUB_GLOBAL_H__
#define __ZRUB_GLOBAL_H__

#include "zrub/core/types.h"
#include "zrub/core/logger.h"
extern struct zrub_logger g_zrub_global_logger;

#define _ZRUB_LOG_FUNCMACRO "[%s]::"


#define zrub_log(logger, code, format, ...) _zrub_log(logger, code, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_info(logger, format, ...) _zrub_log(logger, LOGINFO, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_error(logger, format, ...) _zrub_log(logger, LOGERROR, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_warning(logger, format, ...) _zrub_log(logger, LOGWARNING, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_debug(logger, format, ...) _zrub_log(logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)

#define ZRUB_LOG_INFO(format, ...)      _zrub_log(&g_zrub_global_logger, LOGINFO, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_ERROR(format, ...)     _zrub_log(&g_zrub_global_logger, LOGERROR, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_WARNING(format, ...)   _zrub_log(&g_zrub_global_logger, LOGWARNING, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_DEBUG(format, ...)     _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_CHECK(format, ...)     _zrub_log(&g_zrub_global_logger, LOGCHECK, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)

#define ZRUB_TESTS_CHECK(statement, message)    \
if (!(statement)) {                             \
    ZRUB_LOG_CHECK("%s didn't pass", message);  \
    return;                                     \
}                                               \
ZRUB_LOG_CHECK("%s passed", message)


#endif // __ZRUB_GLOBAL_H__
