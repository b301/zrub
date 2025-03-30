#include "zrub/serializer/interface.h"


uint8_t zrub_serialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strsize, 
    uint32_t    *offset
)
{
    // buffer too small
    if (bufsize - *offset < strsize + 4) {
        return 1;
    }

    // prepend the string size in big endian (uint32_t)
    zrub_serialize_unsigned_int32(buf, bufsize, strsize, offset);
    
    // copy the string to the buffer
    memcpy(buf + *offset, str, strsize);
    *offset += strsize;

    return 0;
}

uint8_t zrub_deserialize_string(
    uint8_t     *buf, 
    uint32_t    bufsize,
    char        *str, 
    uint32_t    strcap, 
    uint32_t    *offset
)
{    
    // buffer too small to read the string size
    if (bufsize - *offset < 4) {
        return 1;
    }

    // get the string size in big endian (uint32_t)
    uint32_t strsize = 0;

    zrub_deserialize_unsigned_int32(buf, bufsize, &strsize, offset);

    // string buffer too small
    if (strcap < strsize) {
        return 2;
    }

    // copy the string to the buffer
    memcpy(str, buf + *offset, strsize);
    *offset += strsize;

    str[strsize] = '\0';

    return 0;
}
