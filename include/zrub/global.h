#ifndef __ZRUB_GLOBAL_H__
#define __ZRUB_GLOBAL_H__

#include "zrub/core/types.h"
#include "zrub/core/logger.h"

#define ZRUB_DEPRECATED         ZRUB_LOG_WARNING("this function is deprecated\n")
#define ZRUB_OBSOLETE(repl)     ZRUB_LOG_WARNING("this function is obsolete, you should use %s\n", repl)
#define ZRUB_NOT_IMPLEMENTED    ZRUB_LOG_ERROR("this function is not implemented\n")

/* memory */
#define ZRUB_MALLOC         malloc
#define ZRUB_FREE           free
#define ZRUB_REALLOC        realloc
#define ZRUB_CALLOC         calloc

#define ZRUB_UNUSED(x)      (void)(x)


#endif /* __ZRUB_GLOBAL_H__ */
