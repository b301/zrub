#include "cslib/os/file.h"


void cslib_list_directory(cslib_vector_t *vec, char *path, size_t depth)
{
    // TODO: IMPL LINUX
    #if defined(_WIN32) || defined(WIN32)
    // TODO: check if vec size is enough for the amount of files in the directory
    WIN32_FIND_DATA ffd;
    TCHAR dirname[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dword_error = 0;

    if (strlen(path) - 3 >= MAX_PATH)
    {
        fprintf(stderr, "cslib_list_directory::path length cannot be larger than %d\n", MAX_PATH);
        return;
    }

    StringCchCopy(dirname, MAX_PATH, path);
    StringCchCat(dirname, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.
    hFind = FindFirstFile(dirname, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        fprintf(stderr, "cslib_list_directory::FindFirstFile error %ld", dword_error);
        return;
    }

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
                char *str = cslib_string_create("\0", strlen(ffd.cFileName) + strlen(path) + 4);

                size_t strsize = strlen(ffd.cFileName) + strlen(path) + 4;
                snprintf(str, strsize, "%s\\%s", path, ffd.cFileName);

                cslib_list_directory(vec, str, depth - 1);

                free(str);
            }
        }
        else
        {
            file->type = OS_FILE_REGULAR;
        }

        cslib_vector_push(vec, file);
    }
    while (FindNextFile(hFind, &ffd) != 0);

    dword_error = GetLastError();
    if (dword_error != ERROR_NO_MORE_FILES)
    {
        fprintf(stderr, "cslib_list_directory::FindFirstFile error %ld", dword_error);
    }

    FindClose(hFind);

    #endif // WIN32
}