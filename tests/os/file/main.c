#include "zrub/os/file.h"
#include "zrub/types/iterator.h"


static void test_1(char *path, size_t no_files)
{
    zrub_vector_t *vec = ALLOC_OBJECT(zrub_vector_t);
    zrub_allocate_vector(vec, no_files);

    zrub_list_directory(vec, path, -1);

    zrub_iterator_t iterator;
    zrub_vector_iterator(&iterator, vec);

    zrub_os_file_t *file = NULL;

    ZRUB_LOG_CHECK("iterator length: %lld", iterator.length);
    for (size_t i = 0; i < iterator.length; i++)
    {
        file = iterator.items[i];

        if (file->type == ZRUB_OS_FILE_DIRECTORY)
        {
            ZRUB_LOG_CHECK("___dirt___ %s", file->name);
        }

        if (file->type == ZRUB_OS_FILE_REGULAR)
        {
            ZRUB_LOG_CHECK("___file___ %s", file->name);
        }
    }

    // Free all used memory
    for (size_t i = 0; i < iterator.length; i++)
    {
        file = iterator.items[i];

        free(file->name);
        free(file);
    }

    free(iterator.items);
    zrub_vector_dumbfree(vec);
}


int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        ZRUB_LOG_INFO("%s <dirpath:char*> <nofiles:size_t>", __FILE__);
        return 0;
    }

    test_1(argv[1], (size_t)(atoi(argv[2])));


    return 0;
}