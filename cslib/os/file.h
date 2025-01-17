#ifndef _CSLIB_FILE_H
#define _CSLIB_FILE_H

#include "cslib/types/string.h"
#include "cslib/types/vector.h"
#include "cslib/pub.h"

#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#endif

#include "cslib/pub.h"


typedef enum OsFileType {
    OS_FILE_REGULAR,
    OS_FILE_DIRECTORY
} cslib_os_file_type_t;

typedef struct OsFile {
    char *name;
    cslib_os_file_type_t type;
} cslib_os_file_t;

bool cslib_list_directory(cslib_vector_t *vec, char *path, size_t depth);
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
        "name": "cslib",
        "type": "directory"
    }
]

*/

#endif // _CSLIB_FILE_H