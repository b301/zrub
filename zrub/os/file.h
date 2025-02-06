#ifndef _ZRUB_FILE_H
#define _ZRUB_FILE_H

#include "zrub/types/string.h"
#include "zrub/types/vector.h"
#include "zrub/pub.h"

#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#elif defined(__linux__)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#endif


typedef enum OsFileType 
{
    OS_FILE_REGULAR,
    OS_FILE_DIRECTORY
} zrub_os_file_type_t;

typedef struct OsFile 
{
    char *name;
    zrub_os_file_type_t type;
} zrub_os_file_t;

bool zrub_list_directory(zrub_vector_t *vec, char *path, ssize_t depth);
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
        "name": "zrub",
        "type": "directory"
    }
]

*/

#endif // _ZRUB_FILE_H