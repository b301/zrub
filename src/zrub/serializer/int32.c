#include "zrub/serializer/interface.h"


uint8_t zrub_serialize_signed_int32(
    uint8_t     *buf,
    uint32_t    bufsize,
    int32_t     value,
    uint32_t    *offset
)
{
    // buffer too small
    if (bufsize - *offset < 4) {
        return 1;
    }

    // prepend the integer in big endian (uint32_t)
    buf[*offset] = (value >> 24) & 0xFF;
    (*offset)++;
    buf[*offset] = (value >> 16) & 0xFF;
    (*offset)++;
    buf[*offset] = (value >> 8) & 0xFF;
    (*offset)++;
    buf[*offset] = value & 0xFF;
    (*offset)++;

    return 0;
}

uint8_t zrub_deserialize_signed_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    int32_t     *value, 
    uint32_t    *offset
)
{
    // buffer too small to read the integer
    if (bufsize - *offset < 4) {
        return 1;
    }

    // get the integer in big endian (uint32_t)
    *value = 0;
    
    *value |= ((int32_t)buf[*offset]) << 24;
    (*offset)++;
    *value |= ((int32_t)buf[*offset]) << 16;
    (*offset)++;
    *value |= ((int32_t)buf[*offset]) << 8;
    (*offset)++;
    *value |= ((int32_t)buf[*offset]);
    (*offset)++;

    return 0;
}
