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

/*
@note: __total should be set to a pointer of int64_t (long long), i.e.
{
    int64_t total = 0;
    ZRUB_MEASURE_PERF(printf("hello, world!\n"), &total);
}
*/ 
#ifdef ZRUBLIB_DEBUG_PERF
#define ZRUB_MEASURE_PERF(__zperffunc, __total)                 \
{                                                               \
    struct timespec __zperfpre, __zperfpost;                    \
    timespec_get(&__zperfpre, TIME_UTC);                        \
    __zperffunc;                                                \
    timespec_get(&__zperfpost, TIME_UTC);                       \
    ZRUB_LOG_PERF("%s took %lld nanoseconds\n", #__zperffunc,   \
        zrub_time_tsdiff(&__zperfpost, &__zperfpre));           \
    if (__total != NULL) *__total += zrub_time_tsdiff(&__zperfpost, &__zperfpre); \
}

#define ZRUB_JUST_MEASURE_PERF(__zperffunc)                     \
{                                                               \
    struct timespec __zperfpre, __zperfpost;                    \
    timespec_get(&__zperfpre, TIME_UTC);                        \
    __zperffunc;                                                \
    timespec_get(&__zperfpost, TIME_UTC);                       \
    ZRUB_LOG_PERF("%s took %lld nanoseconds\n", #__zperffunc,   \
        zrub_time_tsdiff(&__zperfpost, &__zperfpre));           \
}

#else
#define ZRUB_MEASURE_PERF(__zperffunc, __total) __zperffunc  
#define ZRUB_JUST_MEASURE_PERF(__zperffunc) __zperffunc

#endif


/* memory */
#define ZRUB_MALLOC         malloc
#define ZRUB_FREE           free
#define ZRUB_REALLOC        realloc
#define ZRUB_CALLOC         calloc

#define ZRUB_UNUSED(x)      (void)(x)


#endif /* __ZRUB_GLOBAL_H__ */
