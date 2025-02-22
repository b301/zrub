#include "logger.h"


// using the `constructor` and `destructor` attributes to generate a global logger for zrublib before and after main() calls.
zrub_logger_t g_zrub_global_logger;

#ifdef ZRUBLIB_DEBUG
__attribute__((constructor))
static void g_zrub_global_logger_initialize()
{
    if (!zrub_logger_initialize(&g_zrub_global_logger, NULL, ZRUB_LOGGER_FLAG_OUTPUTONLY | ZRUB_LOGGER_FLAG_DEBUG))
    {
        fprintf(stderr, "Failed to initialize global logger\n");
    }
}

__attribute__((destructor))
static void g_zrub_global_logger_finalize()
{
    zrub_logger_finalize(&g_zrub_global_logger);
}

#else
__attribute__((constructor))
static void g_zrub_global_logger_initialize()
{
    if (!zrub_logger_initialize(&g_zrub_global_logger, NULL, ZRUB_LOGGER_FLAG_OUTPUTONLY))
    {
        fprintf(stderr, "Failed to initialize global logger\n");
    }
}

__attribute__((destructor))
static void g_zrub_global_logger_finalize()
{
    zrub_logger_finalize(&g_zrub_global_logger);
}

#endif

/** 
 * @brief initializes the logger.
 *
 * TODO: implement `verbose_mode`
 * 
 * @param logger    zrub_logger_t struct to initialize.
 * @param logfile   path of the log file on the filesystem.
 * @param flags     bit flags to configure the logger.
 * @return bool reflecting whether the struct was initialized successfully.
 */
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

/**
 * @brief writes to the log.
 * 
 * @param logger    zrub_logger_t struct.
 * @param level     #ZRUB_LOG_CODE_XXX macro defined in logger.h.
 * @param format    string format for fprintf.
 * @param va_args   passed onto fprintf.
 */
void _zrub_log(zrub_logger_t *logger, short loglevel, char *format, ...)
{
    if (logger == NULL)
    {
        return;
    }

    if (logger->output_only == false && logger->file == NULL)
    {
        return;
    }

    if (logger->debug_mode == false && loglevel == ZRUB_LOG_CODE_DEBUG)
    {
        return;
    }
    
    va_list args;
    va_start(args, format);
    
    va_list args_copy;
    va_copy(args_copy, args);

    char *level_str;
    FILE *output_stream;

    switch (loglevel)
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
    bool got_time = true;

    if (logger->show_time)
    {
        // incase couldn't get the time, do not print the time.
        if (!zrub_time_utcnow(&time))
        {
            fprintf(stderr, "[%s]::logger failed to retrieve the time\n", __func__);
            got_time = false;
            goto got_time_false;
        }

        zrub_time_set_str(time, ZRUB_TIME_DEFAULT, time_str, 64);
    }
got_time_false:

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

    if (logger->show_time && got_time == true)
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

/**
 * @brief finalizes the logger.
 * 
 * @param logger    zrub_logger_t struct.
 */
void zrub_logger_finalize(zrub_logger_t *logger)
{
    if (!logger) return;

    if (!logger->output_only)
    {
        fclose(logger->file);
    }
}
