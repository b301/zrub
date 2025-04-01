#include "zrub/serializer/interface.h"


uint8_t zrub_serialize_unsigned_int64(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint64_t    value, 
    uint32_t    *offset
)
{
    // buffer too small
    if (bufsize - *offset < 8) {
        return 1;
    }

    // prepend the integer in big endian (uint64_t)
    buf[*offset] = (value >> 56) & 0xFF;
    (*offset)++;
    buf[*offset] = (value >> 48) & 0xFF;
    (*offset)++;
    buf[*offset] = (value >> 40) & 0xFF;
    (*offset)++;
    buf[*offset] = (value >> 32) & 0xFF;
    (*offset)++;
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

uint8_t zrub_deserialize_unsigned_int64(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint64_t    *value, 
    uint32_t    *offset
)
{
    // buffer too small to read the integer
    if (bufsize - *offset < 8) {
        return 1;
    }

    // get the integer in big endian (uint64_t)
    *value = 0;
    
    *value |= (uint64_t)buf[*offset] << 56;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 48;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 40;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 32;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 24;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 16;
    (*offset)++;
    *value |= (uint64_t)buf[*offset] << 8;
    (*offset)++;
    *value |= (uint64_t)buf[*offset];
    (*offset)++;

    return 0;
}

uint8_t zrub_serialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    value, 
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

uint8_t zrub_deserialize_unsigned_int32(
    uint8_t     *buf, 
    uint32_t    bufsize,
    uint32_t    *value, 
    uint32_t    *offset
)
{
    // buffer too small to read the integer
    if (bufsize - *offset < 4) {
        return 1;
    }

    // get the integer in big endian (uint32_t)
    *value = 0;
    
    *value |= buf[*offset] << 24;
    (*offset)++;
    *value |= buf[*offset] << 16;
    (*offset)++;
    *value |= buf[*offset] << 8;
    (*offset)++;
    *value |= buf[*offset];
    (*offset)++;

    return 0;
}
