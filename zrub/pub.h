#ifndef _ZRUB_PUB_H
#define _ZRUB_PUB_H

#include <stdio.h>
#include <stdlib.h>

#include "zrub/core/logger.h"
extern zrub_logger_t _zrub_global_logger;

#define ZRUB_MALLOC malloc
#define ALLOC_OBJECT(type) (type*)ZRUB_MALLOC(sizeof(type))
#define TESTS_CHECK(statement, message)                                     \
if (!(statement)) {                                                         \
    zrub_log(&_zrub_global_logger, ZRUB_LOG_ERROR, "%s failed\n", message); \
    return 1;                                                               \
}                                                                           \
zrub_log(&_zrub_global_logger, ZRUB_LOG_INFO, "%s passed", message)


#endif
