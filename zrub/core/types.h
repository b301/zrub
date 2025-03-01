#ifndef __ZRUB_TYPES_H__
#define __ZRUB_TYPES_H__

// source: https://en.wikipedia.org/wiki/C_data_types
#include <stdint.h>

// gt or eq 0
typedef uint8_t             u8;         // 0 255                        %c
typedef uint16_t            u16;        // 0 65,535                     %hu
typedef uint32_t            u32;        // 0 4,294,967,295              %lu
typedef uint64_t            u64;        // 0 18446744073709551615       %llu

// pm 
typedef int8_t              i8;         //   127                        %c
typedef int16_t             i16;        //   32,767                     %hd
typedef int32_t             i32;        //   2,147,483,647              %ld
typedef int64_t             i64;        //   9,223,372,036,854,775,807  %lld

typedef float               f32;
typedef double              f64;
typedef long double         f128;

typedef char*               str;


#endif // __ZRUB_TYPES_H__
