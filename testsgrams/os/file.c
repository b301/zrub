#include "cslib/os/file.h"
#include "cslib/types/iterator.h"

int main(int argc, char **argv)
{
    {
        char *path = argv[1];

        cslib_vector_t *vec = ALLOC_OBJECT(cslib_vector_t);
        cslib_allocate_vector(vec, 256);

        cslib_list_directory(vec, path, -1);

        cslib_iterator_t iterator;
        cslib_vector_iterator(&iterator, vec);

        cslib_os_file_t *file = NULL;

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
        cslib_vector_dumbfree(vec);
    }

    return 0;
}