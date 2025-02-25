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

#include <ftw.h>

#endif

bool zrub_io_dir_size(const char *dirpath, long long *size);
bool zrub_io_file_size(const char *filepath, long long *size);
bool zrub_io_file_exists(const char *filepath);


#endif // __ZRUB_FILE_H__
