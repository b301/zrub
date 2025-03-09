#ifndef __ZRUB_UITERATOR_H__
#define __ZRUB_UITERATOR_H__

#include "zrub/global.h"
#include "zrub/core/time.h"

#include <string.h>             // memcpy

/**
 * @example looping
 * struct zrub_byte_iter iter;
 * u8 buffer[512];
 * u32 buflen = 0;
 * u32 blocksize = 400;           // make sure this value is less or equal to the buffer size
 * 
 * if (zrub_byte_iter_begin(&iter, buffer, &buflen, blocksize)) {
 *   do {
 *        ... whatever ...
 *   } while (!zrub_byte_iter_end(iter) && zrub_byte_iter_next(&iter, buffer, &buflen, blocksize));
 * }
 */

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

bool zrub_byte_iter_begin(struct zrub_byte_iter *iter, u8 *buf, u32 *buflen, i32 blocksize);
bool zrub_byte_iter_next(struct zrub_byte_iter *iter, u8 *buf, u32 *buflen, i32 blocksize);
bool zrub_byte_iter_end(const struct zrub_byte_iter iter);

u32 zrub_byte_iter_size(const struct zrub_byte_iter iter, i32 blocksize);


#endif // _ITERATOR_H
