#include "cslib/os/file.h"

bool cslib_list_directory(cslib_vector_t *vec, char *path, ssize_t depth)
{
#if defined(_WIN32) || defined(WIN32)
    // TODO: check if vec size is enough for the amount of files in the directory
    WIN32_FIND_DATA ffd;
    TCHAR dirname[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;

    if (strlen(path) >= MAX_PATH - 3)
    {
        fprintf(stderr, "cslib_list_directory::path length exceeds MAX_PATH (%d)\n", MAX_PATH);
        return false;
    }

    StringCchCopy(dirname, MAX_PATH, path);
    StringCchCat(dirname, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(dirname, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        fprintf(stderr, "cslib_list_directory::FindFirstFile error %ld", GetLastError());
        return false;
    }

    ssize_t rc;

    // List all the files in the directory with some info about them.
    do
    {
        cslib_os_file_t *file = ALLOC_OBJECT(cslib_os_file_t);

        if (strncmp(ffd.cFileName, ".", 2) == 0 ||
            strncmp(ffd.cFileName, "..", 3) == 0)
        {
            continue;
        }

        file->name = cslib_string_create(ffd.cFileName, strlen(ffd.cFileName));

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            file->type = OS_FILE_DIRECTORY;

            if (depth > 0)
            {
                size_t strsize = strlen(ffd.cFileName) + strlen(path) + 4;
                char *str = cslib_string_create("\0", strsize);

                snprintf(str, strsize, "%s\\%s", path, ffd.cFileName);

                cslib_list_directory(vec, str, depth - 1);

                free(str);
            }
        }
        else
        {
            file->type = OS_FILE_REGULAR;
        }

        rc = cslib_vector_insert(vec, file);

        switch (rc)
        {
        case VECTOR_CAPACITY_REACHED:
            fprintf(stderr, "cslib_list_directory::Vector Capacity Reached\n");
            break;
        case VECTOR_UNDEFINED_BEHAVIOR:
            fprintf(stderr, "cslib_list_directory::Vector Undefined Behavior over `%s`\n", file->name);
            break;
        default:
            break;
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES)
    {
        fprintf(stderr, "cslib_list_directory::FindFirstFile error %ld", GetLastError());
    }

    FindClose(hFind);

#elif defined(__linux__) // ABOVE: WIN32

    DIR *dirptr = opendir(path);
    struct dirent *entry;
    struct stat st;
    cslib_os_file_t *file = NULL;

    char path_buffer[255];
    size_t strsize;
    ssize_t rc;

    if (dirptr)
    {
        while ((entry = readdir(dirptr)) != NULL)
        {
            if (!strcmp(entry->d_name, ".") ||
                !strcmp(entry->d_name, ".."))
                continue;

            file = ALLOC_OBJECT(cslib_os_file_t);
            file->name = cslib_string_create(entry->d_name, strlen(entry->d_name));

            printf("%s/%s\n", path, file->name);

            if (fstatat(dirfd(dirptr), entry->d_name, &st, 0) < 0)
            {
                fprintf(stderr, "Error: %s\n", entry->d_name);
                continue;
            }

            if (S_ISDIR(st.st_mode))
            {
                file->type = OS_FILE_DIRECTORY;
            }
            else if (S_ISREG(st.st_mode))
            {
                file->type = OS_FILE_REGULAR;
            }

            if (file->type == OS_FILE_DIRECTORY && depth != 0)
            {
                strsize = strlen(entry->d_name) + strlen(path) + 4;
                snprintf(path_buffer, strsize, "%s/%s", path, entry->d_name);
                cslib_list_directory(vec, path_buffer, depth - 1);
            }

            rc = cslib_vector_insert(vec, file);

            switch (rc)
            {
            case VECTOR_CAPACITY_REACHED:
                fprintf(stderr, "cslib_list_directory::Vector Capacity Reached\n");
                break;
            case VECTOR_UNDEFINED_BEHAVIOR:
                fprintf(stderr, "cslib_list_directory::Vector Undefined Behavior over `%s`\n", file->name);
                break;
            default:
                break;
            }
        }

        closedir(dirptr);
    }
#endif

    return true;
}
