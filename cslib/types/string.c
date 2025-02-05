#include "cslib/types/string.h"
#include <stdio.h>


/*
This function removes the prefix (beginning of the string) if the prefix is in it.
Make sure to pass a modifiable string! char *literal = ".." does not count.
Returns true if removed the prefix, else returns false.

NOTE: To just check if the prefix exists use the cslib_isprefix function.
*/
bool cslib_string_remove_prefix(char *str, const char *prefix)
{
    if (str == NULL || prefix == NULL)
    {
        return false;
    }

    // checks if the string has that prefix
    if (!cslib_string_check_prefix(str, prefix))
    {
        return false;
    }

    const size_t psize = strlen(prefix);
    const size_t ssize = strlen(str);

    // removes that prefix
    memmove(str, str + psize, ssize - psize + 1);

    return true;
}

/*
Checks if it is the prefix of string

Returns true if yes, else false.
*/
bool cslib_string_check_prefix(const char *str, const char *prefix)
{
    if (str == NULL || prefix == NULL)
    {
        return false;
    }

    return memcmp(prefix, str, strlen(prefix)) == 0;
}

bool cslib_string_remove_suffix(char *str, const char *suffix)
{
    if (str == NULL || suffix == NULL)
    {
        return false;
    }

    if (!cslib_string_check_suffix(str, suffix))
    {
        return false;
    }

    memset(str + strlen(str) - strlen(suffix), '\0', 1);

    return true;
}

bool cslib_string_check_suffix(const char *str, const char *suffix)
{
    if (str == NULL || suffix == NULL)
    {
        return false;
    }

    const size_t str_length = strlen(str);
    const size_t suf_length = strlen(suffix);

    return memcmp(suffix, str + str_length - suf_length, suf_length) == 0;
}

/*
Allocates memory for a string from a literal
*/
char *cslib_string_create(const char *data, size_t min_size)
{
    if (data == NULL)
    {
        return NULL;
    }

    const size_t datasize = strlen(data);
    const size_t alloc_size = min_size > datasize ? min_size : datasize;

    // +1 for the null terminator
    char *s = (char *)CSLIB_MALLOC(sizeof(char) * alloc_size + 1);
    strncpy(s, data, datasize + 1);

    return s;
}

char *cslib_string_slice(const char *str, size_t begin, size_t end)
{
    /*
    cslib_string_slice("for all the years", 5, 7);

    would produce "all".
    */
    if (str == NULL)
    {
        return NULL;
    }

    const size_t length = strlen(str);

    if (length < end ||
        length < begin ||
        begin >= end)
    {
        return NULL;
    }

    char *new_string = (char *)CSLIB_MALLOC(sizeof(char) * (end - begin) + 1);
    strncpy(new_string, str + begin, end - begin);
    memset(new_string + end - begin, '\0', 1);

    return new_string;
}

void cslib_string_slice2(char *str, const char *data, size_t begin, size_t end)
{
    if (str == NULL || data == NULL)
    {
        return NULL;
    }

    /*
    cslib_string_slice(slice, "for all the years", 5, 7);

    would produce "all".

    keep in mind it may be out of bounds, the responsibility of
    ensuring the str can hold the slice is on the caller.
    */
    const size_t length = strlen(data);

    if (length < end ||
        length < begin ||
        begin >= end)
    {
        return;
    }

    // *str = '\0';
    strncpy(str, data + begin, end - begin);
    memset(str + end - begin, '\0', 1);
}

size_t cslib_string_count_substring(const char *str, const char *sub)
{
    if (str == NULL || sub == NULL)
    {
        return NULL;
    }

    const size_t str_length = strlen(str);
    const size_t sub_length = strlen(sub);
    size_t counter = 0;

    /* example case:
    str = "hello", sub = "ello"

    so 5 - 4 + 1 cases to check ("hell", "ello")
    */

    char *tmp = (char *)CSLIB_MALLOC(sizeof(char) * (sub_length + 1));

    for (size_t i = 0; i <= str_length - sub_length; i++)
    {
        cslib_string_slice2(tmp, str, i, i + sub_length);

        if (strncmp(tmp, sub, sub_length) == 0)
        {
            counter++;
        }
    }

    free(tmp);

    return counter;
}

