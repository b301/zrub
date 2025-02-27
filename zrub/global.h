#ifndef __ZRUB_GLOBAL_H__
#define __ZRUB_GLOBAL_H__

#include "zrub/core/logger.h"
extern zrub_logger_t g_zrub_global_logger;

#define _ZRUB_LOG_FUNCMACRO "[%s]::"


#define zrub_log(logger, code, format, ...) _zrub_log(logger, code, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_info(logger, format, ...) _zrub_log(logger, ZRUB_LOG_CODE_INFO, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_error(logger, format, ...) _zrub_log(logger, ZRUB_LOG_CODE_ERROR, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_warning(logger, format, ...) _zrub_log(logger, ZRUB_LOG_CODE_WARNING, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define zrub_log_debug(logger, format, ...) _zrub_log(logger, ZRUB_LOG_CODE_DEBUG, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)

#define ZRUB_LOG_INFO(format, ...)      _zrub_log(&g_zrub_global_logger, ZRUB_LOG_CODE_INFO, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_ERROR(format, ...)     _zrub_log(&g_zrub_global_logger, ZRUB_LOG_CODE_ERROR, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_WARNING(format, ...)   _zrub_log(&g_zrub_global_logger, ZRUB_LOG_CODE_WARNING, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_DEBUG(format, ...)     _zrub_log(&g_zrub_global_logger, ZRUB_LOG_CODE_DEBUG, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_CHECK(format, ...)     _zrub_log(&g_zrub_global_logger, ZRUB_LOG_CODE_CHECK, _ZRUB_LOG_FUNCMACRO format, __func__, ##__VA_ARGS__)

#define ZRUB_TESTS_CHECK(statement, message)    \
if (!(statement)) {                             \
    ZRUB_LOG_CHECK("%s didn't pass", message);  \
    return;                                     \
}                                               \
ZRUB_LOG_CHECK("%s passed", message)


#endif // __ZRUB_GLOBAL_H__
