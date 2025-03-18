#ifndef __ZRUB_BYTES_ITERATOR_H__
#define __ZRUB_BYTES_ITERATOR_H__

#include "zrub/global.h"
#include "zrub/core/time.h"

#include <string.h>             /* for: memcpy */

#if defined(__linux__)
#include <sys/random.h>         /* for: getrandom */

#endif

/**
 * @example looping
 * struct zrub_bytes_iter iter;
 * u8 buffer[512];
 * u32 buflen = 0;
 * u32 blocksize = 400;           // make sure this value is less or equal to the buffer size
 * 
 * if (zrub_bytes_iter_begin(&iter, buffer, &buflen, blocksize)) {
 *   do {
 *        ... whatever ...
 *   } while (!zrub_bytes_iter_end(iter) && zrub_bytes_iter_next(&iter, buffer, &buflen, blocksize));
 * }
 */

/** 
 * @brief iterator of byte (u8) arrays 
 * @struct zrub_bytes_iter
 * 
 * @param data          byte array to iterate over
 * @param ptr           where the data is now
 * @param usage         how much bytes written to the data buffer
 */
struct zrub_bytes_iter {
    byte *data;
    i32 ptr;
    i32 usage;
};

bool zrub_randombytes(byte *bytes, u32 size);
bool zrub_bytes_as_hex(const byte *bytes, const u32 bsize, char *hexstr, const u32 dsize);

bool zrub_bytes_iter_begin(struct zrub_bytes_iter *iter, byte *buf, u32 *buflen, i32 blocksize);
bool zrub_bytes_iter_next(struct zrub_bytes_iter *iter, byte *buf, u32 *buflen, i32 blocksize);
bool zrub_bytes_iter_end(const struct zrub_bytes_iter iter);

u32 zrub_bytes_iter_size(const struct zrub_bytes_iter iter, i32 blocksize);


#endif /* __ZRUB_BYTES_ITERATOR_H__ */
