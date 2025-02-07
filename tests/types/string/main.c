#include "zrub/types/string.h"
#include "zrub/pub.h"

#include <stdio.h>

static void test_1(void)
{
    char *string = zrub_string_create("Hello There!", 128);
    char *prefix = zrub_string_create("Hell", 128);

    bool remove_prefix = zrub_string_remove_prefix(string, prefix);
    TESTS_CHECK(remove_prefix, "checking if prefix was correctly removed");

    free(string);
    free(prefix);
}

static void test_2(void)
{
    char *origin = "Hello, My Name is Oded Zrubavel";
    char *name = zrub_string_slice(origin, 18, 31);

    TESTS_CHECK(
        strcmp(name, "Oded Zrubavel") == 0, 
        "checking the slice returned the correct string"
    );

    free(name);
}

static void test_3(void)
{
    char *data = "Hello there I am repeating the word there at least three times there we go!";
    size_t count = zrub_string_count_substring(data, "there");

    TESTS_CHECK(count == 3, "ensuring the substring count is correct");
}

static void test_4(void)
{
    char *data = "the world is round and the world is spherical and the world sucks. the ears of giants.";
    char *delimiter = "spherical";

    zrub_vector_t *vec = zrub_string_split(data, delimiter);
    TESTS_CHECK(vec != NULL, "ensuring the string split function works");
    // printf("vec->capacity: %zu\n", vec->capacity);

    /* debug */
    // for (size_t i = 0; i < vec->length; i++)
    // {
    //     printf("(%zu) `%s`\n", i, vec->items[i]);
    // }

    TESTS_CHECK(strncmp(vec->items[0], "the world is round and the world is ", 36) == 0, 
        "checking string first half");
    TESTS_CHECK(strncmp(vec->items[1], " and the world sucks. the ears of giants.", 36) == 0, 
        "checking string second half");

    zrub_vector_naivefree(vec);
    zrub_vector_dumbfree(vec);
}

static void test_5(void)
{
    char *data = zrub_string_create("hello\nworld\n", 16);
    char *pad = "*";

    char *padded = zrub_string_leftpad(data, pad, 4);
    char *expected = "****hello\n****world\n";

    TESTS_CHECK(strncmp(padded, expected, strlen(expected)) == 0, "checking the padding is correct");

    free(data);
    free(padded);
}

static void test_6(void)
{
    char *lowerit = zrub_string_create("My Name is Oded", 32);
    char *upperit = zrub_string_create("mY nAME iS oDED", 32);

    zrub_string_lowercase(lowerit);

    printf("lower failed\n");
    zrub_string_uppercase(upperit);
    
    printf("upper failed\n");
    TESTS_CHECK(strncmp(lowerit, "my name is oded", strlen(lowerit)) == 0, "checking string lower");
    TESTS_CHECK(strncmp(upperit, "MY NAME IS ODED", strlen(upperit)) == 0, "checking string upper");

    free(lowerit);
    free(upperit);
}


int main()
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();


    return 0;
}
