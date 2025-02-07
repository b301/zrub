#include "zrub/os/file.h"

bool zrub_list_directory(zrub_vector_t *vec, char *path, ssize_t depth)
{
#if defined(_WIN32) || defined(WIN32)
    // TODO: check if vec size is enough for the amount of files in the directory
    WIN32_FIND_DATA ffd;
    TCHAR dirname[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;

    if (strlen(path) >= MAX_PATH - 3)
    {
        ZRUB_LOG_ERROR("path length exceeds MAX_PATH (%d)", MAX_PATH);
        return false;
    }

    StringCchCopy(dirname, MAX_PATH, path);
    StringCchCat(dirname, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(dirname, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        ZRUB_LOG_ERROR("the function FindFirstFile(...) returned error code %ld", GetLastError());
        return false;
    }

    ssize_t rc;

    // List all the files in the directory with some info about them.
    do
    {
        zrub_os_file_t *file = ALLOC_OBJECT(zrub_os_file_t);

        if (strncmp(ffd.cFileName, ".", 2) == 0 ||
            strncmp(ffd.cFileName, "..", 3) == 0)
        {
            continue;
        }

        file->name = zrub_string_create(ffd.cFileName, strlen(ffd.cFileName));

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            file->type = ZRUB_OS_FILE_DIRECTORY;

            if (depth > 0)
            {
                size_t strsize = strlen(ffd.cFileName) + strlen(path) + 4;
                char *str = zrub_string_create("\0", strsize);

                snprintf(str, strsize, "%s\\%s", path, ffd.cFileName);

                zrub_list_directory(vec, str, depth - 1);

                free(str);
            }
        }
        else
        {
            file->type = ZRUB_OS_FILE_REGULAR;
        }

        rc = zrub_vector_insert(vec, file);

        switch (rc)
        {
            case ZRUB_VECTOR_CAPACITY_REACHED:
                ZRUB_LOG_ERROR("vector capacity reached");
                break;

            case ZRUB_VECTOR_UNDEFINED_BEHAVIOR:
                ZRUB_LOG_ERROR("vector undefined behaviro over '%s'", file->name);
                break;

            default:
                break;
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES)
    {
        ZRUB_LOG_ERROR("the function FindFirstFile(...) returned error code %ld", GetLastError());
    }

    FindClose(hFind);

#elif defined(__linux__) // ABOVE: WIN32

    DIR *dirptr = opendir(path);
    struct dirent *entry;
    struct stat st;
    zrub_os_file_t *file = NULL;

    char path_buffer[255];
    size_t strsize;
    ssize_t rc;

    if (dirptr)
    {
        while ((entry = readdir(dirptr)) != NULL)
        {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            {
                continue;
            }

            file = ALLOC_OBJECT(zrub_os_file_t);
            file->name = zrub_string_create(entry->d_name, strlen(entry->d_name));

            ZRUB_LOG_DEBUG("%s/%s", path, file->name);

            if (fstatat(dirfd(dirptr), entry->d_name, &st, 0) < 0)
            {
                ZRUB_LOG_ERROR("failed to get file (%s) stat relative to directory file descriptor (%s)", entry->d_name, path);
                continue;
            }

            if (S_ISDIR(st.st_mode))
            {
                file->type = ZRUB_OS_FILE_DIRECTORY;
            }
            else if (S_ISREG(st.st_mode))
            {
                file->type = ZRUB_OS_FILE_REGULAR;
            }

            if (file->type == ZRUB_OS_FILE_DIRECTORY && depth != 0)
            {
                strsize = strlen(entry->d_name) + strlen(path) + 4;
                snprintf(path_buffer, strsize, "%s/%s", path, entry->d_name);
                zrub_list_directory(vec, path_buffer, depth - 1);
            }

            rc = zrub_vector_insert(vec, file);

            switch (rc)
            {
                case ZRUB_VECTOR_CAPACITY_REACHED:
                    ZRUB_LOG_ERROR("vector capacity reached");
                    break;

                case ZRUB_VECTOR_UNDEFINED_BEHAVIOR:
                    ZRUB_LOG_ERROR("vector undefined behaviro over '%s'", file->name);
                    break;

                default:
                    break;
            }
        }

        closedir(dirptr);
    }
#endif

    ZRUB_LOG_DEBUG("zrub_list_directory executed successfully");
    return true;
}
