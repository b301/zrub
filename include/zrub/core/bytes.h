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
 * uint8_t buffer[512];
 * uint32_t buflen = 0;
 * uint32_t blocksize = 400;           // make sure this value is less or equal to the buffer size
 * 
 * if (zrub_bytes_iter_begin(&iter, buffer, &buflen, blocksize)) {
 *   do {
 *        ... whatever ...
 *   } while (!zrub_bytes_iter_end(iter) && zrub_bytes_iter_next(&iter, buffer, &buflen, blocksize));
 * }
 */

/** 
 * @brief iterator of bytes (uint8_t) arrays 
 * @struct zrub_bytes_iter
 * 
 * @param data          bytes array to iterate over
 * @param ptr           where the data is now
 * @param usage         how much bytes written to the data buffer
 */
struct zrub_bytes_iter {
    uint8_t *data;
    int32_t ptr;
    int32_t usage;
};

bool zrub_randombytes(uint8_t *bytes, uint32_t size);
bool zrub_bytes_as_hex(const uint8_t *bytes, const uint32_t bsize, char *hexstr, const uint32_t dsize);

bool zrub_bytes_iter_begin(struct zrub_bytes_iter *iter, uint8_t *buf, uint32_t *buflen, int32_t blocksize);
bool zrub_bytes_iter_next(struct zrub_bytes_iter *iter, uint8_t *buf, uint32_t *buflen, int32_t blocksize);
bool zrub_bytes_iter_end(const struct zrub_bytes_iter iter);

uint32_t zrub_bytes_iter_size(const struct zrub_bytes_iter iter, int32_t blocksize);


#endif /* __ZRUB_bytes_ITERATOR_H__ */
