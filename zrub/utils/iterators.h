#ifndef __ZRUB_UITERATOR_H__
#define __ZRUB_UITERATOR_H__

#include "global.h"
#include "zrub/core/time.h"

/** 
 * @brief iterator of byte (u8) arrays 
 * @struct zrub_byte_iter
 * 
 * @param data          byte array to iterate over
 * @param ptr           where the data is now
 * @param usage         how much bytes written to the data buffer
 */
struct zrub_byte_iter {
    u8 *data;
    i32 ptr;
    i32 usage;
};

bool zrub_byte_iter_begin(struct zrub_byte_iter *iter, u8 *block, i32 blocksize);
bool zrub_byte_iter_next(struct zrub_byte_iter *iter, u8 *buf, i32 blocksize);
bool zrub_byte_iter_end(struct zrub_byte_iter *iter);

u32 zrub_byte_iter_size(struct zrub_byte_iter *iter, i32 blocksize);

#endif // _ITERATOR_H
