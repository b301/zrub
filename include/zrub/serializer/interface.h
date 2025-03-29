#ifndef ZRUB_SERIALIZER_H
#define ZRUB_SERIALIZER_H

// serializers .. using big-endian

#include "zrub/global.h"

#include <string.h>     // memcpy

uint8_t zrub_serialize_signed_int32(
    uint8_t     *buf,
    uint32_t    bufsize,
    int32_t     value,
    uint32_t    *offset
);
uint8_t zrub_deserialize_signed_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    int32_t     *value, 
    uint32_t    *offset
);

uint8_t zrub_serialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    value, 
    uint32_t    *offset
);
uint8_t zrub_deserialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    *value, 
    uint32_t    *offset
);

uint8_t zrub_serialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strsize, 
    uint32_t    *offset
);
uint8_t zrub_deserialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strcap, 
    uint32_t    *offset
);

typedef union {
    float f32;
    uint8_t bytes[4]; 
} zrub_serializer_f32;

uint8_t zrub_serialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       value,
    uint32_t    *offset
);
uint8_t zrub_deserialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       *value,
    uint32_t    *offset
);

uint8_t zrub_serialize_bytes(
    uint8_t     *buf,
    uint32_t    bufsize,
    uint8_t     *bytes,
    uint32_t    bytessize,
    uint32_t    *offset
);
uint8_t zrub_deserialize_bytes(
    uint8_t     *buf,
    uint32_t    bufsize,
    uint8_t     *bytes,
    uint32_t    bytescap,
    uint32_t    *offset
);



#endif // ZRUB_SERIALIZER_H
