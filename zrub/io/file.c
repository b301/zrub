#include "file.h"


/**
 * @brief gets the size of a directory in bytes
 * 
 * @details this function sums the file size of all the files in the directory 
 * @todo implement this function
 * 
 * @param dirpath       path of the directory
 * @param size          variable to set size to
 * @returns true if retrieved the directory size
 */
bool zrub_io_dir_size(const char *dirpath, i64 *size)
{
    ZRUB_LOG_ERROR("not implemented\n");

    (void)dirpath;
    (void)size;

    return false;
}


/**
 * @brief gets the size of a file in bytes
 * 
 * @param filepath      path of the file
 * @param size          pointer to assign the size to
 * @returns true if retrieved the file size
 */
bool zrub_io_file_size(const char *filepath, i64 *size)
{
    // TODO: work on win32
    #if defined(WIN32)
    LARGE_INTEGER fsize;
    HANDLE hFile = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD lastError;

    if (hFile == INVALID_HANDLE_VALUE)
    {
        lastError = GetLastError();
        
        switch (lastError)
        {
            case ERROR_FILE_NOT_FOUND | ERROR_PATH_NOT_FOUND:
                ZRUB_LOG_ERROR("%s not found.\n", filepath);
                break;

            default:
                ZRUB_LOG_ERROR("failed to open file `%s`. win32-error-code `%lu`\n", filepath, lastError);
                break;
        }

        return false;
    }

    ZRUB_LOG_DEBUG("here");
    if (!GetFileSizeEx(hFile, &fsize))
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. win32-error-code `%lu`\n", filepath, GetLastError());
        CloseHandle(hFile);

        *size = -1;
        return false;
    }

    DWORD attributes = GetFileAttributesA(filepath);
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        ZRUB_LOG_WARNING("failed to get attributes for `%s`. win32-error-code `%lu`\n", filepath, GetLastError());
    }

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        ZRUB_LOG_INFO("%s is a directory\n", filepath);
    }

    CloseHandle(hFile);
    *size = fsize.QuadPart;

    return true;

    #elif defined(__linux__)
    struct stat st;

    // if file does not exist
    if (access(filepath, F_OK) != 0)
    {
        ZRUB_LOG_ERROR("no such file %s\n", filepath);

        *size = -1;
        return false;
    }

    if (stat(filepath, &st) != 0)
    {
        ZRUB_LOG_ERROR("failed to retrieve size of `%s`. error: `%s`\n", filepath, strerror(errno));
        *size = -1;
        return false;
    }

    if (S_ISDIR(st.st_mode))
    {
        ZRUB_LOG_INFO("%s is a directory\n", filepath);
    }

    *size = (i64)st.st_size;
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
    #if defined(WIN32)
    ZRUB_LOG_INFO("checking if %s exists\n", filepath);
    ZRUB_LOG_ERROR("not implemented\n");

    return false;

    #elif defined(__linux__)
    if (access(filepath, F_OK) == 0)
    {
        return true;
    }

    switch (errno)
    {
        case EACCES:
            ZRUB_LOG_ERROR("no read permission on `%s`\n", filepath);
            break;
        
        case ENOENT:
            ZRUB_LOG_INFO("no such file `%s`\n", filepath);
            break;
        
        case ENOTDIR:
            ZRUB_LOG_ERROR("a component of the path prefix is not a directory `%s`\n", filepath);
            break;
        
        case ENAMETOOLONG:
            ZRUB_LOG_ERROR("path or pathname is too long `%s`\n", filepath);
            break;

        default:
            break;
    }

    return false;

    #endif
}
