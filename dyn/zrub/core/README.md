# ZRUB Logger

## Overview
The **ZRUB Logger** is a lightweight logging utility for C programs. It provides logging functionality with different verbosity levels and optional time-stamping.

## Features
- Configurable logging levels
- Optional timestamp inclusion
- Different verbosity modes
- Supports logging to a file

## Thread Safety
> **TODO:** Implementation of thread safety is pending.

## Data Structures

### `zrub_logger_t`
The main logger structure:
```c
typedef struct Logger {
    FILE *file;         // Log file handle
    bool debug_mode;    // Enable debug messages
    bool verbose_mode;  // Enable verbose messages
    bool output_only;   // Only output messages, no logs
    bool show_time;     // Show timestamps in logs
} zrub_logger_t;
```

## Logging Flags
Bitwise flags used to configure the logger:
```c
#define ZRUB_LOGGER_DEBUG_MODE      (1 << 0) // Enable debug mode
#define ZRUB_LOGGER_VERBOSE_MODE    (1 << 1) // Enable verbose mode
#define ZRUB_LOGGER_OUTPUT_ONLY     (1 << 2) // Output messages only, no logs
#define ZRUB_LOGGER_SHOW_TIME       (1 << 3) // Include timestamps in logs
```

## Logging Levels
Different levels of logging messages:
```c
#define ZRUB_LOG_INFO_CODE      0 // Informational messages
#define ZRUB_LOG_ERROR_CODE     1 // Error messages
#define ZRUB_LOG_WARNING_CODE   2 // Warning messages
#define ZRUB_LOG_DEBUG_CODE     3 // Debugging messages
#define ZRUB_LOG_CHECK_CODE     4 // Checks and assertions
```

## Functions

### `bool zrub_logger_initialize(zrub_logger_t *logger, char *logfile, int flags);`
Initializes the logger.
- **logger**: Pointer to `zrub_logger_t` instance
- **logfile**: Path to log file
- **flags**: Combination of logging flags
- **Returns**: `true` on success, `false` on failure

### `void _zrub_log(zrub_logger_t *logger, short level, char *format, ...);`
Logs a message.
- **logger**: Pointer to initialized `zrub_logger_t`
- **level**: Logging level
- **format**: Message format string
- **...**: Variable arguments for formatting

### `void zrub_logger_finalize(zrub_logger_t *logger);`
Finalizes and closes the logger.
- **logger**: Pointer to `zrub_logger_t` instance

## Usage Example
```c
#include "zrub_logger.h"

int main() {
    zrub_logger_t logger;
    if (!zrub_logger_initialize(&logger, "app.log", ZRUB_LOGGER_DEBUG_MODE | ZRUB_LOGGER_SHOW_TIME)) {
        return 1;
    }
    
    _zrub_log(&logger, ZRUB_LOG_INFO_CODE, "Application started");
    _zrub_log(&logger, ZRUB_LOG_ERROR_CODE, "An error occurred: %d", -1);
    
    zrub_logger_finalize(&logger);
    return 0;
}
```

## Future Enhancements
- Implement thread safety
- Add log rotation support
- Support for different output formats
