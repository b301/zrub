#include "logger.h"


bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags)
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
    logger->debug_mode = (flags & ZRUB_LOGGER_DEBUG_MODE) == 0;
    logger->verbose_mode = (flags & ZRUB_LOGGER_DEBUG_MODE) == 0;

    return true;
}

void zrub_log(zrub_logger_t *logger, short level, char *format, ...)
{
    if (logger == NULL || logger->file == NULL)
        return;

    if (logger->debug_mode == false && level == ZRUB_LOG_DEBUG)
    {
        return;
    }

    va_list args;
    va_start(args, format);

    zrub_time_t time;
    char time_str[64];

    zrub_time_utcnow(&time);
    zrub_time_set_str(time, ZRUB_TIME_DEFAULT, time_str);

    const char *level_str;
    FILE *output_stream;

    switch (level)
    {
        case ZRUB_LOG_ERROR:
            level_str = "error";
            output_stream = stderr;
            break;

        case ZRUB_LOG_INFO:
            level_str = "info";
            output_stream = stdout;
            break;

        case ZRUB_LOG_WARNING:
            level_str = "warning";
            output_stream = stdout;
            break;

        case ZRUB_LOG_DEBUG:
            level_str = "debug";
            output_stream = stdout;
            break;

        default:
            va_end(args);
            return;
    }

    fprintf(logger->file, "[%s]::[%s]::", time_str, level_str);
    vfprintf(logger->file, format, args);
    fprintf(logger->file, "\n");

    printf("Wrote to file\n");

    va_list args_copy;
    va_copy(args_copy, args);
    fprintf(output_stream, "[%s]::[%s]::", time_str, level_str);
    vfprintf(output_stream, format, args_copy);
    fprintf(output_stream, "\n");

    va_end(args_copy);
    va_end(args);
}

void zrub_logger_finalize(zrub_logger_t *logger)
{
    if (!logger) return;

    fclose(logger->file);
}