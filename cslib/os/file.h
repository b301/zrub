#ifndef _CSLIB_GEN_H
#define _CSLIB_GEN_H

#include "cslib/std/string.h"
#include "cslib/std/vector.h"
#include "cslib/pub.h"

#include <stdio.h>

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#endif

typedef enum OsFileType {
    OS_FILE_REGULAR,
    OS_FILE_DIRECTORY
} cslib_os_file_type_t;

typedef struct OsFile {
    char *name;
    cslib_os_file_type_t type;
} cslib_os_file_t;

void cslib_list_directory(cslib_vector_t *vec, char *path, size_t depth);
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

#endif // _CSLIB_GEN_H