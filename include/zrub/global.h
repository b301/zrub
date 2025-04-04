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
#if defined(ZRUBLIB_DEBUG) || defined(ZRUBLIB_DEBUG_PERF)
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

#if defined(ZRUBLIB_DEBUG) || defined(ZRUBLIB_DEBUG_VARS)
#define ZRUB_DVAR_STRING(var)           \
    _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO "(length: %d) " #var ": %s\n", __func__, zrub_str_len(var), (var))
#define ZRUB_DVAR_INT(var)              \
    _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO #var " %lld\n", __func__, (int64_t)(var))
#define ZRUB_DVAR_UINT(var)             \
    _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO #var " %llu\n", __func__, (uint64_t)(var))
#define ZRUB_DVAR_FLOAT(var)            \
    _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO #var " %f\n", __func__, (double)(var))
#define ZRUB_DVAR_BYTES(var, len)       \
    _zrub_log(&g_zrub_global_logger, LOGDEBUG, _ZRUB_LOG_FUNCMACRO "(length: %u) " #var " ", __func__, (uint32_t)(len)); \
    zrub_bytes_print(var, (uint32_t)(len));

#else
#define ZRUB_DVAR_STRING(var)
#define ZRUB_DVAR_INT(var)
#define ZRUB_DVAR_UINT(var)
#define ZRUB_DVAR_FLOAT(var)
#define ZRUB_DVAR_BYTES(var, len);

#endif


/* memory */
#define ZRUB_MALLOC         malloc
#define ZRUB_FREE           free
#define ZRUB_REALLOC        realloc
#define ZRUB_CALLOC         calloc

#define ZRUB_UNUSED(x)      (void)(x)


#endif /* __ZRUB_GLOBAL_H__ */
