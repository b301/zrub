#ifndef _ZRUB_PUB_H
#define _ZRUB_PUB_H

#include <stdio.h>
#include <stdlib.h>

#include "zrub/core/logger.h"
extern zrub_logger_t _zrub_global_logger;

#define zrub_log(logger, code, format, ...) _zrub_log(logger, code, "[%s]::" format, __func__, ##__VA_ARGS__)
#define zrub_log_info(logger, format, ...) _zrub_log(logger, ZRUB_LOG_INFO_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define zrub_log_error(logger, format, ...) _zrub_log(logger, ZRUB_LOG_ERROR_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define zrub_log_warning(logger, format, ...) _zrub_log(logger, ZRUB_LOG_WARNING_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define zrub_log_debug(logger, format, ...) _zrub_log(logger, ZRUB_LOG_DEBUG_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)

#define ZRUB_LOG_INFO(format, ...)      _zrub_log(&_zrub_global_logger, ZRUB_LOG_INFO_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_ERROR(format, ...)     _zrub_log(&_zrub_global_logger, ZRUB_LOG_ERROR_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_WARNING(format, ...)   _zrub_log(&_zrub_global_logger, ZRUB_LOG_WARNING_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)
#define ZRUB_LOG_DEBUG(format, ...)     _zrub_log(&_zrub_global_logger, ZRUB_LOG_DEBUG_CODE, "[%s]::" format, __func__, ##__VA_ARGS__)


#define ZRUB_MALLOC malloc
#define ALLOC_OBJECT(type) (type*)ZRUB_MALLOC(sizeof(type))

#define TESTS_CHECK(statement, message)                                             \
if (!(statement)) {                                                                 \
    ZRUB_LOG_ERROR("%s failed\n", message);    \
    return 1;                                                                       \
}                                                                                   \
ZRUB_LOG_INFO("%s passed", message)


#endif
