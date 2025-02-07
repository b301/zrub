#include "zrub/os/file.h"
#include "zrub/types/iterator.h"


int main(int argc, char **argv)
{
    {
        char *path = argv[1];

        zrub_vector_t *vec = ALLOC_OBJECT(zrub_vector_t);
        zrub_allocate_vector(vec, 256);

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

    return 0;
}