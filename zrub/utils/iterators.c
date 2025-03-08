#include "iterators.h"


bool zrub_byte_iter_begin(struct zrub_byte_iter *iter, u8 *buf, i32 blocksize)
{
    iter->ptr = 0;

    return zrub_byte_iter_next(iter, buf, blocksize);
}

bool zrub_byte_iter_next(struct zrub_byte_iter *iter, u8 *buf, i32 blocksize)
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
        iter->ptr += bufsize;                   // add blocksize to ptr 
    }
    // if less than 1-block is left  
    else     
    {
        bufsize = iter->usage - iter->ptr;      // set bufsize to remainder
        iter->ptr += bufsize;                   // add remainder to ptr
    }
    
    ZRUB_LOG_DEBUG("retrieveing %ld bytes (%ld/%ld)\n", bufsize, iter->ptr, iter->usage);
    memcpy(buf, iter->data + iter->ptr, bufsize);

    return true;
}

bool zrub_byte_iter_end(struct zrub_byte_iter *iter)
{
    return iter->usage == iter->ptr;
}

u32 zrub_byte_iter_size(struct zrub_byte_iter *iter, i32 blocksize)
{
    u32 len = iter->usage / blocksize;

    if (iter->usage - blocksize * len > 0)
    {
        len++;
    }

    return len;
}
