#include "file.h"


bool zrub_io_filesize(const char *filepath, long long *size)
{
    #if defined(WIN32)
    LARGE_INTEGER fsize;
    HANDLE hFile = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        ZRUB_LOG_ERROR("failed to open file `%s`. error-code `%lu`", GetLastError());
    }

    if (!GetFileSizeEx(hFile, &fsize))
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. error-code `%lu`", GetLastError());
        CloseHandle(hFile);

        *size = -1;
        return false;
    }

    CloseHandle(hFile);
    *size = fsize.QuadPart;

    return true;

    #elif defined(__linux__)
    struct stat st;

    if (stat(filepath, &st) != 0)
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. error: `%s`", filepath, strerror(errno));

        *size = -1;
        return false;
    }

    *size = (long long)st.st_size;
    return true;

    ZRUB_LOG_ERROR("not implemented");

    return false;

    #endif
}