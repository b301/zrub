#include "zrub/utils/str.h"


/**
 * @brief removes suffix from a string
 * 
 * @note the dest buffer should be modifable
 * 
 * @param dest          the buffer to modify
 * @param suffix        the suffix buffer
 */
void zrub_str_rm_suffix(char *dest, const char *suffix)
{
    if (dest == NULL || suffix == NULL)
    {
        ZRUB_LOG_ERROR("received null strings as parameters\n");
        return;
    }

    i64 destlen = strlen(dest);
    i64 suffixlen = strlen(suffix);

    if (destlen < suffixlen)
    {
        return;
    }

    if (strncmp(dest + destlen - suffixlen, suffix, suffixlen) != 0)
    {
        ZRUB_LOG_DEBUG("dest `%s` does not contain suffix `%s`\n", dest, suffix);
        return;
    }

    dest[destlen - suffixlen] = '\0';
}
