#ifndef __ZRUB_GLOBAL_H__
#define __ZRUB_GLOBAL_H__

#include <stdint.h>
#include "zrub/core/logger.h"

#define ZRUB_DEPRECATED             \
    ZRUB_LOG_WARNING("this function is deprecated\n")
#define ZRUB_OBSOLETE(repl)         \
    ZRUB_LOG_WARNING("this function is obsolete, you should use %s\n", repl)
#define ZRUB_NOT_IMPLEMENTED(rc)    \
    ZRUB_LOG_ERROR("this function is not implemented\n"); return rc
#define ZRUB_TODO(msg)  \
    ZRUB_LOG_INFO("%s\n", msg)

/* memory */
#define ZRUB_MALLOC         malloc
#define ZRUB_FREE           free
#define ZRUB_REALLOC        realloc
#define ZRUB_CALLOC         calloc

#define ZRUB_UNUSED(x)      (void)(x)


#endif /* __ZRUB_GLOBAL_H__ */
