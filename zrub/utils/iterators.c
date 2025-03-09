#include "iterators.h"


/**
 * @brief beginning of the iterator
 * 
 * @param iter          the iterator to begin
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_byte_iter_begin(struct zrub_byte_iter *iter, u8 *buf, u32 *buflen, i32 blocksize)
{
    iter->ptr = 0;

    return zrub_byte_iter_next(iter, buf, buflen, blocksize);
}

/**
 * @brief get next iteration
 * 
 * @param iter          the iterator to iterate
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_byte_iter_next(struct zrub_byte_iter *iter, u8 *buf, u32 *buflen, i32 blocksize)
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
bool zrub_byte_iter_end(const struct zrub_byte_iter iter)
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
u32 zrub_byte_iter_size(const struct zrub_byte_iter iter, i32 blocksize)
{
    u32 len = iter.usage / blocksize;

    if (iter.usage - blocksize * len > 0)
    {
        len++;
    }

    return len;
}
