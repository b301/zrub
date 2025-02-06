#include "zrub/types/string.h"
#include "zrub/pub.h"

#include <stdio.h>


int main()
{
    printf("testing string\n");

    /* test 1: bool zrub_remove_prefix(char *str, const char *prefix) */
    {
        char *string = zrub_string_create("Hello There!", 128);
        char *prefix = zrub_string_create("Hell", 128);

        bool remove_prefix = zrub_string_remove_prefix(string, prefix);
        CHECK(remove_prefix, "test 1: checking if prefix was correctly removed");

        free(string);
        free(prefix);
    }

    /* test 2: char* zrub_string_slice(const char *str, size_t begin, size_t end) */
    {
        char *origin = "Hello, My Name is Oded Zrubavel";
        char *name = zrub_string_slice(origin, 18, 31);

        CHECK(
            strcmp(name, "Oded Zrubavel") == 0, 
            "test 2: checking the slice returned the correct string"
        );

        free(name);
    }

    /* test 3: size_t zrub_string_count_substring(const char *str, const char *sub) */
    {
        char *data = "Hello there I am repeating the word there at least three times there we go!";
        size_t count = zrub_string_count_substring(data, "there");

        CHECK(count == 3, "test 3: ensuring the substring count is correct");
    }

    /* test 4: zrub_vector_t *zrub_string_split(const char *str, const char *delimiter) */
    {
        char *data = "the world is round and the world is spherical and the world sucks. the ears of giants.";
        char *delimiter = "spherical";

        zrub_vector_t *vec = zrub_string_split(data, delimiter);
        CHECK(vec != NULL, "test 4: ensuring the string split function works");
        // printf("vec->capacity: %zu\n", vec->capacity);

        /* debug */
        // for (size_t i = 0; i < vec->length; i++)
        // {
        //     printf("(%zu) `%s`\n", i, vec->items[i]);
        // }

        CHECK(strncmp(vec->items[0], "the world is round and the world is ", 36) == 0, 
            "test 4: checking string first half");
        CHECK(strncmp(vec->items[1], " and the world sucks. the ears of giants.", 36) == 0, 
            "test 4: checking string second half");

        zrub_vector_naivefree(vec);
        zrub_vector_dumbfree(vec);
    }

    /* test 5: void zrub_string_leftpad(char *str, const char pad, size_t count) */
    {
        char *data = zrub_string_create("hello\nworld\n", 16);
        char *pad = "*";

        char *padded = zrub_string_leftpad(data, pad, 4);
        char *expected = "****hello\n****world\n";

        CHECK(strncmp(padded, expected, strlen(expected)) == 0, "test 5: checking the padding is correct");

        free(data);
        free(padded);
    }

    /* test 6: lower and upper */
    {
        char *lowerit = zrub_string_create("My Name is Oded", 32);
        char *upperit = zrub_string_create("mY nAME iS oDED", 32);

        zrub_string_lowercase(lowerit);

        printf("lower failed\n");
        zrub_string_uppercase(upperit);
        
        printf("upper failed\n");
        CHECK(strncmp(lowerit, "my name is oded", strlen(lowerit)) == 0, "test 6: checking string lower");
        CHECK(strncmp(upperit, "MY NAME IS ODED", strlen(upperit)) == 0, "test 6: checking string upper");

        free(lowerit);
        free(upperit);
    }


    return 0;
}
