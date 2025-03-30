#include "zrub/utils/str.h"


char *zrub_str_as_dyn(char *s)
{
    if (s == NULL)
    {
        ZRUB_LOG_ERROR("string is null\n");
        return NULL;
    }

    int32_t ssize = zrub_str_len(s);
    char *n = ZRUB_CALLOC(ssize + 1, sizeof(char));
    zrub_str_copy(s, n, ssize, ssize);
    n[ssize] = '\0';

    return n;
}

/**
 * @brief removes suffix from a string
 * 
 * @note the dest buffer should be modifable
 * 
 * @param d         the buffer to modify
 * @param s         the suffix buffer
 */
bool zrub_str_delsfx(char *d, char *sfx)
{
    if (d == NULL)
    {
        ZRUB_LOG_ERROR("dest strin is null\n");
        return false;
    }

    if (sfx == NULL)
    {
        ZRUB_LOG_ERROR("suffix string is null\n");
        return false;
    }

    int64_t dlen = zrub_str_len(d);
    int64_t sfxlen = zrub_str_len(sfx);
    ZRUB_LOG_DEBUG("%d %d\n", dlen, sfxlen);

    if (dlen < sfxlen)
    {
        return false;
    }

    if (!zrub_str_compare(d + dlen - sfxlen, sfx, sfxlen))
    {
        ZRUB_LOG_DEBUG("dest `%s` does not contain suffix `%s`\n", d, sfx);
        return false;
    }

    d[dlen - sfxlen] = '\0';
    return true;
}

/**
 * @brief returns the length of the string
 * 
 * @param s         string
 * @return length of string, -1 for null
 */
int32_t zrub_str_len(char *s)
{
    // if string is null
    // > return 0
    if (s == NULL)
    {
        ZRUB_LOG_ERROR("string is null\n");
        return -1;
    }

    // pointer to s
    char *p = s;
    while (*p != '\0')
    {
        p++;
    }

    return (p - s);
}

/**
 * @brief find character index
 * 
 * @param s         string
 * @param c         character
 * @return first occurrence of c in s, returns -1 if not found 
 */
int32_t zrub_str_findc(char *s, char c)
{
    if (s == NULL)
    {
        ZRUB_LOG_ERROR("string is null\n");
        return -1;
    }

    int32_t idx = 0;

    while (*s != '\0')
    {
        if (*s == c) return idx;
        idx++;
        s++;
    }

    return -1;
}

/**
 * @brief character occurrences counter
 * 
 * @param s         string
 * @param c         character
 * @return how many times c occurred in s
 */
int32_t zrub_str_countc(char *s, char c)
{
    if (s == NULL)
    {
        ZRUB_LOG_ERROR("string is null\n");
        return -1;
    }

    int32_t ctr = 0;

    while (*s != '\0')
    {
        if (*s == c) ctr++;
        s++;
    }

    return ctr;
}

/**
 * @brief compares two strings first n characters
 * 
 * @param s1        first string
 * @param s2        second string
 * @param n         how many characters
 * @return true if equals
 */
bool zrub_str_compare(char *s1, char *s2, int32_t n)
{
    if (s1 == NULL)
    {
        ZRUB_LOG_ERROR("s1 is null\n");
        return false;
    }

    if (s2 == NULL)
    {
        ZRUB_LOG_ERROR("s2 is null\n");
        return false;
    }

    while (*s1 != '\0' && *s2 != '\0' && n > 0)
    {
        if (*s1 != *s2)
        {
            return false;
        }

        s1++;
        s2++;
        n--;
    }

    return n == 0;
}

/**
 * @brief copies string
 * 
 * @param src           copy from
 * @param dest          copy to
 * @param n             how many characters to copy
 * @return how many characters were copied
 */
int32_t zrub_str_copy(char *src, char *dest, int32_t dest_size, int32_t n)
{
    if (dest == NULL)
    {
        ZRUB_LOG_ERROR("dest is null\n");
        return -1;
    }

    if (dest_size == 0)
    {
        ZRUB_LOG_ERROR("dest size must be greater than 0\n");
        return -1;
    }

    if (dest_size < n)
    {
        ZRUB_LOG_ERROR("dest size is too small to copy %d chars\n", n);
        return -1;
    }

    for (int32_t i = 0; i < n; i++)
    {
        if (*src == '\0')
        {
            return i;
        }

        *dest++ = *src++;
    }

    return n;
}

/**
 * @brief splits by a character delimiter
 * 
 * @note returns allocated strings
 * @param s         string
 * @param d         delimiter char
 * @param l         list of strings, allocated already
 * @return length of list, -1 if not split
 */
int32_t zrub_str_splitc(char *s, char d, char **l)
{
    if (s == NULL)
    {
        ZRUB_LOG_ERROR("string is null\n");
        return -1;
    }

    if (l == NULL)
    {
        ZRUB_LOG_ERROR("list is null\n");
        return -1;
    }

    int32_t cptr = zrub_str_findc(s, d);       // pointers to character

    // checks if there is a single occurrence of the delimiter
    if (cptr == -1)
    {
        return -1;
    }

    int32_t eos = zrub_str_len(s);     // stands for end-of-string
    int32_t offset = 0;                // offset relative to last part

    char *tmp = NULL;
    int32_t idx = 0;

    while (cptr < eos)
    {
        // finds the index of the delimiter relative to the offset
        cptr = zrub_str_findc(s + offset, d);

        // there are no more occurrences of the delimiter
        if (cptr == -1)
        {
            break;
        }

        // case when the string starts with the delimtier
        if (cptr == 0 && offset == 0)
        {
            tmp = (char*)ZRUB_MALLOC(2 * sizeof(char));
            if (tmp == NULL)
            {
                ZRUB_LOG_ERROR("failed to allocate string\n");
                return -1;
            }

            zrub_str_copy(" \0", tmp, 2, 2);

            ZRUB_LOG_DEBUG("%s\n", tmp);
            l[idx++] = tmp;
        }
        else
        {
            tmp = (char*)ZRUB_MALLOC((cptr + 2) * sizeof(char));
            if (tmp == NULL)
            {
                ZRUB_LOG_ERROR("failed to allocate string\n");
                return -1;
            }
            
            zrub_str_copy(s + offset, tmp, cptr + 2, cptr);
            tmp[cptr ] = '\0';

            ZRUB_LOG_DEBUG("%s\n", tmp);
            l[idx++] = tmp;
        }
            
        offset += cptr + 1;
    }

    /* handlers the remainder */
    if (cptr != eos)
    {
        tmp = (char*)ZRUB_MALLOC((eos - offset + 1) * sizeof(char));
        if (tmp == NULL)
        {
            ZRUB_LOG_ERROR("failed to allocate string\n");
            return -1;
        }

        zrub_str_copy(s + offset, tmp, eos - offset + 1, eos - offset);        
        tmp[eos - offset] = '\0';

        ZRUB_LOG_DEBUG("%s\n", tmp);
        l[idx++] = tmp;

    }

    return idx;
}
