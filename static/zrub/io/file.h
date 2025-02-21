#ifndef __ZRUB_FILE_H__
#define __ZRUB_FILE_H__

#include "zrub/global.h"
#include <stdio.h>

#if defined(WIN32)
#include <windows.h>

#elif defined(__linux__)
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#endif

bool zrub_io_filesize(const char *filepath, long long *size);


#endif // __ZRUB_FILE_H__
