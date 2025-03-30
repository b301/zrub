#include "zrub/serializer/interface.h"


uint8_t zrub_serialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       value,
    uint32_t    *offset
) {
    // buffer too small
    if (*offset + 4 > bufsize) {
        return 0;
    }

    zrub_serializer_f32 converter;
    converter.f32 = value;

    // Store in big-endian order
    buf[(*offset)++] = converter.bytes[3];
    buf[(*offset)++] = converter.bytes[2];
    buf[(*offset)++] = converter.bytes[1];
    buf[(*offset)++] = converter.bytes[0];

    return 1;
}

// Deserializes a float from big-endian bytes
uint8_t zrub_deserialize_float(
    uint8_t     *buf,
    uint32_t    bufsize,
    float       *value,
    uint32_t    *offset
) {
    // buffer too small to read float
    if (*offset + 4 > bufsize) {
        return 0;
    }

    zrub_serializer_f32 converter;

    // Read in big-endian order
    converter.bytes[3] = buf[(*offset)++];
    converter.bytes[2] = buf[(*offset)++];
    converter.bytes[1] = buf[(*offset)++];
    converter.bytes[0] = buf[(*offset)++];

    *value = converter.f32;
    return 1;
}
