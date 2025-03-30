#include "zrub/serializer/interface.h"


uint8_t zrub_serialize_bytes(
    uint8_t     *buf,
    uint32_t    bufsize,
    uint8_t     *bytes,
    uint32_t    bytessize,
    uint32_t    *offset
)
{
    if (bufsize - *offset < bytessize + 4)
    {
        // buffer too small
        return 1;
    }

    zrub_serialize_unsigned_int32(buf, bufsize, bytessize, offset);

    memcpy(buf + *offset, bytes, bytessize);
    *offset += bytessize;

    return 0;
}

uint8_t zrub_deserialize_bytes(
    uint8_t     *buf,
    uint32_t    bufsize,
    uint8_t     *bytes,
    uint32_t    bytescap,
    uint32_t    *offset
)
{
    if (bufsize - *offset < 4)
    {
        // buffer is too small to contain uint32_t size
        return 1;
    }

    uint32_t bytessize = 0;
    zrub_deserialize_unsigned_int32(buf, bufsize, &bytessize, offset);

    if (bytessize > bytescap)
    {
        // bytes buffer is too small to contain the data
        return 2;
    }

    memcpy(bytes, buf + *offset, bytessize);
    *offset += bytessize;

    return 0;
}
