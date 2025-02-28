#ifndef __ZRUB_TYPES_H__
#define __ZRUB_TYPES_H__

// source: https://en.wikipedia.org/wiki/C_data_types

// gt or eq 0
typedef unsigned char       u8;         // 0 255                        c
typedef unsigned short      u16;        // 0 65,535                     hu
typedef unsigned int        u32;        // 0 4,294,967,295              lu
typedef unsigned long long  u64;        // 0 18446744073709551615       llu

// pm 
typedef signed char         i8;         //   127                        c
typedef signed short        i16;        //   32,767                     hd
typedef signed int          i32;        //   2,147,483,647              ld
typedef signed long long    i64;        //   9,223,372,036,854,775,807  lld

typedef float               f32;
typedef double              f64;
typedef long double         f128;

typedef char*               str;


#endif // __ZRUB_TYPES_H__
