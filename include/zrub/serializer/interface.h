#ifndef OZ_SERIALIZER_H
#define OZ_SERIALIZER_H

// serializers .. using big-endian

#include <stdint.h>     // types
#include <string.h>     // memcpy

uint8_t oz_serialize_signed_int32(
    uint8_t     *buf,
    uint32_t    bufsize,
    int32_t     value,
    uint32_t    *offset
);
uint8_t oz_deserialize_signed_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    int32_t     *value, 
    uint32_t    *offset
);

uint8_t oz_serialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    value, 
    uint32_t    *offset
);
uint8_t oz_deserialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    *value, 
    uint32_t    *offset
);

uint8_t oz_serialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strsize, 
    uint32_t    *offset
);
uint8_t oz_deserialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strcap, 
    uint32_t    *offset
);

typedef union {
    float f32;
    uint8_t bytes[4]; 
} oz_serializer_f32;

uint8_t oz_serialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       value,
    uint32_t    *offset
);
uint8_t oz_deserialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       *value,
    uint32_t    *offset
);



#endif // OZ_SERIALIZER_H
