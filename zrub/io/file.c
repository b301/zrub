#include "file.h"


/**
 * @brief gets the filesize in bytes
 * 
 * @param filepath      path of the file
 * @param size          pointer to assign the size to
 * @returns true if retrieved file size
 */
bool zrub_io_file_size(const char *filepath, long long *size)
{
    #if defined(WIN32)
    LARGE_INTEGER fsize;
    HANDLE hFile = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        ZRUB_LOG_ERROR("failed to open file `%s`. win32-error-code `%lu`", GetLastError());
    }

    if (!GetFileSizeEx(hFile, &fsize))
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. win32-error-code `%lu`", GetLastError());
        CloseHandle(hFile);

        *size = -1;
        return false;
    }

    // impl this for win32
    if (S_ISDIR(st.st_mode))
    {
        ZRUB_LOG_INFO("%s is a directory", filepath);
    }

    CloseHandle(hFile);
    *size = fsize.QuadPart;

    return true;

    #elif defined(__linux__)
    struct stat st;

    // if file does not exist
    if (access(filepath, F_OK) != 0)
    {
        ZRUB_LOG_ERROR("no such file %s", filepath);

        *size = -1;
        return false;
    }

    if (stat(filepath, &st) != 0)
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. error: `%s`", filepath, strerror_s(errno));

        *size = -1;
        return false;
    }

    if (S_ISDIR(st.st_mode))
    {
        ZRUB_LOG_INFO("%s is a directory", filepath);
    }

    *size = (long long)st.st_size;
    return true;

    #endif
}

/**
 * @brief checks whether filepath exists or not
 * 
 * @param filepath      path of the file
 * @returns true if exists
 */
bool zrub_io_file_exists(const char *filepath)
{
    if (access(filepath, F_OK) == 0)
    {
        return true;
    }

    switch (errno)
    {
        case EACCES:
            ZRUB_LOG_ERROR("no read permission on `%s`", filepath);
            break;
        
        case ENOENT:
            ZRUB_LOG_INFO("no such file `%s`", filepath);
            break;
        
        case ENOTDIR:
            ZRUB_LOG_ERROR("a component of the path prefix is not a directory `%s`", filepath);
            break;
        
        case ENAMETOOLONG:
            ZRUB_LOG_ERROR("path or pathname is too long `%s`", filepath);
            break;

        default:
            break;
    }

    return false;
}
