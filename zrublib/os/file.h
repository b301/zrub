#ifndef _ZRUBLIB_FILE_H
#define _ZRUBLIB_FILE_H

#include "zrublib/types/string.h"
#include "zrublib/types/vector.h"
#include "zrublib/pub.h"

#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#elif defined(__linux__)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "zrublib/pub.h"

#endif


typedef enum OsFileType 
{
    OS_FILE_REGULAR,
    OS_FILE_DIRECTORY
} zrublib_os_file_type_t;

typedef struct OsFile 
{
    char *name;
    zrublib_os_file_type_t type;
} zrublib_os_file_t;

bool zrublib_list_directory(zrublib_vector_t *vec, char *path, ssize_t depth);
/* example output

Vector::[
    OsFileType::{
        "name": "windows.h",
        "type": "regular"
    },
    OsFileType::{
        "name": "gcc.exe",
        "type": "regular"
    },
    OsFileType::{
        "name": "zrublib",
        "type": "directory"
    }
]

*/

#endif // _ZRUBLIB_FILE_H