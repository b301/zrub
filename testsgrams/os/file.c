#include "zrublib/os/file.h"
#include "zrublib/types/iterator.h"

int main(int argc, char **argv)
{
    {
        char *path = argv[1];

        zrublib_vector_t *vec = ALLOC_OBJECT(zrublib_vector_t);
        zrublib_allocate_vector(vec, 256);

        zrublib_list_directory(vec, path, -1);

        zrublib_iterator_t iterator;
        zrublib_vector_iterator(&iterator, vec);

        zrublib_os_file_t *file = NULL;

        printf("iterator length: %lld\n", iterator.length);
        for (size_t i = 0; i < iterator.length; i++)
        {
            file = iterator.items[i];

            if (file->type == OS_FILE_DIRECTORY)
            {
                printf("___dirt___ %s\n", file->name);
            }

            if (file->type == OS_FILE_REGULAR)
            {
                printf("___file___ %s\n", file->name);
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
        zrublib_vector_dumbfree(vec);
    }

    return 0;
}