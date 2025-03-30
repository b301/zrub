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

/**
 * @brief beginning of the iterator
 * 
 * @param iter          the iterator to begin
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_bytes_iter_begin(struct zrub_bytes_iter *iter, uint8_t *buf, uint32_t *buflen, int32_t blocksize);

/**
 * @brief get next iteration
 * 
 * @param iter          the iterator to iterate
 * @param buf           buffer to set as data of blocksize
 * @param blocksize     length of data per iteration
 */
bool zrub_bytes_iter_next(struct zrub_bytes_iter *iter, uint8_t *buf, uint32_t *buflen, int32_t blocksize);

/**
 * @brief checks if reached end-of-iterator
 * 
 * @param iter          the iterator to check
 */
bool zrub_bytes_iter_end(const struct zrub_bytes_iter iter);

/**
 * @brief get the iterator size
 * 
 * @param iter          the iterator whose size is checked
 * @param blocksize     the size of each block
 * @returns size of iterator
 */
uint32_t zrub_bytes_iter_size(const struct zrub_bytes_iter iter, int32_t blocksize);

void zrub_bytes_print(const uint8_t *bytes, uint32_t bufsize);

#endif /* __ZRUB_bytes_ITERATOR_H__ */
