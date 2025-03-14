#include "zrub/core/bytes.h"


bool zrub_randombytes(u8 *bytes, u32 size)
{
    if (getrandom(bytes, size, 0) == -1)
    {
        ZRUB_LOG_ERROR("failed to generate random bytes\n");
        return false;
    }

    return true;
}

bool zrub_bytes_as_hex(const u8 *bytes, const u32 bsize, char *hexstr, const u32 dsize)
{
    if (dsize < bsize * 2 + 1)
    {
        ZRUB_LOG_ERROR("hex string is too small to contain the bytes array, \
            (%llu < %llu)\n", dsize, bsize * 2 +1);
        return false;
    }

    for (u32 i = 0; i < bsize; i++)
    {
        snprintf(hexstr + 2 * i, 3, "%02x", bytes[i]);
    }

    return true;
}

/**
 * @brief beginning of the iterator
 * 
 * @param iter          the iterator to begin
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_bytes_iter_begin(struct zrub_bytes_iter *iter, u8 *buf, u32 *buflen, i32 blocksize)
{
    iter->ptr = 0;

    return zrub_bytes_iter_next(iter, buf, buflen, blocksize);
}

/**
 * @brief get next iteration
 * 
 * @param iter          the iterator to iterate
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_bytes_iter_next(struct zrub_bytes_iter *iter, u8 *buf, u32 *buflen, i32 blocksize)
{
    if (iter->usage == iter->ptr)
    {
        iter->ptr = 0;
    }
    
    i32 bufsize;
    
    // if there are 1-block or more left
    if (iter->usage - iter->ptr >= blocksize)
    {
        bufsize = blocksize;                    // set bufsize to blocksize
    }
    // if less than 1-block is left  
    else     
    {
        bufsize = iter->usage - iter->ptr;      // set bufsize to remainder
    }
    
    ZRUB_LOG_DEBUG("retrieveing %ld bytes (%ld/%ld)\n", bufsize, iter->ptr, iter->usage);
    memcpy(buf, iter->data + iter->ptr, bufsize);
    
    iter->ptr += bufsize;                   // add remainder to ptr
    *buflen = bufsize;

    return true;
}

/**
 * @brief check if reached end-of-iterator
 * 
 * @param iter          the iterator to check
 */
bool zrub_bytes_iter_end(const struct zrub_bytes_iter iter)
{
    return iter.usage == iter.ptr;
}

/**
 * @brief get the iterator size
 * 
 * @param iter          the iterator whose size is checked
 * @param blocksize     the size of each block
 * @returns size of iterator
 */
u32 zrub_bytes_iter_size(const struct zrub_bytes_iter iter, i32 blocksize)
{
    u32 len = iter.usage / blocksize;

    if (iter.usage - blocksize * len > 0)
    {
        len++;
    }

    return len;
}
