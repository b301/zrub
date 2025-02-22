#include "logger.h"


zrub_logger_t _zrub_global_logger;

#ifdef ZRUBLIB_DEBUG
__attribute__((constructor))
static void _zrub_global_logger_initialize()
{
    if (!zrub_logger_initialize(&_zrub_global_logger, NULL, ZRUB_LOGGER_FLAG_OUTPUTONLY | ZRUB_LOGGER_FLAG_DEBUG))
    {
        fprintf(stderr, "Failed to initialize global logger\n");
    }
}

__attribute__((destructor))
static void _zrub_global_logger_finalize()
{
    zrub_logger_finalize(&_zrub_global_logger);
}

#else
__attribute__((constructor))
static void _zrub_global_logger_initialize()
{
    if (!zrub_logger_initialize(&_zrub_global_logger, NULL, ZRUB_LOGGER_FLAG_OUTPUTONLY))
    {
        fprintf(stderr, "Failed to initialize global logger\n");
    }
}

__attribute__((destructor))
static void _zrub_global_logger_finalize()
{
    zrub_logger_finalize(&_zrub_global_logger);
}

#endif


bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags)
{
    logger->output_only = (flags & ZRUB_LOGGER_FLAG_OUTPUTONLY) != 0;

    if (logger->output_only)
    {
        logger->file = NULL;
    }
    else
    {
        // open logfile with append
        FILE *fptr;
        fptr = fopen(logfile, "a");

        if (fptr == NULL)
        {
            fprintf(stderr, "failed to open logfile %s\n", logfile);
            return false;
        }

        logger->file = fptr;
    }

    logger->debug_mode = (flags & ZRUB_LOGGER_FLAG_DEBUG) != 0;
    logger->verbose_mode = (flags & ZRUB_LOGGER_FLAG_VERBOSE) != 0;
    logger->show_time = (flags & ZRUB_LOGGER_FLAG_TIME) != 0;

    return true;
}

void _zrub_log(zrub_logger_t *logger, short level, char *format, ...)
{
    if (logger == NULL)
    {
        return;
    }

    if (logger->output_only == false && logger->file == NULL)
    {
        return;
    }

    if (logger->debug_mode == false && level == ZRUB_LOG_CODE_DEBUG)
    {
        return;
    }
    
    va_list args;
    va_start(args, format);
    
    va_list args_copy;
    va_copy(args_copy, args);

    char *level_str;
    FILE *output_stream;

    switch (level)
    {
        case ZRUB_LOG_CODE_ERROR:
            level_str = "error";
            output_stream = stderr;
            break;

        case ZRUB_LOG_CODE_INFO:
            level_str = "info";
            output_stream = stdout;
            break;

        case ZRUB_LOG_CODE_WARNING:
            level_str = "warning";
            output_stream = stdout;
            break;

        case ZRUB_LOG_CODE_DEBUG:
            level_str = "debug";
            output_stream = stdout;
            break;
        
        case ZRUB_LOG_CODE_CHECK:
            level_str = "check";
            output_stream = stdout;
            break;

        default:
            goto cleanup;
    }

    zrub_time_t time;
    char time_str[64];

    if (logger->show_time)
    {
        zrub_time_utcnow(&time);
        zrub_time_set_str(time, ZRUB_TIME_DEFAULT, time_str);
    }

    if (!logger->output_only)
    {
        if (logger->show_time)
        {
            fprintf(logger->file, "[%s]::", time_str);    
        }

        fprintf(logger->file, "[%s]::", level_str);
        vfprintf(logger->file, format, args);
        fprintf(logger->file, "\n");
    }

    if (logger->show_time)
    {
        fprintf(output_stream, "[%s]::", time_str);        
    }

    fprintf(output_stream, "[%s]::", level_str);
    vfprintf(output_stream, format, args_copy);
    fprintf(output_stream, "\n");

cleanup:
    va_end(args_copy);
    va_end(args);
}

void zrub_logger_finalize(zrub_logger_t *logger)
{
    if (!logger) return;

    if (!logger->output_only)
    {
        fclose(logger->file);
    }
}
