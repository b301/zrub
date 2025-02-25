#include "str.h"


/**
 * @brief removes prefix from a string
 * 
 * @note the dest buffer should be modifable
 * 
 * @param dest          the buffer to modify
 * @param prefix        the prefix buffer
 */
void zrub_str_rm_prefix(char *dest, const char *prefix)
{
    if (dest == NULL || prefix == NULL)
    {
        ZRUB_LOG_ERROR("received null strings as parameters");
        return;
    }

    size_t destlen = strlen(dest);
    size_t prefixlen = strlen(prefix);

    if (destlen < prefixlen)
    {
        return;
    }

    if (strncmp(dest + destlen - prefixlen, prefix, prefixlen) != 0)
    {
        ZRUB_LOG_DEBUG("dest `%s` does not contain prefix `%s`", dest, prefix);
        return;
    }

    dest[destlen - prefixlen] = '\0';
}