char *cslib_string_leftpad(const char *str, const char *pad, size_t count)
{
    /* exmaple case:
    NOTE: this function delimiter is the newline character!

    str = "hello \nworld", pad = '*', count = 2

    expected output: str = "**hello \n**world";
    */
    if (str == NULL || pad == NULL || count <= 0)
    {
        return NULL;
    }

    cslib_vector_t *lines = cslib_string_split(str, "\n");
    const size_t str_length = strlen(str);

    size_t buffersize = str_length + sizeof(char) * count * lines->length + 1;
    char *padded = (char *)CSLIB_MALLOC(buffersize);

    if (padded == NULL)
    {
        fprintf(stderr, "failed to CSLIB_MALLOC string\n");
        exit(1);
    }

    char *item = NULL;
    size_t offset = 0;

    for (size_t i = 0; i < lines->length; i++)
    {
        item = lines->items[i];

        if (item != NULL)
        {
            memset(padded + offset, pad[0], count);
            offset += count;

            snprintf(padded + offset, buffersize - offset, "%s", item);

            offset += strlen(item);

            memset(padded + offset, '\n', 1);
            offset += 1;
        }
    }

    memset(padded + offset, '\0', 1);

    cslib_vector_naivefree(lines);
    cslib_vector_dumbfree(lines);

    return padded;
}

cslib_vector_t *cslib_string_split(const char *str, const char *delimiter)
{
    if (str == NULL || delimiter == NULL)
    {
        return NULL;
    }

    const size_t str_length = strlen(str);
    const size_t sub_length = strlen(delimiter);
    size_t offset = 0;
    size_t counter = 0;

    bool delim_is_suffix = false;

    if (str_length == 0)
    {
        return NULL;
    }

    cslib_vector_t *vec = (cslib_vector_t *)CSLIB_MALLOC(sizeof(cslib_vector_t));

    if (str_length <= sub_length)
    {
        cslib_allocate_vector(vec, 0);

        return vec;
    }

    size_t occurences = cslib_string_count_substring(str, delimiter);
    // printf("found %zu occurences of the delimiter `%s`\n", occurences, delimiter);

    /* checks if str ends with delim */
    char *end = cslib_string_slice(str, str_length - sub_length, str_length);
    char *start = cslib_string_slice(str, 0, sub_length);

    if (cslib_string_check_prefix(str, delimiter))
    {
        occurences--;
    }

    if (cslib_string_check_suffix(str, delimiter))
    {
        delim_is_suffix = true;
        occurences--;
    }

    cslib_allocate_vector(vec, occurences + 1);

    free(start);
    free(end);

    if (occurences == 0)
    {
        char *sub = cslib_string_create(str, str_length);
        cslib_vector_insert(vec, sub);
    }
    else
    {

        /* example case:
        str = "hello, the world is the nice thing", delimiter = "the"
        expected output: ["hello, ", " world is ", " nice thing"]

        so 5 - 4 + 1 cases to check ("hell", "ello")
        */

        // char *tmp = (char *)CSLIB_MALLOC(sizeof(char) * (sub_length + 1));
        char *tmp = cslib_string_create(delimiter, sub_length + 1);

        for (size_t i = 0; i <= str_length - sub_length; i++)
        {
            cslib_string_slice2(tmp, str, i, i + sub_length);
            // printf("%zu %zu %zu %s\n", counter, offset, i, tmp);

            if (strncmp(tmp, delimiter, sub_length) == 0 &&
                offset != i && i != 0)
            {
                char *sub = cslib_string_slice(str, offset, i);
                // printf("pushing `%s`\n", sub);

                cslib_vector_insert(vec, sub);

                offset = i + sub_length;
                i += sub_length - 1;
                counter++;

                if (counter == occurences)
                {
                    // printf("offset: %zu, str_length: %zu\n", offset, str_length);
                    char *final = cslib_string_slice(
                        str,
                        offset,
                        delim_is_suffix == true ? str_length - sub_length : str_length);

                    if (final == NULL)
                    {
                    }
                    else
                    {
                        // printf("pushing `%s`\n", final);
                        cslib_vector_insert(vec, final);
                    }

                    break;
                }
            }
        }

        free(tmp);
    }

    return vec;
}

void cslib_string_lowercase(char *str)
{
    if (str == NULL) return;

    for (; *str; ++str) *str = tolower(*str); 
}

void cslib_string_uppercase(char *str)
{
    if (str == NULL) return;

    for (; *str; ++str) *str = toupper(*str);
}
